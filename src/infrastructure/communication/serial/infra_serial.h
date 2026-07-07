#ifndef INFRA_SERIAL_H
#define INFRA_SERIAL_H


#include <QObject>
#include "qserialport.h"
#include "model/config_serial.h"
#include <QVariant>

class QSerialPort;
class QSerialPortInfo;

class SerialComm  : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();
private:
    QSerialPort *m_serialPort;
    QByteArray m_recvArray;

public :

    void readData();
    QMap<bool, QString > openConnection() ;
    void closeConnection() ;
    void setConfig(const SerialConfig &config);

signals:
    void dataReceived(const QByteArray& data);
    void openPortErrorMsg(const QString &);
};


#endif // INFRA_SERIAL_H
