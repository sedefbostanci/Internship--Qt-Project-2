#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtSerialPort>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();
    void readSerial();
    void executeMessage(QByteArray ba);
    void checkMessage(struct message *comingMessage);
    bool checkSumControl(struct message *comingMessage);
    void processMessage(quint16 command);

private:

    Ui::MainWindow *ui;
    //QSerialPort *serial;
    QUdpSocket *socket;

};

#endif // MAINWINDOW_H
