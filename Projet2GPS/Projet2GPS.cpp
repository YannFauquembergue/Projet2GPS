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

void Projet2GPS::ConnectToDatabase()
{
    // Création de l'objet QSqlDatabase
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("192.168.64.207");
    db.setDatabaseName("Lowrence");
    db.setUserName("lowrenceUser");
    db.setPassword("LowrencePASSWORD");
    db.setPort(45);

    // Connexion
    if (db.open())
    {
        qDebug() << "Connexion à la base de données réussie!";
    }
    else
    {
        qDebug() << "Échec de la connexion à la base de données:" << db.lastError().text();
    }
}

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
    QByteArray data = port->readAll();
    readBuffer.append(QString(data));

    int index;
    while ((index = readBuffer.indexOf("\r\n")) != -1)
    {
        QString nmeaTrame = readBuffer.left(index).trimmed();
        readBuffer.remove(0, index + 2);

        DecodeNMEATrame(nmeaTrame);
    }
}

void Projet2GPS::DecodeNMEATrame(const QString& nmeaTrame)
{
    QStringList fields = nmeaTrame.split(",");
    if (fields.size() < 6 || fields[0] != "$GPGGA")
        return;

    // latitude
    QString rawLatitude = fields[2];
    QString latHemisphere = fields[3];
    double latitude = rawLatitude.mid(0, 2).toDouble() + (rawLatitude.mid(2).toDouble() / 60.0);
    if (latHemisphere == "S")
        latitude = -latitude;

    // longitude
    QString rawLongitude = fields[4];
    QString lonHemisphere = fields[5];
    double longitude = rawLongitude.mid(0, 3).toDouble() + (rawLongitude.mid(3).toDouble() / 60.0);
    if (lonHemisphere == "W")
        longitude = -longitude;

    ui.dataBox->addItem("Latitude: " + QString::number(latitude, 'f', 6));
    ui.dataBox->addItem("Longitude: " + QString::number(longitude, 'f', 6));
}
