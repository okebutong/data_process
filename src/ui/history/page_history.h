#ifndef PAGE_HISTORY_H
#define PAGE_HISTORY_H

#include <QWidget>
#include "model/data_sensor.h"

namespace Ui {
class page_history;
}

class QChart;
class QValueAxis;
class QSplineSeries;

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);
    ~HistoryWidget();
    void updateView(const QList<SensorData> &sensorList);
    void updateData(const QList<SensorData> &sensorList);
    void createChart(const QString &chartName);
    void addSeries(const QString &seriesName, const QList<QPointF> &dataList);
    void on_horizontalSlider_min_sliderMoved(int position);
    void on_horizontalSlider_max_sliderMoved(int position);
    void onLegendMarkerClicked();
private:
    void setSliderMaxMin(int min, int max);
signals:
    void historyQueryRequested(quint64 startTime, quint64 endTime);

private slots:
    void on_pushButton_query_clicked();

    void on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime);

    void on_horizontalSlider_min_actionTriggered(int action);

    void on_horizontalSlider_max_actionTriggered(int action);

private:
    Ui::page_history *ui;
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    quint64 m_startTime = 0;
    quint64 m_endTime = 0;
    quint64 m_startMaxTime = 0;
    quint64 m_endMaxTime = 0;
    quint64 m_maxValue = 0;
    quint64 m_minValue = 0;

};

#endif // PAGE_HISTORY_H
