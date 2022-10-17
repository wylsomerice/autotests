#ifndef STATS_H
#define STATS_H

#include <QWidget>

namespace Ui {
class stats;
}

class stats : public QWidget
{
    Q_OBJECT

public:
    explicit stats(QWidget *parent = nullptr);
    ~stats();

    unsigned long flash_read_good = 0;      //Число успешных чтений Flash памяти
    unsigned long flash_read_bad = 0;       //Число ошибок чтения Flash памяти
    unsigned long flash_write_good = 0;     //Число успешных записей Flash памяти
    unsigned long flash_write_bad = 0;      //Число ошибок записи Flash памяти
    unsigned long adc_1_2_read_good = 0;    //Число успешного считывания с АЦП (1.2В)
    unsigned long adc_1_2_read_bad = 0;     //Число ошибок считывания с АЦП (1.2В)
    unsigned long adc_1_5_read_good = 0;    //Число успешного считывания с АЦП (1.5В)
    unsigned long adc_1_5_read_bad = 0;     //Число ошибок считывания с АЦП (1.5В)
    unsigned long adc_2_5_read_good = 0;    //Число успешного считывания с АЦП (2.5В)
    unsigned long adc_2_5_read_bad = 0;     //Число ошибок считывания с АЦП (2.5В)
    unsigned long gpio_sensor1_good = 0;   //Число успешного считывания GPIO (Sensor 1)
    unsigned long gpio_sensor1_bad = 0;    //Число ошибок считывания GPIO (Sensor 1)
    unsigned long gpio_sensor2_good = 0;   //Число успешного считывания GPIO (Sensor 2)
    unsigned long gpio_sensor2_bad = 0;    //Число ошибок считывания GPIO (Sensor 1)
    unsigned long timer_uptime = 0;         //Значения таймера (RTC)
    unsigned long timer_tim6 = 0;           //Значения таймера (TIM6)
    unsigned long eth_tx = 0;               //Число принятых данных через Ethernet
    unsigned long eth_rx = 0;               //Число отправленных данных через Ethernet
    unsigned long eth_rx_bad = 0;           //Число ошибочно принятых данных через Ethernet
    unsigned long i2c_read_good = 0;        //Число успешного чтения I2C
    unsigned long i2c_read_bad = 0;         //Число ошибок чтения I2C
    unsigned long smi_read_good = 0;        //Число успешного чтения SMI
    unsigned long smi_read_bad = 0;         //Число ошибок чтения SMI
    unsigned long spi_read_good = 0;        //Число успешного чтения SPI
    unsigned long spi_read_bad = 0;         //Число ошибок чтения SPI

    void set_stats();
private slots:


private:
    Ui::stats *ui;

};

#endif // STATS_H
