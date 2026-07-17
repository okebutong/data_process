#include "manager_storage.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include <QTimer>
#include <QFile>
#include "infrastructure/sql_manager/sql_manager.h"
#include "model/data_sensor.h"
#include "logger/myLogger/myloggers.h"


ManagerStorage::ManagerStorage(QObject *parent):QObject(parent)
{
    m_sqlManager = new SqlManager(parent);
    MyLoggers::getLogger("DataParse")->trace("ManagerStorage init success");

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &ManagerStorage::insertSensorDataList);
    timer->start();
}



bool ManagerStorage::openFile(void)
{
    QFile file(m_sqlitePath);
    if (!file.exists())
    {
        //file not exist
        if (!file.open(QIODeviceBase::ReadWrite))
        {
            file.close();
            emit openStorageError(file.errorString());
            MyLoggers::getLogger("DataParse")->info("SqlManager create file error {}", file.errorString().toStdString());
            return false;
        }
    }
    auto status = m_sqlManager->openDataBase(m_sqlitePath);
    if (!status.status)
    {
        emit openStorageError(status.errorStr);
        return false;
    }
    QSqlDatabase sql = m_sqlManager->getInstance();
    if (!sql.tables().contains(m_tableName))
    {
        MyLoggers::getLogger("DataParse")->critical("SqlManager database not have table {},error {}",m_tableName.toStdString(), sql.lastError().text().toStdString());
        QSqlQuery query(sql);

        bool res = query.exec(m_createDatabaseCommand);
        if (res)
            MyLoggers::getLogger("DataParse")->info("SqlManager create table success");
        else
        {
            MyLoggers::getLogger("DataParse")->critical("SqlManager create file success ");
            emit openStorageError(QString("New database can not create table"));
            return false;
        }
    }
    return true;
}

ManagerStorage::~ManagerStorage()
{
    insertSensorDataList();
    MyLoggers::getLogger("DataParse")->trace("ManagerStorage destruct success");
}


void ManagerStorage::insertSensorData(const SensorData &sensorData)
{
    // 设定一个buf 每次向写入到buf中，直到写满limitSize个或者等待时间超过1s自动提交
    int limitSize = 50;

    m_insertDataList.append(sensorData);

    if (m_insertDataList.size() >= limitSize)
    {
        insertSensorDataList();
    }
}
void ManagerStorage::insertSensorDataList()
{
    bool result;
    if (m_insertDataList.isEmpty())
        return ;
    MyLoggers::getLogger("DataParse")->trace("ManagerStorage insert data size:{}", m_insertDataList.size());

    QSqlDatabase sql = m_sqlManager->getInstance();
    sql.transaction();
    QSqlQuery query(sql);
    if (!query.prepare(m_insertSensorDataCommand))
    {
        MyLoggers::getLogger("DataParse")->warn("ManagerStorage insert query text {0}",query.lastQuery().toStdString());
        emit storageOperatorError(query.lastError().text());
        return ;
    }
    const QList<SensorData> List = m_insertDataList;
    for (auto &varData : List) {
        query.bindValue(":time", varData.getTimestamp());
        query.bindValue(":device_id", varData.getDeviceId());
        query.bindValue(":type", varData.getDataType());
        query.bindValue(":value", varData.getValue());
        result = query.exec();
        MyLoggers::getLogger("DataParse")->trace("ManagerStorage insert query text {0}",query.lastQuery().toStdString());

        if (!result)
        {
            MyLoggers::getLogger("DataParse")->warn("ManagerStorage insert error {}",query.lastError().text().toStdString());
            sql.rollback();
            m_insertDataList.clear();
            emit storageOperatorError(query.lastError().text());
            return;
        }
    }
    sql.commit();
    MyLoggers::getLogger("DataParse")->info("ManagerStorage insert commit success");

    m_insertDataList.clear();

}


void ManagerStorage::setWALConfig()
{
    QSqlDatabase sql = m_sqlManager->getInstance();
    QSqlQuery pragma(sql);
    pragma.exec("PRAGMA journal_mode=WAL");
    pragma.exec("PRAGMA synchronous=NORMAL");
}

QList<SensorData> ManagerStorage::findSensorDataByTime(quint64 begin, quint64 end)
{
    QSqlDatabase db = m_sqlManager->getInstance();
    bool result;
    QSqlQuery query(db);
    QList<SensorData> dataList;
    dataList.clear();

    QString sqlCommand = QString(m_findSensorDataByTimeCommand);
    if (!query.prepare(sqlCommand))
    {
        emit storageOperatorError(query.lastError().text());
        return QList<SensorData>{};
    }
    query.bindValue(":time1", begin);
    query.bindValue(":time2", end);
    result = query.exec();
    MyLoggers::getLogger("DataParse")->trace("ManagerStorage find query text {}",query.executedQuery().toStdString());

    if (!result)
    {
        MyLoggers::getLogger("DataParse")->warn("ManagerStorage find exec error{}",query.lastError().text().toStdString());
        emit storageOperatorError(query.lastError().text());
        return QList<SensorData>(0);
    }

    if (!query.first())
    {
        MyLoggers::getLogger("DataParse")->trace("ManagerStorage find result is empty");
        return QList<SensorData>(0);
    }
    while(query.isValid())
    {
        std::unique_ptr< SensorData> data=  std::make_unique<SensorData>();
        data->setDataType(query.value("type").toInt());
        data->setDeviceId(query.value("device_id").toInt());
        data->setTimestamp(query.value("time").toReal(&result));
        if (!result)
        {
            data->clear();
            query.next();
            continue;
        }

        data->setValue(query.value("value").toReal(&result));
        if (!result)
        {
            data->clear();
            query.next();
            continue;
        }
        dataList << data->getObject();
        query.next();
    }
    MyLoggers::getLogger("DataParse")->info("ManagerStorage find success list size:{}",dataList.size());
    return dataList;
}


