#ifndef PAGE_SERIAL_CONFIG_H
#define PAGE_SERIAL_CONFIG_H

#include <QWidget>

class SerialConfig;
namespace Ui {
class TSerialConfig;
}

class SerialConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit SerialConfigPage(QWidget *parent = nullptr);
    ~SerialConfigPage();
private:
    int parityValue(int index);
private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::TSerialConfig *ui;
signals:
    void configSubmitted(const SerialConfig &config);
};

#endif // PAGE_SERIAL_CONFIG_H
