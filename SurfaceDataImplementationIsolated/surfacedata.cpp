#include "surfacedata.h"

#include <QtCore>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <Qstring>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <new>

#include <chrono>
#include <thread>

SurfaceData::SurfaceData(QObject *parent)   :
    QThread(parent)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------
// Functions


QStringList readOneLn(QFile &f){
    // Seperate Qstring of one line into QStringList of its values

    QString separator(";");

    QString ln = f.readLine();
    QStringList SplitLn = ln.split(separator);
    //qDebug() << SplitLn;

    return SplitLn;
}


bool saveValsToLists(QList<double> &TimeList, QList<double> &Amp1List, QList<double> &Amp2List, QStringList &SplitLn, int &colNum){
    // Save values of one line into corresponding lists

    bool ok;

    for (int colInd = 0; colInd < colNum; ++colInd) {
        double doubleVal = SplitLn[colInd].toDouble(&ok);

         if (ok == false) {
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

    return true;
}


void SurfaceData::run(){

    // Check files and directories in current folder
    QDir dir;
    foreach(QFileInfo item, dir.entryInfoList() )
        {
            if(item.isDir())
                qDebug() << "Dir: " << item.absoluteFilePath();
            if(item.isFile())
                qDebug() << "File: " << item.absoluteFilePath();
        }

    // Open csv file

    QFile f(this->path);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << f.errorString();
    }


    // Locate row of headers and determine number of columns

    int colNum = 0;
    int lastLnInd = 0;

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        QString ln1 = f.readLine(0);

        qDebug() << ln1;

        if (lnInd == 5) {
            qDebug() << f.errorString();
            //return false;
        }
        else if (ln1 == twoHeaders) {
            colNum = 2;
            break;
        }
        else if (ln1 == threeHeaders) {
            colNum = 3;
            break;
        }
        lastLnInd += 1;

    }


    // Check if number of columns is permissible

    if (colNum == 0) {
        qDebug() << f.errorString();
    }


    // Read data

    while (!f.atEnd()){

        // Read one line
        QStringList SplitLn = readOneLn(f);
        if(this->Stop) break;
        //qDebug() << SplitLn;
        emit LnReadingFinished(SplitLn, colNum);

        // Save line in lists
        QMutex mutex;         // Lock lists and columns to avoid acessing same storage twice at the same time
        mutex.lock();
        if (!saveValsToLists(this->TimeList, this->Amp1List, this->Amp2List, SplitLn, colNum)) {
            qDebug() << f.errorString();
        }
        mutex.unlock();

        // Sleep for certain time
        this->msleep(this->sleepingTime);
    }

    emit DataReadingFinished(&TimeList, &Amp1List, &Amp2List, colNum);
}
