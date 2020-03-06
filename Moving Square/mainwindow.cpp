#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QWindow>
//#include <QSerialPortInfo>
//#include <QtSerialPort>
#include <QAbstractSocket>
#include <m_struct.h>

#define TRUE_HEADER 0xAA
#define LEFT 0x0001
#define RIGHT 0x0002
#define UP 0x0003
#define DOWN 0x0004


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket=new QUdpSocket(this);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readSerial()));
}

MainWindow::~MainWindow()
{
    delete ui;


    socket->close();
}

void MainWindow::on_pushButton_clicked()
{

    /*serial->setPortName(ui->comboBox->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    bool a=serial->open(QIODevice::ReadWrite);

    qDebug()<<"sonuc:" + QString::number(a);*/

    ushort port=ui->label_6->text().toUShort();

    qDebug()<<port;
    socket->bind(port);


    bool a=socket->open(QIODevice::ReadOnly);
    qDebug()<<"bağlandı:"+ QString::number(a) ;

}

void MainWindow::readSerial()
{
    qDebug()<<"okundu";

    bool ok;

    //QByteArray ascii = serial->readAll();

    //while(socket->hasPendingDatagrams()){

        QByteArray ascii;

        ascii.resize(socket->pendingDatagramSize());
        socket->readDatagram(ascii.data(),ascii.size());

        QByteArray hex = ascii.toHex();
        QString binaryNumber = QString::number(hex.toLongLong(&ok, 16), 2);

        QString text=" ";

        for(int i=0;i< ascii.size();i++){

            quint8 decimal= ascii.at(i);
            text.append(QString::number(decimal));
            text.append(" ");

        }

        ui->lineEdit->setText(text);
        ui->lineEdit_4->setText(binaryNumber);
        ui->lineEdit_3->setText(hex);
        ui->lineEdit_2->setText(ascii);

        executeMessage(ascii);
    //}


}

void MainWindow::executeMessage(QByteArray ba){


    message* m = reinterpret_cast<message*>(ba.data());

    checkMessage(m);

}

void MainWindow::checkMessage(message *comingMessage){

    bool checkSum;

    if(comingMessage->header == TRUE_HEADER){


            checkSum=checkSumControl(comingMessage);

            if(checkSum==true){

                if(comingMessage->command == LEFT || comingMessage->command == RIGHT || comingMessage->command ==UP || comingMessage->command==DOWN){

                    qDebug()<<"message is true";
                    processMessage(comingMessage->command);
                }
                else{

                    qDebug()<<"invalid command"+QString::number(comingMessage->command);
                }
            }
            else{
                qDebug()<<"invalid checkSum";
            }
    }
    else{
        qDebug()<<"invalid header";
    }

}

bool MainWindow::checkSumControl(message* msg){

    int msgSize=sizeof(message);
    char text[msgSize];
    int sum=0;

    memcpy(text,msg,msgSize);

    for(int i=0; i< msgSize-1 ;i++){
        sum+=text[i];
    }


    if(sum != text[msgSize-1]){

        return false;
    }
    else{

        return true;
    }

}

void MainWindow::processMessage(quint16 command){

    int x,y,newX,newY;

    switch(command)
    {
        case LEFT:

            x=ui->frame->x();
            y=ui->frame->y();

            newX=x-5;

            if(newX >= 0){
               ui->frame->move(newX,y);
            }


            break;

        case RIGHT:

            x=ui->frame->x();
            y=ui->frame->y();

            newX=x+5;

            if(newX <= ui->widget->width()-ui->frame->width()){
               ui->frame->move(newX,y);
            }


            break;

        case UP:

            x=ui->frame->x();
            y=ui->frame->y();

            newY=y-5;

            if(newY >= 0){
               ui->frame->move(x,newY);
            }

            break;

        case DOWN:

            x=ui->frame->x();
            y=ui->frame->y();

            newY=y+5;

            if(newY <= ui->widget->height()-ui->frame->height()){
               ui->frame->move(x,newY);
            }

            break;

    }
}
