#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H

#include <QObject>
#include  <QSqlDatabase>
#include <QSqlQuery>
class QSqlQueryModel;
class QItemSelectionModel;
class SensorData;


class SqlManager : public QObject
{
    Q_OBJECT
public:
    explicit SqlManager(QObject *parent = nullptr);
    ~SqlManager();
    QSqlDatabase &getInstance();
    SqlManager(const SqlManager &) = delete;
    SqlManager &operator=(const SqlManager &)=delete;

private:
    QSqlDatabase m_dataBase;
    const QString m_sqlitePath = "E:/StudyFile/Qt/2026/serialport/DataProcess/resources/DataParse.db3";
signals:
    void openDatabaseError(const QString &);
};

#endif // SQL_MANAGER_H
