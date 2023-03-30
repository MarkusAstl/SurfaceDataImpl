#include "loadingthread.h"
#include <QtCore>
#include <QDebug>
#include <surfacedata.h>

LoadingThread::LoadingThread(QObject *parent)   :
    QThread(parent)
{

}

QStringList LoadingThread::LoadOneLn(QFile *f){
    // Seperate Qstring of one line into QStringList of its values
    QString separator(";");
    QString ln = f->readLine();
    QStringList SplitLn = ln.split(separator);
    return SplitLn;
}

bool LoadingThread::saveValsToLists(QList<double> *TimeList, QList<double> *Amp1List, QStringList *SplitLn){
    // Save values of one line into corresponding lists
    bool ok1;
    double timeVal = SplitLn->at(0).toDouble(&ok1);
    bool ok2;
    double amp1Val = SplitLn->at(1).toDouble(&ok2);

    if (ok1 == false || ok2 == false) {
       return false;
       }
    else {
        TimeList->append(timeVal/1000);      // convert from ms to s
        Amp1List->append(amp1Val);
        return true;
    }
}

bool LoadingThread::readHeaderLines(QFile* f){
//    // Display files and directories in current folder
//    QDir dir;
//    foreach(QFileInfo item, dir.entryInfoList() )
//        {
//            if(item.isDir())
//                qDebug() << "Dir: " << item.absoluteFilePath();
//            if(item.isFile())
//                qDebug() << "File: " << item.absoluteFilePath();
//        }

    // Locate row of headers and determine number of columns
    int colNum = 0;

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        QString ln1 = f->readLine(0);

        if (lnInd == 5) {
            qDebug() << "Error in LoadingThread::readHeaderLines(QFile*)."
                        " First row of data in csv file was not found.";
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
        return false;
    }
    else {
        return true;
    }
}

void LoadingThread::readOnly(){

    // Read one line
    SplitLn = LoadOneLn(&f);

    // Save line in lists (TimeList, Amp1List)
    mutex.lock();
    if (!saveValsToLists(&TimeList, &Amp1List, &SplitLn)) {
        qDebug() << "Error in LoadingThread::readOnly()."
                    " Saving values was not succesful";
    }
    mutex.unlock();
}

void LoadingThread::run(){

    //--------Prepare for reading-------------------------------------------------------------------------------

    f.setFileName("SurfaceData/" + path);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Error in LoadingThread::run(). Opening SurfaceData file was not successful";
    }

    // Read header lines of csv file and find first datapoint
    if(!readHeaderLines(&f)){
            qDebug() << "Error in LoadingThread::run()."
                        " Reading header lines in function readHeaderLines(QFile*) was not successfull";
    }



    //--------Reading only-------------------------------------------------------------------------------------

    int iteration = 1;
    while (!f.atEnd()){

        readOnly();
        iteration = iteration + 1;
    }

    // Emit signal to start addAvgData() to adjust staticChart layout and to add the data
    emit DataLoadingFinished(&TimeList, &Amp1List);
    emit LoadingStopped();

    // Close file
    f.close();
}
