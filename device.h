#ifndef DEVICE_H
#define DEVICE_H
#include <QString>
#include <QThread>
#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QSignalMapper>
#include <QModbusTcpClient>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class device : public QObject
{
    Q_OBJECT

public:
    device();

    device(QString ip);

    ~device();

    QString ip;

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

    unsigned long current_flash_read_bad = 0;
    unsigned long current_flash_write_bad = 0;
    unsigned long current_adc_1_2_read_bad = 0;
    unsigned long current_adc_1_5_read_bad = 0;
    unsigned long current_adc_2_5_read_bad = 0;
    unsigned long current_gpio_sensor1_bad = 0;
    unsigned long current_gpio_sensor2_bad = 0;
    unsigned long current_i2c_read_bad = 0;
    unsigned long current_smi_read_bad = 0;
    unsigned long current_spi_read_bad = 0;

    QString device_status = "Connecting...";

    bool connected = false;

    void set_device_ip(QString ip_addr)
    {
        ip = ip_addr;
    }
    void get_json(QString ip);

    QNetworkAccessManager *manager;
    QNetworkAccessManager *jsonmanager;
    QNetworkReply *networkreply;
    QNetworkReply *jsonreply;
    QNetworkReply *get_json_finished;
private:


private slots:
    void NetworkError(QNetworkReply::NetworkError error);
    void NetworkSuccess();
    void parse_json(/*QNetworkReply *jsonreply*/);


public slots:
    void start_testing();
    void stop_testing();
    void get_result();

};

#endif // DEVICE_H
