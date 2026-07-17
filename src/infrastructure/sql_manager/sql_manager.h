#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H

#include <QObject>
#include <QSqlDatabase>

class SqlManager : public QObject
{
    Q_OBJECT
    struct DataBaseStatus
    {
        bool status;
        QString errorStr;
    };
public:
    explicit SqlManager(QObject *parent = nullptr);
    ~SqlManager();
    QSqlDatabase &getInstance();
    SqlManager(const SqlManager &) = delete;
    SqlManager &operator=(const SqlManager &)=delete;
    DataBaseStatus openDataBase(const QString &fileName);


private:
    QSqlDatabase m_dataBase;
signals:
    void openDatabaseError(const QString &);
};

#endif // SQL_MANAGER_H
