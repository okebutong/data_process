#include "page_serial_config.h"
#include "ui_page_serial_config.h"
#include <QSerialPortInfo>
#include "logger/myLogger/myloggers.h"
#include "model/config_serial.h"

SerialConfigPage::SerialConfigPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TSerialConfig)
{
    ui->setupUi(this);

    foreach (const auto &item, QSerialPortInfo::availablePorts())
        ui->comboBox_port->addItem(item.portName());

    foreach(const auto &item, QSerialPortInfo::standardBaudRates())
        ui->comboBox_badu_rate->addItem(QString::number(item));

    ui->comboBox_badu_rate->setCurrentIndex(12);

    QStringList dataFlag;
    dataFlag<<"5"<<"6"<<"7"<<"8";
    ui->comboBox_data_bit->addItems(dataFlag);
    ui->comboBox_data_bit->setCurrentIndex(3);
    QStringList stopFlag;
    stopFlag<<"1"<<"2"<<"1.5";
    ui->comboBox_stop_bit->addItems(stopFlag);
    QStringList parityFlag;
    parityFlag<<"No"<<"Evne"<<"Odd";
    ui->comboBox_parity->addItems(parityFlag);
    MyLoggers::getLogger("DataParse")->info("class SerialConfigPage destract");

}

SerialConfigPage::~SerialConfigPage()
{
    MyLoggers::getLogger("DataParse")->info("class SerialConfigPage destract");
    delete ui;
}

int SerialConfigPage::parityValue(int index)
{
    int num = 0;
    switch (index) {
    case 0:
        num = 0;
        break;
    case 1:
        num = 2;
        break;
    case 2:
        num = 3;
        break;
    default:
        break;
    }
    return num;
}

void SerialConfigPage::on_pushButton_OK_clicked()
{
    SerialConfig data;
    data.resetConfig();
    data.portName.append(ui->comboBox_port->currentText());
    data.baudRate = ui->comboBox_badu_rate->currentText().toInt();
    data.stopBit = ui->comboBox_stop_bit->currentIndex() + 1;
    data.dataBit = ui->comboBox_data_bit->currentText().toInt();
    data.parity = parityValue(ui->comboBox_parity->currentIndex());
    emit configSubmitted(data);
    this->close();
}
