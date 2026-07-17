#include "service_comm.h"
#include "infrastructure/communication/serial/infra_serial.h"
#include "logger/myLogger/myloggers.h"

CommService::CommService(QObject *parent): QObject{parent}
{
    m_serialComm = new SerialComm(this);
    m_portStatus = PortClose;
    connect(m_serialComm, &SerialComm::dataReceived,
            this, &CommService::dataReceived);
    connect(m_serialComm, &SerialComm::serialDeviceRemove, this, &CommService::commDeviceRemove);
    MyLoggers::getLogger("DataParse")->trace("CommService success init");

}

CommService::~CommService()
{
    if (m_serialComm != nullptr && m_portStatus == PortOpen)
    {
        m_serialComm->closeConnection();
        emit connectionClosed();
    }
    MyLoggers::getLogger("DataParse")->trace("CommService success destruct");
}

void CommService::startCommunication()
{
    if (m_portStatus == PortClose && m_getConfigFlag)
    {
       auto status = m_serialComm->openConnection();
        if (status.status)
        {
            m_portStatus = PortOpen;
            MyLoggers::getLogger("DataParse")->trace("CommService success open");
            emit connectionOpened();
        }
        else
        {
            MyLoggers::getLogger("DataParse")->warn("CommService open error");
            emit connectionError(status.errorStr);
        }
    }
    else if (m_getConfigFlag == false)
    {
        emit connectionError("serial have null parma");
    }
}

void CommService::stopCommunication()
{
    if (m_portStatus == PortOpen)
    {
        MyLoggers::getLogger("DataParse")->trace("CommService success close");
        m_serialComm->closeConnection();
        m_portStatus = PortClose;
        emit connectionClosed();
    }

}

void CommService::setCommunicationConfig(const SerialConfig &config)
{
    if (m_portStatus == PortClose)
    {
        m_getConfigFlag = true;
        m_serialComm->setConfig(config);
    }
}
