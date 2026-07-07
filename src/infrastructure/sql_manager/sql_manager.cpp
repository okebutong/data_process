#include "sql_manager.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include "logger/myLogger/myloggers.h"

SqlManager::SqlManager(QObject *parent):QObject(parent)
{
    m_dataBase = QSqlDatabase::addDatabase("QSQLITE");
    m_dataBase.setDatabaseName(m_sqlitePath);
    if (!m_dataBase.open())
    {
        MyLoggers::getLogger("DataParse")->critical("SqlManager open error {}", m_dataBase.lastError().text().toStdString());
        emit openDatabaseError(m_dataBase.lastError().text());
        return;
    }
    MyLoggers::getLogger("DataParse")->trace("SqlManager init success");

}

SqlManager::~SqlManager()
{
    m_dataBase.close();
    MyLoggers::getLogger("DataParse")->trace("SqlManager destruct success");

}

QSqlDatabase &SqlManager::getInstance()
{
    return m_dataBase;
}
