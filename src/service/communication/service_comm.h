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
        PortOpen,
    }m_portStatus; //用来显示串口的状态
    bool m_getConfigFlag = false; //用来显示是否获取到串口参数的状态
signals:
    void dataReceived(const QByteArray &value);
    void connectionError( QString );
    void connectionOpened();
    void connectionClosed();
    void commDeviceRemove();
};

#endif // SERVICE_COMM_H
