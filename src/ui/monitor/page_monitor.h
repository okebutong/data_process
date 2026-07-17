#ifndef PAGE_MONITOR_H
#define PAGE_MONITOR_H

#include <QWidget>
#include "model/data_sensor.h"

namespace Ui {
class MonitorWidget;
}
class QChart;
class QValueAxis;
class QLineSeries;

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget( QWidget *parent = nullptr);
    ~MonitorWidget();
    void createChart(const QString & chartName);

private:
    void addSeries(const QString &seriesName, const QPointF &point);

private slots:
    void onLegendMarkerClicked();

public slots:
    void updateData(const SensorData &data);

private:
    Ui::MonitorWidget *ui;
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    int m_limitSize;
    int m_visibleSize;
    qreal m_valueMax = 0.0;
    qreal m_valueMin = 0.0;
    qint64 m_beginTime;
    QMap<QString, QLineSeries*>m_seriesMap;

    QPoint beginPoint; //选择矩形区域的起点
    QPoint endPoint; //选择矩形区域的终点
    bool m_customZoom= true; //是否使用自定义矩形放大模式

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};



#endif // PAGE_MONITOR_H
