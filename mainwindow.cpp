#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "device.h"
#include "stats.h"
#include "qdeadlinetimer.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSerialPort>
#include <QStandardItemModel>
#include <QTimer>
#include <QDeadlineTimer>
#include <QTime>
#include <QBrush>
#include <QUrl>
#include <QSslConfiguration>
#include <QSettings>
#include <QtGui>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);
    setWindowTitle("Autotests");
    manager = new QNetworkAccessManager(this);
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);



    QIntValidator *validator = new QIntValidator(1, 100, this);
    QIntValidator *timeValidator = new QIntValidator(1, 1000, this);
    ui->slotCount->setValidator(validator);
    ui->timeEdit->setValidator(timeValidator);
    ui->StartButton->setEnabled(false);
    ui->StopButton->setEnabled(false);
    ui->progress_bar->setValue(0);

    ui->deviceTable->setColumnCount(10);
    ui->deviceTable->setRowCount(10);
    for(int i = 0; i < 10; i++)
    {
        ui->deviceTable->setColumnWidth(i,80);
        ui->deviceTable->setRowHeight(i,48);
    }
    ui->deviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QSettings settings("C:/Users/Test/Documents/autotests/settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    slotCount = settings.value ("Devices_count", sett_count).toInt();
    left_time = settings.value("Time_to_test", sett_time).toInt();
    settings.endGroup();

    ui->slotCount->setText(QString::number(slotCount));
    ui->timeEdit->setText(QString::number(left_time));

    json_tmr = new QTimer(this);
    json_tmr->setInterval(1000);



    connect(json_tmr, SIGNAL(timeout()),this, SLOT(get_json()));

    tmr = new QTimer(this);
    tmr->setInterval(1000);

    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));
}

MainWindow::~MainWindow()
{


    delete ui;
    delete tmr;
    delete json_tmr;
}

void MainWindow::test_request()
{
    int x = 0;
    int y = 0;
    for(int i = 0; i < slotCount; i++)
    {
        QString dev_show = dev[i].ip + "\n" + dev[i].device_status;
        ui->deviceTable->setItem(x,y, new QTableWidgetItem(dev_show));


        dev[i].start_testing();
        if(x == 9){
            y++;
            x = 0;
        }else{
            x++;
        }
    }
}

void MainWindow::stop_request()
{
    for(int i = 0; i < slotCount; i++)
    {
        dev[i].stop_testing();
    }
}

void MainWindow::get_json()
{
    int x = 0;
    int y = 0;
    set_status();
    for(int i = 0; i < slotCount; i++)
    {
        if(dev[i].connected == true)
        {
            dev[i].get_result();
            //dev[i].device_status = "Testing...";
            QString dev_show = dev[i].ip + "\n" + dev[i].device_status;
            ui->deviceTable->setItem(x,y, new QTableWidgetItem(dev_show));
            if(dev[i].device_status == "Error"){
                ui->deviceTable->item(x,y)->setBackground(Qt::red);
            }else
                if(dev[i].device_status == "Warning"){
                    ui->deviceTable->item(x,y)->setBackground(Qt::yellow);
                }else
                    if(dev[i].device_status == "Good"){
                        ui->deviceTable->item(x,y)->setBackground(Qt::darkGreen);
                    }
        }else{
            ui->deviceTable->item(x,y)->setText("");
        }
        if(x == 9){
            y++;
            x = 0;
        }else{
            x++;
        }

    }

}

void MainWindow::get_average_results()
{
    for(int i = 0; i < slotCount; i++)
    {
        average_flash_read_good += dev[i].flash_read_good;
        average_flash_write_good += dev[i].flash_write_good;
        average_adc_1_2_read_good += dev[i].adc_1_2_read_good;
        average_adc_1_5_read_good += dev[i].adc_1_5_read_good;
        average_adc_2_5_read_good += dev[i].adc_2_5_read_good;
        average_gpio_sensor1_good += dev[i].gpio_sensor1_good;
        average_gpio_sensor2_good += dev[i].gpio_sensor2_good;
        average_eth_rx += dev[i].eth_rx;
        average_i2c_read_good += dev[i].i2c_read_good;
        average_smi_read_good += dev[i].smi_read_good;
        average_spi_read_good += dev[i].spi_read_good;
    }

    average_flash_read_good = average_flash_read_good / slotCount;
    average_flash_write_good = average_flash_write_good / slotCount;
    average_adc_1_2_read_good = average_adc_1_2_read_good / slotCount;
    average_adc_1_5_read_good = average_adc_1_5_read_good / slotCount;
    average_adc_2_5_read_good = average_adc_2_5_read_good / slotCount;
    average_gpio_sensor1_good = average_gpio_sensor1_good / slotCount;
    average_gpio_sensor2_good = average_gpio_sensor2_good / slotCount;
    average_eth_rx = average_eth_rx / slotCount;
    average_i2c_read_good = average_i2c_read_good / slotCount;
    average_smi_read_good = average_smi_read_good / slotCount;
    average_spi_read_good = average_spi_read_good / slotCount;
}



void MainWindow::set_status()
{
    for(int i = 0; i < slotCount; i++)
    {   if(dev[i].connected)
        {
            if(dev[i].flash_read_bad > 10 ||
                    dev[i].flash_write_bad > 10 ||
                    dev[i].adc_1_2_read_bad > 10 ||
                    dev[i].adc_1_5_read_bad > 10 ||
                    dev[i].adc_2_5_read_bad > 10 ||
                    dev[i].gpio_sensor1_bad > 10 ||
                    dev[i].gpio_sensor2_bad > 10 ||
                    dev[i].i2c_read_bad > 10 ||
                    dev[i].smi_read_bad > 10 ||
                    dev[i].spi_read_bad > 10)
            {
                dev[i].device_status = "Error";
            }else{
                if((dev[i].flash_read_bad > 1 ||
                    dev[i].flash_write_bad > 1 ||
                    dev[i].adc_1_2_read_bad > 1 ||
                    dev[i].adc_1_5_read_bad > 1 ||
                    dev[i].adc_2_5_read_bad > 1 ||
                    dev[i].gpio_sensor1_bad > 1 ||
                    dev[i].gpio_sensor2_bad > 1 ||
                    dev[i].i2c_read_bad > 1 ||
                    dev[i].smi_read_bad > 1 ||
                    dev[i].spi_read_bad > 1)&&dev[i].device_status != "Good")
                {
                    dev[i].device_status = "Warning";
                }else{
                    if(dev[i].device_status != "Error" && dev[i].device_status != "Warning")
                        dev[i].device_status = "Good";
                }
            }

            if((average_flash_read_good - dev[i].flash_read_good) / average_flash_read_good > 0.3 ||
                    (average_flash_write_good - dev[i].flash_write_good) / average_flash_write_good > 0.3 ||
                    (average_adc_1_2_read_good - dev[i].adc_1_2_read_good) / average_adc_1_2_read_good > 0.3 ||
                    (average_adc_1_5_read_good - dev[i].adc_1_5_read_good) / average_adc_1_5_read_good > 0.3 ||
                    (average_adc_2_5_read_good - dev[i].adc_2_5_read_good) / average_adc_2_5_read_good > 0.3 ||
                    (average_gpio_sensor1_good - dev[i].gpio_sensor1_good) / average_gpio_sensor1_good > 0.3 ||
                    (average_gpio_sensor2_good - dev[i].gpio_sensor2_good) / average_gpio_sensor2_good > 0.3 ||
                    (average_eth_rx - dev[i].eth_rx) / average_eth_rx > 0.3 ||
                    (average_i2c_read_good - dev[i].i2c_read_good) / average_i2c_read_good > 0.3 ||
                    (average_smi_read_good - dev[i].smi_read_good) / average_smi_read_good > 0.3 ||
                    (average_spi_read_good - dev[i].spi_read_good) / average_spi_read_good > 0.3)
            {
                dev[i].device_status = "Error";
            }else{
                if((average_flash_read_good - dev[i].flash_read_good) / average_flash_read_good > 0.1 ||
                        (average_flash_write_good - dev[i].flash_write_good) / average_flash_write_good > 0.1 ||
                        (average_adc_1_2_read_good - dev[i].adc_1_2_read_good) / average_adc_1_2_read_good > 0.1 ||
                        (average_adc_1_5_read_good - dev[i].adc_1_5_read_good) / average_adc_1_5_read_good > 0.1 ||
                        (average_adc_2_5_read_good - dev[i].adc_2_5_read_good) / average_adc_2_5_read_good > 0.1 ||
                        (average_gpio_sensor1_good - dev[i].gpio_sensor1_good) / average_gpio_sensor1_good > 0.1 ||
                        (average_gpio_sensor2_good - dev[i].gpio_sensor2_good) / average_gpio_sensor2_good > 0.1 ||
                        (average_eth_rx - dev[i].eth_rx) / average_eth_rx > 0.1 ||
                        (average_i2c_read_good - dev[i].i2c_read_good) / average_i2c_read_good > 0.1 ||
                        (average_smi_read_good - dev[i].smi_read_good) / average_smi_read_good > 0.1 ||
                        (average_spi_read_good - dev[i].spi_read_good) / average_spi_read_good > 0.1)
                {
                    if(dev[i].device_status != "Error"){
                        dev[i].device_status = "Warning";
                    }else{
                        if(dev[i].device_status != "Warning" && dev[i].device_status != "Error"){
                            dev[i].device_status = "Good";
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < slotCount; i++)
    {
        if(dev[i].flash_read_bad > dev[i].current_flash_read_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("flash_read_error on %1").arg(dev[i].ip) + "\n" + QString("%1 total flash read errors:%2").arg(dev[i].ip).arg(dev[i].flash_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_flash_read_bad = dev[i].flash_read_bad;        }

        if(dev[i].flash_write_bad > dev[i].current_flash_write_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("flash_write_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total flash write errors:%2").arg(dev[i].ip).arg(dev[i].flash_write_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_flash_write_bad = dev[i].flash_write_bad;        }

        if(dev[i].adc_1_2_read_bad > dev[i].current_adc_1_2_read_bad){
            QString item =  QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("adc_1_2_read_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total adc_1_2_read errors:%2").arg(dev[i].ip).arg(dev[i].adc_1_2_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_adc_1_2_read_bad = dev[i].adc_1_2_read_bad;        }

        if(dev[i].adc_1_5_read_bad > dev[i].current_adc_1_5_read_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("adc_1_5_read_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total adc_1_5_read errors:%2").arg(dev[i].ip).arg(dev[i].adc_1_5_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_adc_1_5_read_bad = dev[i].adc_1_5_read_bad;        }

        if(dev[i].adc_2_5_read_bad > dev[i].current_adc_2_5_read_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("adc_2_5_read_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total adc_2_5_read errors:%2").arg(dev[i].ip).arg(dev[i].adc_2_5_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_adc_2_5_read_bad = dev[i].adc_2_5_read_bad;        }

        if(dev[i].gpio_sensor1_bad > dev[i].current_gpio_sensor1_bad){
            QString item =  QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("gpio_sensor1_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total gpio_sensor1 errors:%2").arg(dev[i].ip).arg(dev[i].gpio_sensor1_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_gpio_sensor1_bad = dev[i].gpio_sensor1_bad;        }

        if(dev[i].gpio_sensor2_bad > dev[i].current_gpio_sensor2_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("gpio_sensor2_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total gpio_sensor2 errors:%2").arg(dev[i].ip).arg(dev[i].gpio_sensor2_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_gpio_sensor2_bad = dev[i].gpio_sensor2_bad;        }

        if(dev[i].i2c_read_bad > dev[i].current_i2c_read_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("i2c_read_eror on %1").arg(dev[i].ip) + "\n" + QString("%1 total i2c read errors:%2").arg(dev[i].ip).arg(dev[i].i2c_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_i2c_read_bad = dev[i].i2c_read_bad;        }

        if(dev[i].smi_read_bad > dev[i].current_smi_read_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("smi_read_error on %1").arg(dev[i].ip) + "\n"+ QString("%1 total smi read errors:%2").arg(dev[i].ip).arg(dev[i].smi_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_smi_read_bad = dev[i].smi_read_bad;
        }

        if(dev[i].spi_read_bad > dev[i].current_spi_read_bad){
            QString item = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "\n" +
                    QString("spi_read_bad on %1").arg(dev[i].ip) + "\n"+ QString("%1 total spi read errors:%2").arg(dev[i].ip).arg(dev[i].spi_read_bad) + "\n";
            ui->log->addItem(item);
            dev[i].current_spi_read_bad = dev[i].spi_read_bad;
        }
    }
}

//unsigned long current_flash_read_bad = 0;
//unsigned long current_flash_write_bad = 0;
//unsigned long current_adc_1_2_read_bad = 0;
//unsigned long current_adc_1_5_read_bad = 0;
//unsigned long current_adc_2_5_read_bad = 0;
//unsigned long current_gpio_sensor1_bad = 0;
//unsigned long current_gpio_sensor2_bad = 0;
//unsigned long current_i2c_read_bad = 0;
//unsigned long current_smi_read_bad = 0;
//unsigned long current_spi_read_bad = 0;


void MainWindow::on_StopButton_clicked()
{
    ui->StartButton->setEnabled(true);
    ui->StopButton->setEnabled(false);
    ui->slotCount->setEnabled(true);
    ui->timeEdit->setEnabled(true);
    ui->requestTime->setEnabled(true);
    json_tmr->stop();
    tmr->stop();
    ui->SpentTimer->setText("00:00:00");
    ui->LeftTimer->setText("00:00:00");
    sec = 0;
    min = 0;
    hour = 0;

    for(int i = 0; i < slotCount; i++)
    {
        stop_request();
    }

    save_to_file();

    QSettings testlogs("C:/Users/Test/Documents/autotests/testlogs/testlogs.ini", QSettings::IniFormat);
    for(int i = 0; i < slotCount; i++)
    {
        QString ip_name = dev[i].ip;
        testlogs.beginGroup(ip_name);

        int param1 = dev[i].flash_read_good;
        int param2 = dev[i].flash_read_bad;
        int param3 = dev[i].flash_write_good;
        int param4 = dev[i].flash_write_bad;
        int param5 = dev[i].adc_1_2_read_good;
        int param6 = dev[i].adc_1_2_read_bad;
        int param7 = dev[i].adc_1_5_read_good;
        int param8 = dev[i].adc_1_5_read_bad;
        int param9 = dev[i].adc_2_5_read_good;
        int param10 = dev[i].adc_2_5_read_bad;
        int param11 = dev[i].gpio_sensor1_good;
        int param12= dev[i].gpio_sensor1_bad;
        int param13= dev[i].gpio_sensor2_good;
        int param14= dev[i].gpio_sensor2_bad;
        int param15= dev[i].timer_uptime;
        int param16= dev[i].timer_tim6;
        int param17= dev[i].eth_tx;
        int param18 = dev[i].eth_rx;
        int param19 = dev[i].eth_rx_bad;
        int param20 = dev[i].i2c_read_good;
        int param21 = dev[i].i2c_read_bad;
        int param22 = dev[i].smi_read_good;
        int param23 = dev[i].smi_read_bad;
        int param24 = dev[i].spi_read_good;
        int param25 = dev[i].spi_read_bad;

        testlogs.setValue("flash_read_good",param1);
        testlogs.setValue("flash_read_bad",param2);
        testlogs.setValue("flash_write_good",param3);
        testlogs.setValue("flash_write_bad",param4);
        testlogs.setValue("adc_1_2_read_good",param5);
        testlogs.setValue("adc_1_2_read_bad",param6);
        testlogs.setValue("adc_1_5_read_good",param7);
        testlogs.setValue("adc_1_5_read_bad",param8);
        testlogs.setValue("adc_2_5_read_good",param9);
        testlogs.setValue("adc_2_5_read_bad",param10);
        testlogs.setValue("gpio_sensor1_good",param11);
        testlogs.setValue("gpio_sensor1_bad",param12);
        testlogs.setValue("gpio_sensor2_good",param13);
        testlogs.setValue("gpio_sensor2_bad",param14);
        testlogs.setValue("timer_uptime",param15);
        testlogs.setValue("timer_tim6",param16);
        testlogs.setValue("eth_tx",param17);
        testlogs.setValue("eth_rx",param18);
        testlogs.setValue("eth_rx_bad",param19);
        testlogs.setValue("i2c_read_good",param20);
        testlogs.setValue("i2c_read_bad",param21);
        testlogs.setValue("smi_read_good",param22);
        testlogs.setValue("smi_read_bad",param23);
        testlogs.setValue("spi_read_good",param24);
        testlogs.setValue("spi_read_bad",param25);

        testlogs.endGroup();
    }

    ui->progress_bar->setValue(0);
    QSettings errorlogs("C:/Users/Test/Documents/autotests/testlogs/errorlogs.ini", QSettings::IniFormat);
    errorlogs.beginGroup("ErrorList");
    for(int i = 0; i < ui->log->count(); i++)
    {
        ui->log->setCurrentRow(i);
        QString error_string = QString(ui->log->currentRow());
        errorlogs.setValue("",error_string);
    }
}

void MainWindow::save_to_file()
{
    QFile file("C:/Users/Test/Documents/autotests/testlogs/errorlogs.txt");
    if (!file.open(QFile::WriteOnly | QIODevice::Text))
    {
        return;
    }

    for(int row = 0; row < ui->log->count(); row++)
    {
        QListWidgetItem *item = ui->log->item(row);
        QTextStream out(&file);
        out << item->text() << "\n";
    }

    file.close();
}

void MainWindow::on_StartButton_clicked()
{
    QSettings settings("C:/Users/Test/Documents/autotests/settings.ini",QSettings::IniFormat);
    settings.beginGroup("Settings");
    sett_req_time = request_time;
    sett_time = left_time;
    sett_count = slotCount;
    settings.setValue( "Devices_count", sett_count);
    settings.setValue("Time_to_test",sett_time);
    settings.endGroup();

    ui->StopButton->setEnabled(true);
    ui->StartButton->setEnabled(false);
    ui->slotCount->setEnabled(false);
    ui->timeEdit->setEnabled(false);
    ui->requestTime->setEnabled(false);
    ui->deviceTable->clear();
    ui->log->clear();
    slotCount = ui->slotCount->text().toInt();

    ui->progress_bar->setMinimum(0);
    ui->progress_bar->setMaximum(left_time *60 *60);

    time_till_end = left_time - 1;
    remaining_time = new QTime(0,0,0);


    current_time = new QTime(0,0,0);



    dev = new device[slotCount];

    for(int i = 0; i < slotCount; i++)
    {
        dev[i].set_device_ip(QString("192.168.0.%1").arg(i + 1));
    }

    test_request();

    json_tmr->start();
    tmr->start();

}

void MainWindow::updateTime()
{
    if(current_time->hour() == 1)
    {
        current_time->restart();
        remaining_time->restart();
        passed_time++;
        time_till_end--;
    }
    if(passed_time == left_time){
        on_StopButton_clicked();
        QMessageBox::about(this,"Autotests", "Тестирование завершено.");
    }
    *current_time = current_time->addSecs(1);
    ui->SpentTimer->setText(QString("%1:%2:").arg(passed_time / 24).arg(passed_time % 24) + (current_time->toString("mm:ss")));



    *remaining_time = remaining_time->addSecs(-1);

    ui->LeftTimer->setText(QString("%1:%2:").arg(time_till_end / 24).arg(time_till_end % 24) + (remaining_time->toString("mm:ss")));

    if( ui->progress_bar->value() >= ui->progress_bar->maximum() ){
        ui->progress_bar->reset();
    }
    ui->progress_bar->setValue(ui->progress_bar->value()+1);
}

void MainWindow::on_timeEdit_editingFinished()
{
    left_time = ui->timeEdit->text().toInt();


    time_ready = true;
    start_ready_check();
}


int MainWindow::on_slotCount_textChanged(const QString &arg1)
{
    slotCount = ui->slotCount->text().toInt();

    count_ready = true;
    start_ready_check();

    return slotCount;
}

void MainWindow::on_requestTime_currentIndexChanged(int index)
{
    request_time = ui->requestTime->currentText().toInt();
    json_tmr->setInterval(request_time * 1000);

    request_time_ready = true;
    start_ready_check();
}

void MainWindow::start_ready_check()
{
    if(!(slotCount == 0) && !(left_time == 0) && request_time_ready){
        ui->StartButton->setEnabled(true);
    }
}



void MainWindow::on_deviceTable_cellDoubleClicked(int row, int column)
{
    if((column * 10 + row) < slotCount)
    {
        stats_window.setWindowTitle(QString("%1 stats").arg(dev[row + column * 10].ip));
        stats_window.flash_read_good = dev[column * 10 + row].flash_read_good;
        stats_window.flash_read_bad = dev[column * 10 + row].flash_read_bad;
        stats_window.flash_write_good = dev[column * 10 + row].flash_write_good;
        stats_window.flash_write_bad = dev[column * 10 + row].flash_write_bad;
        stats_window.adc_1_2_read_good = dev[column * 10 + row].adc_1_2_read_good;
        stats_window.adc_1_2_read_bad = dev[column * 10 + row].adc_1_2_read_bad;
        stats_window.adc_1_5_read_good = dev[column * 10 + row].adc_1_5_read_good;
        stats_window.adc_1_5_read_bad = dev[column * 10 + row].adc_1_5_read_bad;
        stats_window.adc_2_5_read_good = dev[column * 10 + row].adc_2_5_read_good;
        stats_window.adc_2_5_read_bad = dev[column * 10 + row].adc_2_5_read_bad;
        stats_window.gpio_sensor1_good = dev[column * 10 + row].gpio_sensor1_good;
        stats_window.gpio_sensor1_bad = dev[column * 10 + row].gpio_sensor1_bad;
        stats_window.gpio_sensor2_good = dev[column * 10 + row].gpio_sensor2_good;
        stats_window.gpio_sensor2_bad = dev[column * 10 + row].gpio_sensor2_bad;
        stats_window.timer_uptime = dev[column * 10 + row].timer_uptime;
        stats_window.timer_tim6 = dev[column * 10 + row].timer_tim6;
        stats_window.eth_tx = dev[column * 10 + row].eth_tx;
        stats_window.eth_rx = dev[column * 10 + row].eth_rx;
        stats_window.eth_rx_bad = dev[column * 10 + row].eth_rx_bad;
        stats_window.i2c_read_good = dev[column * 10 + row].i2c_read_good;
        stats_window.i2c_read_bad = dev[column * 10 + row].i2c_read_bad;
        stats_window.smi_read_good = dev[column * 10 + row].smi_read_good;
        stats_window.i2c_read_bad = dev[column * 10 + row].i2c_read_bad;
        stats_window.spi_read_good = dev[column * 10 + row].spi_read_good;
        stats_window.spi_read_bad = dev[column * 10 + row].spi_read_bad;

        stats_window.set_stats();
        stats_window.show();
    }
}




