#include "mainwindow.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //QTimer::singleShot(25000, &w, SLOT(close()));


    return a.exec();
}
