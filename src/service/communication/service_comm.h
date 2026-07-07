#ifndef SERVICE_COMM_H
#define SERVICE_COMM_H

#include <QObject>
#include <QVariant>
#include "model/config_serial.h"


class SerialComm;
class QThread;


class CommService : public QObject
{
    Q_OBJECT
public:
    explicit CommService(QObject *parent = nullptr);
    ~CommService();
 public slots:
    void startCommunication();
    void stopCommunication();
    void setCommunicationConfig(const SerialConfig &config);
private:
    void readData(const QVariant &data);

private:
    SerialComm *m_serialComm;
    SerialConfig m_configData;
    enum{
        PortClose,
        ConfigGet,
        PortOpen,
    }m_portStatus;
    bool m_getConfigFlag = false;
signals:
    void dataReceived(const QByteArray &value);
    void connectionError( QString );
    void connectionOpened();
    void connectionClosed();
};

#endif // SERVICE_COMM_H
