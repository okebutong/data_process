#ifndef DATA_SENSOR_H
#define DATA_SENSOR_H
#include <QObject>

class SensorData
{
private:
    qreal timestamp;
    uint16_t deviceId;
    uint dataType;
    qreal value;
public :
    inline SensorData() : timestamp(0.0),
        deviceId(std::move(0)),
        dataType(std::move(0)),
        value(std::move(0))
    {}
    void clear()
    {
        timestamp = 0.0;
        deviceId = 0;
        dataType = 0;
        value = 0.0;
    }
    qreal getTimestamp() const;
    void setTimestamp(qreal newTimestamp);
    uint16_t getDeviceId() const;
    void setDeviceId(uint16_t newDeviceId);
    uint getDataType() const;
    void setDataType(uint newDataType);
    qreal getValue() const;
    void setValue(qreal newValue);
    SensorData getInstance();

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

inline SensorData SensorData::getInstance()
{
    return *this;
}

inline qreal SensorData::getTimestamp() const
{
    return timestamp;
}

inline void SensorData::setTimestamp(qreal newTimestamp)
{
    timestamp = newTimestamp;
}

Q_DECLARE_METATYPE(SensorData);
#endif // DATA_SENSOR_H

