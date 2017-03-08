#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "qextserialenumerator.h"
#include "qextserialport.h"
#include "qextserialport_global.h"

#include <QMessageBox>
#include <QTextStream>

//счетчик нажатия кнопки закрытия
bool id=true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Dadc"<<"Nchan"<<"Uin"<<"Tcod"<<"Tgrad");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   id=true;
  //переменная - сигнал для начала передачи данных микроконтроллера
   char na4alo;
   na4alo=13;

 //Формулы для АЦП
   double UrefH=ui->lineEdit->text().toDouble();
   double UrefL=ui->lineEdit_2->text().toDouble();
   double Umid=ui->lineEdit_3->text().toDouble();

 //Номер канала
   short int chan=0;

 //Старший и младший байты регистра данных АЦП
   unsigned char dataLow=0;
   unsigned char dataHi=0;

 //Полные данные решистра данных АЦП
    short unsigned int LoHi=0;
    short signed int Result=0;

 //Переменная для посылки в УАРТ
   char posilka;
   posilka=13;

 // Инициализация порта УАРТ
  /*  QString portName = "com8";
    QextSerialPort* com = new QextSerialPort(portName, QextSerialPort::Polling );
    com->setBaudRate(BAUD9600); //(BaudRateType)(int)ddsCom("baudRate"));
    com->setFlowControl(FLOW_OFF);
    com->setParity(PAR_NONE); //(ParityType)(int)ddsCom("parity"));
    com->setDataBits(DATA_8);
    com->setStopBits(STOP_1);
    // timeout means total absolute timeout in ms
    com->setTimeout(1000);  */

//while (id)
    //{
    // Инициализация порта УАРТ
       QString portName = "com8";
       QextSerialPort* com = new QextSerialPort(portName, QextSerialPort::Polling );
       com->setBaudRate(BAUD9600); //(BaudRateType)(int)ddsCom("baudRate"));
       com->setFlowControl(FLOW_OFF);
       com->setParity(PAR_NONE); //(ParityType)(int)ddsCom("parity"));
       com->setDataBits(DATA_8);
       com->setStopBits(STOP_1);
       // timeout means total absolute timeout in ms
       com->setTimeout(15000);

        //если порт не был открыт
        if (! (com->open(QIODevice::ReadWrite | QIODevice::Unbuffered)))
            {
                delete com;
                com = 0;
                QMessageBox::information(this,"info","Can not open UART port ");
                id=false;
            }
        //если открыть порт удалось
        else
            {
                com->write(&na4alo);
                Sleep(15000);
                //если данные для чтения доступны
                int avail = com->bytesAvailable();
                if( avail > 0 )
                {
                    QByteArray usbdata;
                    usbdata.resize(avail);
                    //запись в массив усбдата данных с уарта
                    int read = com->read(usbdata.data(), usbdata.size());
                    if( read > 0 )
                    {

                        char konez;
                        konez=228;
                        com->write(&konez);
                        com->close();
                        for (int i=1;i<usbdata.size()-1;i+=3)
                        {
                            //разделение полученных байт
                                chan=usbdata[i-1];
                                dataLow=usbdata[i];
                                dataHi=usbdata[i+1];
                                LoHi=dataHi;
                                LoHi=LoHi<<8;
                                LoHi=LoHi|dataLow;
                                Result=LoHi;
                            //запись в первую колонку таблицы значения данных регистра
                                QTableWidgetItem *adc = new QTableWidgetItem;
                                adc->setText(QString::number(Result));
                                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,adc);
                            //запись во вторую колонку номера канала измерения
                                QTableWidgetItem *NChanel=new QTableWidgetItem;
                                NChanel->setText(QString::number(chan));
                                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,NChanel);
                            //добавление новой строки
                                ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

                        }//цикл фор заполнения первой колонки таблицы (анализа массива данных)
                            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()-1);

                        for (int i=0;i<ui->tableWidget->rowCount();i++)
                        {
                          QTableWidgetItem *read=ui->tableWidget->item(i,0);
                          int Dadc=read->text().toInt();
                            //QMessageBox::information(this,"info",QString::number(UrefH));
                            // QMessageBox::information(this,"info",QString::number(UrefL));
                            // QMessageBox::information(this,"info",QString::number(Umid));
                            // QMessageBox::information(this,"info",QString::number(Dadc));
                          double Uin=(0.8/8192)*Dadc*(UrefH-UrefL)+Umid;
                          double Tcod=(424*Uin-649);
                          double T=0.2*Tcod-10;
                            //добавление в третью колонку значения напряжения в вольтах
                                QTableWidgetItem *itm1=new QTableWidgetItem;
                                itm1->setText(QString::number(Uin));
                                ui->tableWidget->setItem(i,2,itm1);
                            //добавление в четвертую колонку значения Ткод
                                QTableWidgetItem *itm2=new QTableWidgetItem;
                                itm2->setText(QString::number(Tcod));
                                ui->tableWidget->setItem(i,3,itm2);
                            //добавление в пятую колонку значени температуры к градусах
                                QTableWidgetItem *itm3=new QTableWidgetItem;
                                itm3->setText(QString::number(T));
                                ui->tableWidget->setItem(i,4,itm3);

                           }//цикл фор заполнения таблицы

                        }//если функция рид выполнена успешно
                }//если данные доступны


                    //Sleep(20000);



            }//если порт был открыт

        delete com;
        //QMessageBox::information(this,"info","Vse Rabotaet Horosho");
        //for (int i=0;i<=100000;i++);



    //}//цикл вайл
//delete com;
}

/* for (int i=0;i<ui->tableWidget->rowCount();i++)
         {
           QTableWidgetItem *read=ui->tableWidget->item(i,0);
           int Dadc=read->text().toInt();
           //QMessageBox::information(this,"info",QString::number(UrefH));
          // QMessageBox::information(this,"info",QString::number(UrefL));
          // QMessageBox::information(this,"info",QString::number(Umid));
          // QMessageBox::information(this,"info",QString::number(Dadc));
           double Uin=(0.8/8192)*Dadc*(UrefH-UrefL)+Umid;
           double Tcod=(424*Uin-649);
            double T=0.2*Tcod-10;
          QTableWidgetItem *itm1=new QTableWidgetItem;
            itm1->setText(QString::number(Uin));
           ui->tableWidget->setItem(i,2,itm1);

         QTableWidgetItem *itm2=new QTableWidgetItem;
                  itm2->setText(QString::number(Tcod));
         ui->tableWidget->setItem(i,3,itm2);

         QTableWidgetItem *itm3=new QTableWidgetItem;
                  itm3->setText(QString::number(T));
         ui->tableWidget->setItem(i,4,itm3);
     */



     //QMessageBox::information(this,"info",QString::number(Umid));

    //for (int k=1;k<5;k++)
    //ui->tableWidget->setRowCount(k+1);

void MainWindow::on_pushButton_2_clicked()
{

/*short signed int Result=-228;
short int chan=0;
short int i=ui->tableWidget->rowCount()-1;
    //запись в первую колонку таблицы значения данных регистра
        QTableWidgetItem *adc = new QTableWidgetItem;
        adc->setText(QString::number(Result));
        ui->tableWidget->setItem(i,0,adc);
    //запись во вторую колонку номера канала измерения
        QTableWidgetItem *NChanel=new QTableWidgetItem;
        NChanel->setText(QString::number(chan));
        ui->tableWidget->setItem(i,1,NChanel);
    //добавление новой строки
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1); */
    id = false;
   // ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
}
