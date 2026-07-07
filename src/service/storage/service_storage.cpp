#include "service_storage.h"
#include "repository/manager_storage/manager_storage.h"
#include "logger/myLogger/myloggers.h"
#include <QThread>

Storage::Storage(QObject *parent): QObject(parent)
{

}

Storage::~Storage()
{
    MyLoggers::getLogger("DataParse")->trace("[storage service] [dastruct] success");
}

void Storage::initStorage()
{
    m_storage = new ManagerStorage();
    m_storage->setWALConfig();
    connect(m_storage, &ManagerStorage::storageOperatorError, this, &Storage::operatorError);
    connect(m_storage, &ManagerStorage::openDatabaseError, this, &Storage::openError);

}

void Storage::storageSaveData(const SensorData &data)
{
    if (m_storage == nullptr)
    {
        MyLoggers::getLogger("DataParse")->trace("[storage service] [init] error,try init again");
        initStorage();
    }
    m_storage->insertSensorData(data);
}

void Storage::findHistoryData(quint64 begin, quint64 end)
{
    if (begin > end)
    {
        emit historyReady( QList<SensorData>());
        MyLoggers::getLogger("DataParse")->trace("[storage service] [find operatro]error: time begin:{} > end:{}", begin, end);
        begin = end - 10;
    }
    if (m_storage == nullptr)
    {
        MyLoggers::getLogger("DataParse")->trace("[storage service] [init] error,try init again");
        initStorage();
    }
    QList<SensorData>list= m_storage->findSensorDataByTime(begin, end);
    emit historyReady(list);
}
