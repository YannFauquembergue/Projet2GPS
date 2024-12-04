#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Projet2GPS.h"
#include "QSerialPort"
#include "qserialportinfo.h"

class Projet2GPS : public QMainWindow
{
    Q_OBJECT

public:
    Projet2GPS(QWidget *parent = nullptr);
    ~Projet2GPS();

private:
    Ui::Projet2GPSClass ui;
    QSerialPort* port;

public slots:
    void OnOpenPortButtonClicked();
    void OnRequestGPSDataButtonClicked();
    void OnSerialPortReadyRead();
};
