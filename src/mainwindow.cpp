#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDateTime>
#include <QLabel>
#include <QMessageBox>
#include "ui/comm_config/serial/page_serial_config.h"
#include "service/acquisition/service_acquisition.h"
#include "logger/myLogger/myloggers.h"
#include "ui/monitor/page_monitor.h"
#include "ui/history/page_history.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MyLoggers::init("DataParse");
    MyLoggers::init("parse_data");

    auto logger = MyLoggers::getLogger("DataParse");

    ui->setupUi(this);
    logger->info("ui init ready");

    loadMonitorPage();
    logger->info("monitor page load ready");

    loadHistoryPage();
    logger->info("history page load ready");

    loadStatusLabel();
    logger->info("status label load ready");

    m_acqService = new Acquisition(this);

    connect(m_acqService, &Acquisition::parsedDataReceived, m_monitorWidget, &MonitorWidget::updateData);
    connect(m_acqService, &Acquisition::connectionStarted, this, &MainWindow::changeStartStatus);
    connect(m_acqService, &Acquisition::connectionStopped, this, &MainWindow::changeStopStatus);
    connect(m_acqService, &Acquisition::serialPortOpenError, this, &MainWindow::openErrorDialog);
    connect(m_acqService, &Acquisition::openErrorString, this, &MainWindow::openErrorDialog);

    connect(m_acqService, &Acquisition::operatorErrorString, this, &MainWindow::printErrorMsg);

    connect(m_acqService, &Acquisition::parsedErrorByte, this, &MainWindow::changeErrorByte);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadHistoryPage()
{
    m_historyWidget = new HistoryWidget(ui->hitsory);
    QVBoxLayout * verticalLayout_history= new QVBoxLayout(ui->hitsory);
    verticalLayout_history->addWidget(m_historyWidget);
    m_historyWidget->setAttribute(Qt::WA_DeleteOnClose);
    m_historyWidget->createChart("历史数据查询");
}

void MainWindow::loadMonitorPage()
{
    m_monitorWidget = new MonitorWidget(ui->realtime);
    QVBoxLayout * verticalLayout_monitor= new QVBoxLayout(ui->realtime);
    verticalLayout_monitor->addWidget(m_monitorWidget);
    m_monitorWidget->setAttribute(Qt::WA_DeleteOnClose);
    m_monitorWidget->createChart("实时数据监控");
}

void MainWindow::loadStatusLabel()
{
    m_connStatusLabel = new QLabel ("已断开",this);
    m_parsedErrorByteLabel = new QLabel(this);
    m_receivedDataByte = new QLabel(this);

    ui->statusBar->addWidget(m_connStatusLabel);
    ui->statusBar->addWidget(m_parsedErrorByteLabel);
    ui->statusBar->addWidget(m_receivedDataByte);
}

void MainWindow::on_widget_data_currentChanged(int index)
{
    if (index == 0)
    {
        disconnect(m_historyWidget, &HistoryWidget::historyQueryRequested,m_acqService, &Acquisition::acqFindHistoryData);
        connect(m_acqService, &Acquisition::parsedDataReceived, m_monitorWidget, &MonitorWidget::updateData);
        MyLoggers::getLogger("DataParse")->info("sub table switch to monitor page");
    }
    else
    {
        disconnect(m_acqService, &Acquisition::parsedDataReceived, m_monitorWidget, &MonitorWidget::updateData);
        connect(m_historyWidget, &HistoryWidget::historyQueryRequested, m_acqService, &Acquisition::acqFindHistoryData);
        connect(m_acqService, &Acquisition::historyListReady, m_historyWidget, &HistoryWidget::updateData);
        MyLoggers::getLogger("DataParse")->info("sub table switch to history page");
    }
}


void MainWindow::on_action_connection_triggered()
{
    m_acqService->startComm();
}

void MainWindow::on_action_disconnection_triggered()
{
    m_acqService->stopComm();
}

void MainWindow::on_action_config_triggered()
{
    m_commConfigPage = new SerialConfigPage();
    connect(m_commConfigPage, &SerialConfigPage::configSubmitted, m_acqService, &Acquisition::acqReceiveConfig);

    m_commConfigPage->setAttribute(Qt::WA_DeleteOnClose);
    m_commConfigPage->setWindowTitle("Set serial config");
    m_commConfigPage->show();

    ui->action_config->setEnabled(false);
    ui->action_connection->setEnabled(true);
    ui->action_disconnection->setEnabled(false);
    MyLoggers::getLogger("DataParse")->info("open config windows");
}

void MainWindow::changeStartStatus()
{
    ui->action_config->setEnabled(false);
    ui->action_connection->setEnabled(false);
    ui->action_disconnection->setEnabled(true);
    m_connStatusLabel->setText("连接状态:已连接");
    MyLoggers::getLogger("DataParse")->info("communication opened");
}

void MainWindow::changeStopStatus()
{
    ui->action_config->setEnabled(true);
    ui->action_connection->setEnabled(true);
    ui->action_disconnection->setEnabled(false);
    m_connStatusLabel->setText("连接状态:已断开");
    MyLoggers::getLogger("DataParse")->info("communication closed");

}

void MainWindow::openErrorDialog(QString errorMsg)
{
    QMessageBox::warning(this, "ERROR",errorMsg);
    ui->action_config->setEnabled(true);
    ui->action_connection->setEnabled(false);
    //错误后，重新设置，无法打开窗口
}

void MainWindow::printErrorMsg(const QString &msg)
{
    ui->plainTextEdit_error_msg->appendPlainText(msg);
}

void MainWindow::changeErrorByte(quint32 byte)
{
    m_parsedErrorByteLabel->setText(QString("解析错误字节数:%1").arg(byte));
}

void MainWindow::changeReceivedByte(quint32 byte)
{
    m_receivedDataByte->setText(QString("已接收字节数:%1").arg(byte));
}
