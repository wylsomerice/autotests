#include "stats.h"
#include "ui_stats.h"
#include "mainwindow.h"

stats::stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stats)
{
    ui->setupUi(this);
}

stats::~stats()
{
    ui->deviceStats->clear();
    delete ui;
}

void stats::set_stats()
{
    ui->deviceStats->clear();
    QString flash_read_good = QString("flash_read_good: %1").arg(this->flash_read_good);      //Число успешных чтений Flash памяти
    QString flash_read_bad = QString("flash_read_bad: %1").arg(this->flash_read_bad);
    QString flash_write_good = QString("flash_write_good: %1").arg(this->flash_write_good);
    QString flash_write_bad = QString("flash_write_bad: %1").arg(this->flash_write_bad);
    QString adc_1_2_read_good = QString("adc_1_2_read_good: %1").arg(this->adc_1_2_read_good);
    QString adc_1_2_read_bad = QString("adc_1_2_read_bad: %1").arg(this->adc_1_2_read_bad);
    QString adc_1_5_read_good = QString("adc_1_5_read_good: %1").arg(this->adc_1_5_read_good);
    QString adc_1_5_read_bad = QString("adc_1_5_read_bad: %1").arg(this->adc_1_5_read_bad);
    QString adc_2_5_read_good = QString("adc_2_5_read_good: %1").arg(this->adc_2_5_read_good);
    QString adc_2_5_read_bad = QString("adc_2_5_read_bad: %1").arg(this->adc_2_5_read_bad);
    QString gpio_sensor1_good = QString("gpio_sensor1_good: %1").arg(this->gpio_sensor1_good);
    QString gpio_sensor1_bad = QString("gpio_sensor1_bad: %1").arg(this->gpio_sensor1_bad);
    QString gpio_sensor2_good = QString("gpio_sensor2_good: %1").arg(this->gpio_sensor2_good);
    QString gpio_sensor2_bad = QString("gpio_sensor2_bad: %1").arg(this->gpio_sensor2_bad);
    QString timer_uptime = QString("timer_uptime: %1").arg(this->timer_uptime);
    QString timer_tim6 = QString("timer_tim6: %1").arg(this->timer_tim6);
    QString eth_tx = QString("eth_tx: %1").arg(this->eth_tx);
    QString eth_rx = QString("eth_rx: %1").arg(this->eth_rx);
    QString eth_rx_bad = QString("eth_rx_bad: %1").arg(this->eth_rx_bad);
    QString i2c_read_good = QString("i2c_read_good: %1").arg(this->i2c_read_good);
    QString i2c_read_bad = QString("i2c_read_bad: %1").arg(this->i2c_read_bad);
    QString smi_read_good = QString("smi_read_good: %1").arg(this->smi_read_good);
    QString smi_read_bad = QString("smi_read_bad: %1").arg(this->smi_read_bad);
    QString spi_read_good = QString("spi_read_good: %1").arg(this->spi_read_good);
    QString spi_read_bad = QString("spi_read_bad: %1").arg(this->spi_read_bad);

    ui->deviceStats->addItem(flash_read_good);
    ui->deviceStats->addItem(flash_read_bad);
    ui->deviceStats->addItem(flash_write_good);
    ui->deviceStats->addItem(flash_write_bad);
    ui->deviceStats->addItem(adc_1_2_read_good);
    ui->deviceStats->addItem(adc_1_2_read_bad);
    ui->deviceStats->addItem(adc_1_5_read_good);
    ui->deviceStats->addItem(adc_1_5_read_bad);
    ui->deviceStats->addItem(adc_2_5_read_good);
    ui->deviceStats->addItem(adc_2_5_read_bad);
    ui->deviceStats->addItem(gpio_sensor1_good);
    ui->deviceStats->addItem(gpio_sensor1_bad);
    ui->deviceStats->addItem(gpio_sensor2_good);
    ui->deviceStats->addItem(gpio_sensor2_bad);
    ui->deviceStats->addItem(timer_uptime);
    ui->deviceStats->addItem(timer_tim6);
    ui->deviceStats->addItem(eth_tx);
    ui->deviceStats->addItem(eth_rx);
    ui->deviceStats->addItem(eth_rx_bad);
    ui->deviceStats->addItem(i2c_read_good);
    ui->deviceStats->addItem(i2c_read_bad);
    ui->deviceStats->addItem(smi_read_good);
    ui->deviceStats->addItem(smi_read_bad);
    ui->deviceStats->addItem(spi_read_good);
    ui->deviceStats->addItem(spi_read_bad);
}


