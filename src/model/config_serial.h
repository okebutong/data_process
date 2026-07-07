#ifndef CONFIG_SERIAL_H
#define CONFIG_SERIAL_H

#include <QObject>

class  SerialConfig{
public:
    QString portName;
    qint32 baudRate;
    quint8 dataBit;
    quint8 stopBit;
    quint8 parity;
public:
    void resetConfig()
    {
        portName.clear();
        baudRate = 0;
        dataBit = 0;
        stopBit = 0;
        parity = 0;
    }
};
Q_DECLARE_METATYPE(SerialConfig)


#endif // CONFIG_SERIAL_H
