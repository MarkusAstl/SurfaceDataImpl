#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    qDebug() << "App path : " << qApp->applicationDirPath();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
