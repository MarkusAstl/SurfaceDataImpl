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

    // Display files and directories in current folder
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

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        QString ln1 = f.readLine(0);

        if (lnInd == 5) {
            qDebug() << f.errorString();
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
    }


    // Read data line by line
    while (!f.atEnd()){

        // Read one line
        QStringList SplitLn = readOneLn(f);

        // Stop if stop button was clicked
        if(this->Stop) break;

        // Emit signal to update time and ampl values in UI
        emit LnReadingFinished(SplitLn, colNum);        // ??? Maybe emit two similar signals instead of only one

        // Save line in lists (TimeList, Amp1List, Amp2List)
        QMutex mutex;               // Lock lists and columns to avoid acessing same storage twice at the same time
        mutex.lock();
        if (!saveValsToLists(this->TimeList, this->Amp1List, this->Amp2List, SplitLn, colNum)) {
            qDebug() << f.errorString();
        }
        mutex.unlock();

        // Sleep for certain time to simulate dynamical reading
        this->msleep(this->sleepingTime);
    }

    // Emit signal to start addAvgData() to adjust staticChart layout and to add the data
    emit DataReadingFinished(&TimeList, &Amp1List, &Amp2List, colNum);
}
