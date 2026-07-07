#include "page_history.h"
#include "ui_page_history.h"
#include <QtCharts>
#include <QLineSeries>
#include "model/data_sensor.h"
#include <QDateTime>
#include "logger/myLogger/myloggers.h"

HistoryWidget::HistoryWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::page_history)
{
    ui->setupUi(this);
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}

void HistoryWidget::updateData(const QList<SensorData> &sensorList)
{
    m_chart->removeAllSeries();

    QMap<QString, QList<QPointF>>dataMap;
    for(auto sensorData : sensorList)
    {
        qDebug()<<"time:"<<(quint64)sensorData.getTimestamp()
                 <<"value:"<<sensorData.getValue();
    }
    m_startMaxTime = (quint64)sensorList.first().getTimestamp();
    m_endMaxTime = (quint64)sensorList.last().getTimestamp();
    MyLoggers::getLogger("DataParse")->trace("history page start time:{} end time:{}", m_startMaxTime, m_endMaxTime);

    for (auto sensorData : sensorList) {
        m_maxValue = qMax(m_maxValue, (quint64)sensorData.getValue());
        m_minValue = qMin(m_minValue, (quint64)sensorData.getValue());
        QString seriesName = QString::asprintf("dev_%d", sensorData.getDeviceId());
        dataMap[seriesName]<<QPointF(sensorData.getTimestamp() - m_startMaxTime, sensorData.getValue());
    }

    for (auto it = dataMap.cbegin(), end = dataMap.cend(); it != end; ++it)
    {
        addSeries( it.key(), it.value());
    }

    m_axisX->setRange(0, m_endMaxTime - m_startMaxTime);
    m_axisY->setRange(m_minValue, m_maxValue);
    setSliderMaxMin(0, m_endMaxTime - m_startMaxTime);

    for (QLegendMarker* marker : m_chart->legend()->markers()) {
        connect(marker, &QLegendMarker::clicked,
                this, &HistoryWidget::onLegendMarkerClicked);
    }
}

void HistoryWidget::createChart(const QString &chartName)
{
    m_chart = new QChart();
    ui->graphicsView->setChart(m_chart);
    m_chart->setTitle(chartName);

    QFont font;
    font.setPointSize(20);
    m_chart->setTitleFont(font);

    m_axisX = new QValueAxis();
    m_axisX->setTitleText("Time");
    m_axisX->setRange(0, 20);
    m_axisX->setReverse(true);
    m_axisX->setTickCount(15) ;

    m_axisX->setLabelsAngle(45);
    m_axisX->setLabelFormat("%0.4f");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 260);
    m_axisY->setTitleText("Value");
    m_axisY->setLabelFormat("%0.0f");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
}

void HistoryWidget::addSeries(const QString &seriesName, const QList<QPointF> &dataList)
{
    QSplineSeries *line = new QSplineSeries();
    QPen pen;
    int r = QRandomGenerator::global()->bounded(0, 255);
    int g = QRandomGenerator::global()->bounded(0, 255);
    int b = QRandomGenerator::global()->bounded(0, 255);
    pen.setColor(QColor(r, g, b));
    pen.setWidth(3);
    pen.setStyle(Qt::DotLine);
    line->setPen(pen);
    line->setName(seriesName);
    m_chart->addSeries(line);

    line->attachAxis(m_axisX);
    line->attachAxis(m_axisY);
    line->replace(dataList);
}

void HistoryWidget::on_horizontalSlider_min_sliderMoved(int position)
{
    if (position > ui->horizontalSlider_max->value()) {
        position = ui->horizontalSlider_max->value() - 10;
    }
    MyLoggers::getLogger("DataParse")->trace("min slider position:{}, value:{}", position, ui->horizontalSlider_min->value());
    m_axisX->setMin(position);
}


void HistoryWidget::on_horizontalSlider_max_sliderMoved(int position)
{
    if (position < ui->horizontalSlider_min->value()) {
        position = ui->horizontalSlider_min->value() + 10;
    }
    MyLoggers::getLogger("DataParse")->trace("max slider position:{}, value:{}", position, ui->horizontalSlider_min->value());
    m_axisX->setMax(position);
}

void HistoryWidget::onLegendMarkerClicked()
{
    QLegendMarker* marker= qobject_cast<QLegendMarker*> (sender());
    marker->series()->setVisible(!marker->series()->isVisible()); //序列的可见性
    marker->setVisible(true);
    qreal alpha= 1.0;
    if (!marker->series()->isVisible())
        alpha= 0.3; //设置为半透明表示序列不可见
    QBrush brush= marker->labelBrush();
    QColor color= brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setLabelBrush(brush); //设置文字的 brush
    brush= marker->brush();
    color= brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setBrush(brush); //设置图例标记
}

void HistoryWidget::setSliderMaxMin(int min, int max)
{
    ui->horizontalSlider_min->setMinimum(min);
    ui->horizontalSlider_min->setMaximum(max);
    ui->horizontalSlider_max->setMinimum(min);
    ui->horizontalSlider_max->setMaximum(max);
}
void HistoryWidget::on_pushButton_query_clicked()
{
    qDebug()<<"start"<<m_startTime<<"end"<<m_endTime;
    emit historyQueryRequested(m_startTime, m_endTime);
}


void HistoryWidget::on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime)
{
    qDebug()<<"start"<<dateTime.toString("yyyy-MM-dd h:mm:ss");
    m_startTime = dateTime.toSecsSinceEpoch();
}


void HistoryWidget::on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime)
{
    m_endTime = dateTime.toSecsSinceEpoch();
}


void HistoryWidget::on_horizontalSlider_min_actionTriggered(int action)
{
    qDebug()<<"position"<<action<<"min"<<ui->horizontalSlider_min->value();
    int position = ui->horizontalSlider_min->value();
    if (position > ui->horizontalSlider_max->value()) {
        position = ui->horizontalSlider_max->value() - 10;
    }
    m_axisX->setMin(position);
}


void HistoryWidget::on_horizontalSlider_max_actionTriggered(int action)
{
    qDebug()<<"position"<<action<<"min"<<ui->horizontalSlider_max->value();
    int position = ui->horizontalSlider_max->value();
    if (position < ui->horizontalSlider_min->value()) {
        position = ui->horizontalSlider_min->value() + 10;
    }
    m_axisX->setMax(position);
}
