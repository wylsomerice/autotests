#include "device.h"
#include <QModbusDevice>
#include <QModbusClient>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QUrl>
#include <QModbusTcpServer>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

//#include "mainwindow.h"

device::device()
{
    manager = new QNetworkAccessManager(this);
    jsonmanager = new QNetworkAccessManager(this);
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    jsonmanager->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

void device::NetworkSuccess()
{
    //qDebug()<<networkreply->readAll() + QString(this->ip);
    if(networkreply->readAll() == "Ok")
    {
        qDebug()<<"Success";
        this->connected = true;
    }
}

void device::NetworkError(QNetworkReply::NetworkError error)
{

        qDebug()<<"NetworkError";
        this->connected = false;

}

//запрос начала тестирования
void device::start_testing()
{
    networkreply =  manager->get(QNetworkRequest(QUrl(QString("http://%1/api/selftest?start=1").arg(this->ip))));
    connect(networkreply,SIGNAL(finished()),SLOT(NetworkSuccess()));
    connect(networkreply,SIGNAL(error(QNetworkReply::NetworkError)), SLOT(NetworkError(QNetworkReply::NetworkError)));

}

//запрос остановки тестирования
void device::stop_testing()
{
    networkreply =  manager->get(QNetworkRequest(QUrl(QString("http://%1/api/selftest?start=0").arg(this->ip))));
    connect(networkreply,SIGNAL(finished()),SLOT(NetworkSuccess()));
    connect(networkreply,SIGNAL(error(QNetworkReply::NetworkError)), SLOT(NetworkError(QNetworkReply::NetworkError)));
}
//запрос json файла
void device::get_result()
{
    //request_json(QUrl(QString("http://%1/api/selftestResult.json").arg(this->ip)));
    jsonreply =  jsonmanager->get(QNetworkRequest(QUrl(QString("http://%1/api/selftestResult.json").arg(this->ip))));
    connect(jsonreply, &QNetworkReply::finished, this, &device::parse_json);
}

void device::parse_json(/*QNetworkReply *jsonreply*/)
{
    QString str = jsonreply->readAll();
    qDebug()<<str;
    if(!jsonreply->error()){
        QJsonParseError  parseError;
        QJsonDocument document = QJsonDocument::fromJson( str.toLocal8Bit().data()/*jsonreply->readAll()"{\"test\":123}"*/, &parseError);
        if(parseError.error == QJsonParseError::NoError)
        {

            if(document.isObject())
            {
                qDebug()<<"Ошибок нет";
            }
        }
        else{
            qDebug() << parseError.errorString();
            qDebug() << parseError.offset;
            this->device_status = "Error";
            this->connected = false;
        }
        QJsonObject root = document.object();

        //           unsigned long flash_read_good = 1;   //Число успешных чтений Flash памяти
        //           unsigned long flash_read_bad;       //Число ошибок чтения Flash памяти
        //           unsigned long flash_write_good;     //Число успешных записей Flash памяти
        //           unsigned long flash_write_bad;      //Число ошибок записи Flash памяти
        //           unsigned long adc_1_2_read_good;    //Число успешного считывания с АЦП (1.2В)
        //           unsigned long adc_1_2_read_bad;     //Число ошибок считывания с АЦП (1.2В)
        //           unsigned long adc_1_5_read_good;    //Число успешного считывания с АЦП (1.5В)
        //           unsigned long adc_1_5_read_bad;     //Число ошибок считывания с АЦП (1.5В)
        //           unsigned long adc_2_5_read_good;    //Число успешного считывания с АЦП (2.5В)
        //           unsigned long adc_2_5_read_bad;     //Число ошибок считывания с АЦП (2.5В)
        //           unsigned long gpio_sensor_1_good;   //Число успешного считывания GPIO (Sensor 1)
        //           unsigned long gpio_sensor_1_bad;    //Число ошибок считывания GPIO (Sensor 1)
        //           unsigned long gpio_sensor_2_good;   //Число успешного считывания GPIO (Sensor 2)
        //           unsigned long gpio_sensor_2_bad;    //Число ошибок считывания GPIO (Sensor 1)
        //           unsigned long timer_uptime;         //Значения таймера (RTC)
        //           unsigned long timer_tim6;           //Значения таймера (TIM6)
        //           unsigned long eth_tx;               //Число принятых данных через Ethernet
        //           unsigned long eth_rx;               //Число отправленных данных через Ethernet
        //           unsigned long eth_rx_bad;           //Число ошибочно принятых данных через Ethernet
        //           unsigned long i2c_read_good;        //Число успешного чтения I2C
        //           unsigned long i2c_read_bad;         //Число ошибок чтения I2C
        //           unsigned long smi_read_good;        //Число успешного чтения SMI
        //           unsigned long smi_read_bad;         //Число ошибок чтения SMI
        //           unsigned long spi_read_good;        //Число успешного чтения SPI
        //           unsigned long spi_read_bad;         //Число ошибок чтения SPI

        this->flash_read_good = root.value("flash_read_good").toInt();
        this->flash_read_bad = root.value("flash_read_bad").toInt();
        this->flash_write_good = root.value("flash_write_good").toInt();
        this->flash_write_bad = root.value("flash_write_bad").toInt();
        this->adc_1_2_read_good = root.value("adc_1_2_read_good").toInt();
        this->adc_1_2_read_bad = root.value("adc_1_2_read_bad").toInt();
        this->adc_1_5_read_good = root.value("adc_1_5_read_good").toInt();
        this->adc_1_5_read_bad = root.value("adc_1_5_read_bad").toInt();
        this->adc_2_5_read_good = root.value("adc_2_5_read_good").toInt();
        this->adc_2_5_read_bad = root.value("adc_2_5_read_bad").toInt();
        this->gpio_sensor1_good = root.value("gpio_sensor1_good").toInt();
        this->gpio_sensor1_bad = root.value("gpio_sensor1_bad").toInt();
        this->gpio_sensor2_good = root.value("gpio_sensor2_good").toInt();
        this->gpio_sensor2_bad = root.value("gpio_sensor2_bad").toInt();
        this->timer_uptime = root.value("timer_uptime").toInt();
        this->timer_tim6 = root.value("timer_tim6").toInt();
        this->eth_tx = root.value("eth_tx").toInt();
        this->eth_rx = root.value("eth_rx").toInt();
        this->eth_rx_bad = root.value("eth_rx_bad").toInt();
        this->i2c_read_good = root.value("i2c_read_good").toInt();
        this->i2c_read_bad = root.value("i2c_read_bad").toInt();
        this->smi_read_good = root.value("smi_read_good").toInt();
        this->smi_read_bad = root.value("smi_read_bad").toInt();
        this->spi_read_good = root.value("spi_read_good").toInt();
        this->spi_read_bad = root.value("spi_read_bad").toInt();

        qDebug()<<QString("flash_read_good: %1").arg(this->flash_read_good);

    }
    jsonreply->deleteLater();
}

device::~device()
{

}



