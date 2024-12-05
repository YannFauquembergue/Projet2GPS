#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Projet2GPS.h"
#include "QSerialPort"
#include "qserialportinfo.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Projet2GPS : public QMainWindow
{
    Q_OBJECT

public:
    Projet2GPS(QWidget *parent = nullptr);
    ~Projet2GPS();

private:
    Ui::Projet2GPSClass ui;
    QSerialPort* port;
    QString readBuffer;

    void ConnectToDatabase();
    void DecodeNMEATrame(const QString& nmeaTram);

public slots:
    void OnOpenPortButtonClicked();
    void OnRequestGPSDataButtonClicked();
    void OnSerialPortReadyRead();
};
