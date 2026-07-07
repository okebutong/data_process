#include "page_monitor.h"
#include "ui_page_monitor.h"

#include <QtCharts>
#include <QLineSeries>
#include "model/data_sensor.h"

MonitorWidget::MonitorWidget( QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MonitorWidget)
{
    ui->setupUi(this);

    m_beginTime = QDateTime::currentSecsSinceEpoch() / 1.0;
    m_limitSize = 200;
    m_visibleSize = 100;
}

MonitorWidget::~MonitorWidget()
{
    delete ui;
}


/* 20260612 在给定数据时，发现databuf保存所有曲线的数据，会造成取用数据时造成混乱，为避免此类问题，特定给每一个曲线给定一个buf
 * 20260613 解析完之后可以把数据传过来，存到buffer里面，然后buffer根据你的start跟end。 或者max、min，直接来确定显示的地方。
 * 20260613 会存在有相同的index处存储不同time的数据，如果按照index来读取，显示的数据是index，所以应该直接读取database里的数据，读取的范围就是min ，max以及visiable
 * 20260617 创建两个以seriesName为key的链表，一个value为 所对应的曲线的数值 ，另一个value为所对应的曲线，用来保存color
 *
 */
void MonitorWidget::updateData(const SensorData &sensorData)
{
    qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    QString seriesName = QString::asprintf("dev_%d", sensorData.getDeviceId());
    m_valueMax = qMax(m_valueMax, sensorData.getValue());
    m_valueMin = qMin(m_valueMin, sensorData.getValue());
    while(m_dataMap[seriesName].size() > m_limitSize)
    {
        m_dataMap[seriesName].removeFirst();
    }
    addSeries(seriesName, QPointF( sensorData.getTimestamp() - m_beginTime, sensorData.getValue()));
    m_axisX->setRange(currentTime - m_beginTime - m_visibleSize,
                    currentTime - m_beginTime);
    m_axisY->setRange(m_valueMin, m_valueMax);
}



void MonitorWidget::addSeries(const QString &seriesName, const QPointF &point)
{
    if (!m_seriesMap.contains(seriesName))
    {
        QLineSeries *line = new QLineSeries(this);
        QPen pen;
        int r = QRandomGenerator::global()->bounded(0, 255);
        int g = QRandomGenerator::global()->bounded(0, 255);
        int b = QRandomGenerator::global()->bounded(0, 255);
        pen.setColor(QColor(r, g, b));
        pen.setWidth(3);
        pen.setStyle(Qt::SolidLine);
        line->setPen(pen);
        line->setName(seriesName);
        m_chart->addSeries(line);

        line->attachAxis(m_axisX);
        line->attachAxis(m_axisY);
        m_seriesMap.insert(seriesName, line);
        QLegendMarker* marker = m_chart->legend()->markers(line).at(0);
        connect(marker, &QLegendMarker::clicked,
                this, &MonitorWidget::onLegendMarkerClicked);
    }
    QLineSeries *line = m_seriesMap[seriesName];
    while (line->points().size() > m_visibleSize)
    {
        line->removePoints(0, 1);
    }
    line->append(point);

}



void MonitorWidget::createChart(const QString &chartName)
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
    m_axisX->setTickCount(15) ;
    // m_axisX->setReverse(true);
    m_axisX->setLabelsAngle(45);
    m_axisX->setLabelFormat("%0.4f");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 260);
    m_axisY->setTitleText("Value");
    m_axisY->setLabelFormat("%0.0f");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
}

void MonitorWidget::onLegendMarkerClicked()
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
