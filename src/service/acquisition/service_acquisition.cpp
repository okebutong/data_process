#include "service_acquisition.h"
#include "core/tlvparser/tlvparser.h"
#include <QThread>
#include "service/communication/service_comm.h"
#include "service/storage/service_storage.h"
#include <QPointF>
#include <QTimer>
#include <QDateTime>
#include <QMetaObject>
#include "logger/myLogger/myloggers.h"


Acquisition::Acquisition(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<SensorData>();
    auto logger = MyLoggers::getLogger("DataParse");

    m_storage = new Storage();
    m_storageThread = new QThread(this);
    if (!m_storage->moveToThread(m_storageThread))
    {
        logger->error("[{}] [{}] {} move to {} error", __FILE__, __LINE__, "m_storage", "m_storageThread");
    }

    connect(m_storageThread, &QThread::finished, m_storage, &QObject::deleteLater);
    connect(m_storageThread, &QThread::started, m_storage, &Storage::initStorage);
    connect(m_storage, &Storage::historyReady, this, &Acquisition::historyListReady);
    connect(m_storage, &Storage::operatorError, this, &Acquisition::operatorErrorString);
    connect(m_storage, &Storage::openError, this, &Acquisition::openErrorHandle);


    m_parseWorker = new TlvParserWorker();
    m_parseThread = new QThread(this);
    if (!m_parseWorker->moveToThread(m_parseThread))
    {
        logger->error("[{}] [{}] {} move to {} error", __FILE__, __LINE__, "m_parseWorker", "m_parseThread");
    }

    connect(m_parseThread, &QThread::finished, m_parseWorker, &QObject::deleteLater);
    connect(m_parseWorker, &TlvParserWorker::sensorParsed, m_storage, &Storage::storageSaveData);
    connect(m_parseWorker, &TlvParserWorker::sensorParsed, this, &Acquisition::parsedDataReceived);
    connect(m_parseWorker, &TlvParserWorker::parseFrameDataError, this, &Acquisition::parsedErrorHandler);

    m_commService = new CommService();
    m_commThread = new QThread(this);
    if (!m_commService->moveToThread(m_commThread))
    {
        logger->error("[{}] [{}] {} move to {} error", __FILE__, __LINE__, "m_commService", "m_commThread");
    }

    connect(m_commThread, &QThread::finished, m_commService, &QObject::deleteLater);
    connect(m_commService, &CommService::connectionOpened, this, &Acquisition::connectionStarted);
    connect(m_commService, &CommService::connectionClosed, this, &Acquisition::connectionStopped);
    connect(m_commService, &CommService::dataReceived, m_parseWorker, &TlvParserWorker::parseSensorData);
    connect(m_commService, &CommService::connectionError, this, &Acquisition::serialPortOpenError);
    connect(m_commService, &CommService::commDeviceRemove, this, &Acquisition::deviceRemove);

    m_storageThread->start(QThread::NormalPriority);
    logger->info("m_storageThread success start");

    m_commThread->start();
    logger->info("m_commThread success start");

    m_parseThread->start(QThread::HighPriority);
    logger->info("m_parseThread success start");

    logger->info("acquisition service success init");
}

Acquisition::~Acquisition()
{
    m_commThread->quit();
    m_parseThread-> quit();
    m_storageThread->quit();
    m_storageThread->wait();
    m_parseThread->wait();
    m_commThread->wait();
}

void Acquisition::startComm()
{
    QMetaObject::invokeMethod(m_commService, "startCommunication");
    MyLoggers::getLogger("DataParse")->info("Communication Service Success Start");
}

void Acquisition::stopComm()
{
    QMetaObject::invokeMethod(m_commService, "stopCommunication");
    MyLoggers::getLogger("DataParse")->info("Communication Service Success stop");
}

void Acquisition::acqFindHistoryData(quint64 startTime, quint64 endTime)
{
    QMetaObject::invokeMethod(m_storage, "findHistoryData", Qt::QueuedConnection,
                              Q_ARG(quint64, startTime), Q_ARG(quint64, endTime));
}

void Acquisition::acqReceiveConfig(const SerialConfig &config)
{
    QMetaObject::invokeMethod(m_commService, "setCommunicationConfig", config);
}

void Acquisition::parsedErrorHandler()
{
    m_parsedErrorByte++;
    emit parsedErrorByte(m_parsedErrorByte);
}

void Acquisition::openErrorHandle(const QString &error)
{
    emit openStorageErrorString(error);
    m_commThread->quit();
    m_parseThread-> quit();
    m_storageThread->quit();
    m_storageThread->wait();
    m_parseThread->wait();
    m_commThread->wait();
}
