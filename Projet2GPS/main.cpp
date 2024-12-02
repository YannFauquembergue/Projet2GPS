#include "Projet2GPS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Projet2GPS w;
    w.show();
    return a.exec();
}
