#ifndef DATA_SENSOR_H
#define DATA_SENSOR_H
#include <QObject>

class SensorData
{
private:
    qint64 timestamp;
    uint16_t deviceId;
    uint dataType;
    qreal value;
public :
    inline SensorData() : timestamp(0),
        deviceId(0),
        dataType(0),
        value(0)
    {}
    void clear()
    {
        timestamp = 0.0;
        deviceId = 0;
        dataType = 0;
        value = 0.0;
    }
    qint64 getTimestamp() const;
    void setTimestamp(qint64 newTimestamp);
    uint16_t getDeviceId() const;
    void setDeviceId(uint16_t newDeviceId);
    uint getDataType() const;
    void setDataType(uint newDataType);
    qreal getValue() const;
    void setValue(qreal newValue);
    SensorData getObject();
};

inline uint16_t SensorData::getDeviceId() const
{
    return deviceId;
}

inline void SensorData::setDeviceId(uint16_t newDeviceId)
{
    deviceId = newDeviceId;
}

inline uint SensorData::getDataType() const
{
    return dataType;
}

inline void SensorData::setDataType(uint newDataType)
{
    dataType = newDataType;
}

inline qreal SensorData::getValue() const
{
    return value;
}

inline void SensorData::setValue(qreal newValue)
{
    value = newValue;
}

inline SensorData SensorData::getObject()
{
    return *this;
}

inline qint64 SensorData::getTimestamp() const
{
    return timestamp;
}

inline void SensorData::setTimestamp(qint64 newTimestamp)
{
    timestamp = newTimestamp;
}

Q_DECLARE_METATYPE(SensorData);
#endif // DATA_SENSOR_H

