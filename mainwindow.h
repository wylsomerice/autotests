#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "device.h"
#include "stats.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QProgressBar>

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>
#include <QSettings>
#include <QTableWidget>
#include <QDeadlineTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    Ui::MainWindow *ui;

    ~MainWindow();


    void test_request();
    void stop_request();
    void start_ready_check();
    void get_average_results();
    void save_to_file();

    double average_flash_read_good = 0;      //Число успешных чтений Flash памяти
    double average_flash_write_good = 0;     //Число успешных записей Flash памяти
    double average_adc_1_2_read_good = 0;    //Число успешного считывания с АЦП (1.2В)
    double average_adc_1_5_read_good = 0;    //Число успешного считывания с АЦП (1.5В)
    double average_adc_2_5_read_good = 0;    //Число успешного считывания с АЦП (2.5В)
    double average_gpio_sensor1_good = 0;   //Число успешного считывания GPIO (Sensor 1)
    double average_gpio_sensor2_good = 0;   //Число успешного считывания GPIO (Sensor 2)
    double average_eth_rx = 0;               //Число отправленных данных через Ethernet
    double average_i2c_read_good = 0;        //Число успешного чтения I2C
    double average_smi_read_good = 0;        //Число успешного чтения SMI
    double average_spi_read_good = 0;        //Число успешного чтения SPI

signals:


public slots:


private slots:

    void on_StartButton_clicked();

    void on_StopButton_clicked();

    void updateTime();

    void on_timeEdit_editingFinished();

    int on_slotCount_textChanged(const QString &arg1);

    void on_requestTime_currentIndexChanged(int index);

    void get_json();

    void set_status();

    void on_deviceTable_cellDoubleClicked(int row, int column);

private:
    QSettings settings;

    stats stats_window;

    int sett_req_time;
    int sett_time;
    int sett_count;

    bool count_ready = false;
    bool time_ready = false;
    bool request_time_ready = false;

    int row_count = 10;
    int col_count = 10;

    QProgressBar progress_bar;
    QDeadlineTimer *general_timer;
    QTime* current_time;
    QTime* remaining_time;
    QTimer *tmr;
    int passed_time = 0;
    int time_till_end;

    int sec = 00;   //секунды int, обновляется по таймеру
    int min = 00;   //минуты int, обновляется через if
    int hour = 00;  //часы int, обновляется через if
    int left_time;
    int lmin;;
    int lhour;

    int slotCount;

    QTableWidgetItem *current_item;

    QNetworkAccessManager *manager;
    QNetworkReply *networkreply;

    QTimer *json_tmr;
    int request_time;

    device *dev;

};
#endif // MAINWINDOW_H
