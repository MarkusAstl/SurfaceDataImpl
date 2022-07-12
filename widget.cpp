#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <Qstring>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <new>


QString path1 = "surface_datasets/BruMi-20210531-083409_clean.csv";                          // two columns
QString path2 = "surface_datasets/2021_11_30_Sarah_RespSignal_20211130_171956_clean.csv";    // three columns
QString path3 = "surface_datasets/HaGer-202104022_162059-Leber_clean.csv";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


bool ReadCSV(QString path)
{
    // Open csv file

    /*
    //Check files in current folder
    QDir dir;
    QFileInfoList dirlist = dir.entryInfoList();
    qDebug() << dirlist;
    */

    QFile f(path);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << f.errorString();
        return false;
    }

    // Locate row of headers and determine number of columns

    int colNum = 0;

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        QString ln1 = f.readLine(0);

        if (lnInd == 5) {
            qDebug() << f.errorString();
            return false;
        }
        else if (ln1 == twoHeaders) {
            colNum = 2;
            break;
        }
        else if (ln1 == threeHeaders) {
            colNum = 3;
            break;
        }
    }

    // Check if number of columns is permissible

    if (colNum == 0) {
        qDebug() << f.errorString();
        return false;
    }

    // Load data

    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;

    QString separator(";");
    bool ok;

    while (!f.atEnd())
    {
        QString ln = f.readLine();
        QStringList SplitLn = ln.split(separator);

        for (int colInd = 0; colInd < colNum; ++colInd) {
            double doubleVal = SplitLn[colInd].toDouble(&ok);

            if (ok == false) {
                qDebug() << f.errorString();
                return false;
            }

            switch (colInd) {

            case 0:
                TimeList.append(doubleVal);
                break;
            case 1:
                Amp1List.append(doubleVal);
                break;
            case 2:
                Amp2List.append(doubleVal);
                break;
            }
        }
    }

    // Check if all lists have the same length

    int tLen = TimeList.length();
    int a1Len = Amp1List.length();
    int a2Len = Amp2List.length();

    switch (colNum) {
    case 2:
        if (tLen != a1Len) {
            qDebug() << f.errorString();
            return false;
        }
        break;
    case 3:
        if (tLen != a1Len || tLen != a2Len) {
            qDebug() << f.errorString();
            return false;
        }
        break;
    }

    // Display lists

    qDebug() << TimeList;
    qDebug() << Amp1List;
    qDebug() << Amp2List;

    f.close();

    return true;
}

void Widget::loadSurfaceDataSlot()
{
    bool done0 = ReadCSV(path1);
}

