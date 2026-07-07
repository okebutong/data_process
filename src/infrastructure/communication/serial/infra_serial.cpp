#include "infra_serial.h"
#include <QVariant>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "logger/myLogger/myloggers.h"

SerialComm::SerialComm(QObject *parent):QObject(parent)
{
    m_serialPort = new QSerialPort(parent);
    MyLoggers::getLogger("DataParse")->trace("SerialComm init success");
}

SerialComm::~SerialComm()
{
    MyLoggers::getLogger("DataParse")->trace("SerialComm destruct success");
}

QMap<bool, QString > SerialComm::openConnection()
{
    QMap<bool, QString>map;
    if (!m_serialPort->open(QSerialPort::ReadOnly))
    { //这个地方打不开，也没必要再上一级重试，直接返回错误原因
        MyLoggers::getLogger("DataParse")->critical("[{}] [{}] serial port open error {}", m_serialPort->errorString().toStdString(), __FILE__, __LINE__);
        emit openPortErrorMsg(m_serialPort->errorString());
        map.insert(false,m_serialPort->errorString());
        return map;
    }

    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialComm::readData);
    MyLoggers::getLogger("DataParse")->trace("serial port open succ");
    map.insert(true,"success");
    return map;
}

void SerialComm::closeConnection()
{
    disconnect(m_serialPort, &QSerialPort::readyRead, this, &SerialComm::readData);
    m_serialPort->close();
    MyLoggers::getLogger("DataParse")->trace("serial port close");
}

void SerialComm::setConfig(const SerialConfig &config)
{

    m_serialPort->setBaudRate(QSerialPort::BaudRate(config.baudRate));
    m_serialPort->setDataBits(QSerialPort::DataBits(config.dataBit));
    m_serialPort->setParity(QSerialPort::Parity(config.parity));
    m_serialPort->setPortName(config.portName);
    m_serialPort->setStopBits(QSerialPort::StopBits( config.stopBit));
    MyLoggers::getLogger("DataParse")->info("receive port name：{} bandrate:{} parity:{} stop bit:{} data bit:{}",
                                            config.portName.toStdString(),
                                            config.baudRate,config.parity, config.stopBit,config.dataBit);
}

void SerialComm::readData()
{
    m_recvArray.clear();
    m_recvArray = m_serialPort->readAll();
    MyLoggers::getLogger("DataParse")->trace("recvive date {} byte", m_recvArray.size());
    if (m_recvArray.size() != 0)
    {
        emit dataReceived(m_recvArray);
    }
    else
    {
        MyLoggers::getLogger("DataParse")->warn("recvive empty buffer");
    }
}
