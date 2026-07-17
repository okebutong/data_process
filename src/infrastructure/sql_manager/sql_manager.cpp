#include "sql_manager.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include "logger/myLogger/myloggers.h"
#include <QFile>
SqlManager::SqlManager(QObject *parent):QObject(parent)
{

}

SqlManager::~SqlManager()
{
    m_dataBase.close();
    MyLoggers::getLogger("DataParse")->trace("SqlManager destruct success");
}

SqlManager::DataBaseStatus SqlManager::openDataBase(const QString &fileName)
{
    DataBaseStatus status;
    m_dataBase = QSqlDatabase::addDatabase("QSQLITE","defalut");
    m_dataBase.setDatabaseName(fileName);
    if (!m_dataBase.open())
    {
        MyLoggers::getLogger("DataParse")->critical("SqlManager open error {}", m_dataBase.lastError().text().toStdString());
        status = {false, m_dataBase.lastError().text()};
        return status;
    }

    MyLoggers::getLogger("DataParse")->trace("SqlManager init success");
    status = {true, "success"};
    return status;
}

QSqlDatabase &SqlManager::getInstance()
{
    return m_dataBase;
}




