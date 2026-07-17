#ifndef SERVICE_ACQUISITION_H
#define SERVICE_ACQUISITION_H

#include <QObject>
#include "model/data_sensor.h"
class TlvParserWorker;
class SerialConfig;
class Storage;
class CommService;

class QThread;

class Acquisition : public QObject
{
    Q_OBJECT
public:
    explicit Acquisition(QObject *parent = nullptr);
    ~Acquisition();
    void startComm();
    void stopComm(void);
public slots:
    void acqFindHistoryData(quint64 startTime, quint64 endTime);
    void acqReceiveConfig(const SerialConfig &config);

private slots:
    void parsedErrorHandler();
    void openErrorHandle(const QString&);
signals:
    void openStorageErrorString(const QString&);
    void receivedConfig(const SerialConfig &config);
    void parsedDataReceived(const SensorData &data);
    void historyListReady(const QList<SensorData> &data);
    void connectionStarted();
    void connectionStopped();

    void serialPortOpenError(const QString &);


    void operatorErrorString(const QString &);    //还需要在ui中添加text
    void parsedErrorByte(quint32);

    void deviceRemove();

private:
    CommService *m_commService;
    QThread *m_commThread;
    TlvParserWorker *m_parseWorker;
    QThread *m_parseThread;
    Storage *m_storage;
    QThread *m_storageThread;
    quint32 m_parsedErrorByte = 0;
};

#endif // SERVICE_ACQUISITION_H
