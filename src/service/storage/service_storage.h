#ifndef SERVICE_STORAGE_H
#define SERVICE_STORAGE_H

#include <QObject>
class ManagerStorage;
class QThread;
#include "model/data_sensor.h"

class Storage : public QObject
{
    Q_OBJECT
public:
    explicit Storage(QObject *parent = nullptr);
    ~Storage();

signals:
    void historyReady(const QList<SensorData>&);
    void operatorError(const QString &);
    void openError(const QString &);
public slots:
    void storageSaveData(const SensorData &data);
    void initStorage();
    void findHistoryData(quint64 begin, quint64 end);

private:
    ManagerStorage *m_storage;
};

#endif // SERVICE_STORAGE_H
