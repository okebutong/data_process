#ifndef MANAGER_STORAGE_H
#define MANAGER_STORAGE_H

#include <QObject>

class UserData;
class SensorData;
class SqlManager;
class QSqlQuery;
class ManagerStorage : public QObject
{
    Q_OBJECT
public:
    explicit ManagerStorage(QObject *parent = nullptr);
    ~ManagerStorage();
    void insertSensorData(const SensorData &data);
    void setWALConfig();
    QList<SensorData> findSensorDataByTime(quint64 begin, quint64 end);

private :
    void insertSensorDataList();

private:
    QList<SensorData> m_insertDataList;
    SqlManager *m_sqlManager;

   const QString m_sqlitePath = "E:/StudyFile/Qt/2026/serialport/DataProcess/resources/DataParse.db3";
   const QString m_insertSensorDataCommand = "INSERT OR IGNORE INTO SensorData (time,device_id,type,value) VALUES(:time,:device_id,:type,:value)";
   const QString m_findSensorDataByTimeCommand = "SELECT * FROM SensorData WHERE time >= :time1 AND time <= :time2 ORDER BY time";
signals:
    void storageOperatorError(const QString&);
    void openDatabaseError(const QString&);
};

#endif // MANAGER_STORAGE_H
