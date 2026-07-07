#ifndef TLVPARSER_H
#define TLVPARSER_H

#include <QObject>
class SensorData;

class TlvParserWorker : public QObject
{
    Q_OBJECT
public:
    enum State {
        WAIT_HEAD1,
        WAIT_HEAD2,
        WAIT_DEV_ID,
        WAIT_DATA_TYPE,
        WAIT_LENGTH,
        WAIT_VALUE,
        WAIT_CHECKNUM,
    };

    enum Event {
        RECV_HEAD1,
        RECV_HEAD2,
        RECV_DEV_ID,
        RECV_DATA_TYPE,
        RECV_LENGTH,
        RECV_VALUE,
        RECV_CHECK_NUM,
    };

public:
    explicit TlvParserWorker(QObject *parent = nullptr);
    ~TlvParserWorker();
    void setCrc16_table();

public slots:
    void parseSensorData(const QByteArray &data);
private:
    void dispatch(quint8 byte);

signals :
    void parseFrameDataError();
    void sensorParsed(const SensorData &data);
private:
    State m_currentState = WAIT_HEAD1;
    std::unique_ptr< SensorData> m_data;
    int m_len;
    quint16 m_crc = 0XFFFF;
    bool m_readCheckNumSecond = false;
    quint16 m_underExam = 0X0000;
    int m_num = 0;
    quint32 m_value = 0;
    int m_index = 0;
    qreal m_currentTime = 0.0;
    QList <quint16> crc16_table;
    void resetToHead1();
    void handlePendingHead1(quint8 byte);
    void handlePendingHead2(quint8 byte);
    void handlePendingDevID(quint8 byte);
    void handlePendingDataType(quint8 byte);
    void handlePendingLength(quint8 byte);
    void handlePendingValue(quint8 byte);
    void handlePendingCheckNum(quint8 byte);
};

#endif