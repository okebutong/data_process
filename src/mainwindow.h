#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model/config_serial.h"
class SerialConfigPage;

class Acquisition;
class SensorData;
class QValueAxis;
class MonitorWidget;
class HistoryWidget;
class QLabel;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void loadHistoryPage();

    void loadMonitorPage();

    void loadStatusLabel();

    void test();
private slots:

    void on_widget_data_currentChanged(int index);

    void on_action_connection_triggered();

    void on_action_disconnection_triggered();

    void on_action_config_triggered();

    void changeStartStatus();

    void changeStopStatus();

    void openErrorDialog(QString);

    void printErrorMsg(const QString &);

    void changeErrorByte(quint32 byte);

    void changeReceivedByte(quint32 byte);
private:
    Ui::MainWindow *ui;
    Acquisition *m_acqService;
    SerialConfigPage *m_commConfigPage;
    MonitorWidget *m_monitorWidget;
    HistoryWidget * m_historyWidget;
    QLabel *m_connStatusLabel;
    QLabel *m_receivedDataByte;
    QLabel *m_parsedErrorByteLabel;
};



#endif // MAINWINDOW_H
