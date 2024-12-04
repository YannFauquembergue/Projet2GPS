#include "Projet2GPS.h"

Projet2GPS::Projet2GPS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (int i = 0; i < availablePorts.size(); i++)
    {
        QSerialPortInfo info = availablePorts[i];
        ui.portCombo->addItem(info.portName(), QVariant(info.portName()));
    }
}

Projet2GPS::~Projet2GPS()
{}

void Projet2GPS::OnOpenPortButtonClicked()
{
    if (ui.portCombo->currentIndex() >= 0)
    {
        port = new QSerialPort(ui.portCombo->currentText());
        QObject::connect(port, SIGNAL(readyRead()), this, SLOT(OnSerialPortReadyRead()));
        port->setBaudRate(QSerialPort::Baud9600);
        port->setDataBits(QSerialPort::DataBits::Data8);
        port->setParity(QSerialPort::Parity::NoParity);
        port->setStopBits(QSerialPort::StopBits::OneStop);
        if (port->open(QIODevice::OpenModeFlag::ExistingOnly | QIODevice::OpenModeFlag::ReadWrite))
        {
            ui.portStatusLabel->setText("Statut port : Ouvert");
        }
    }
}

void Projet2GPS::OnRequestGPSDataButtonClicked()
{
    if (port->isOpen())
    {
        port->write("REQUEST_GPS_DATA");
    }
}

void Projet2GPS::OnSerialPortReadyRead()
{
    QByteArray data = port->read(port->bytesAvailable());
    QString str(data);
}
