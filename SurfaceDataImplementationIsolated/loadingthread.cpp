// LoadingThread is only for loading the average breathing cycle of the planning CT

#include "loadingthread.h"
#include <QtCore>
#include <QDebug>
#include <surfacedata.h>

LoadingThread::LoadingThread(QObject *parent)   :
    QThread(parent)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------
// Functions

QStringList LoadingThread::LoadOneLn(QFile *f){
    // f: csv file with amplitude & time values of the average breathing cycle of the planning CT
    // Read one line in file and seperate it into QStringList of its values
    // SplitLn: [timeVal, Amp1Val, Amp2Val] -> also without Amp2Val possible

    QString separator(";");
    QString ln = f->readLine();
    QStringList SplitLn = ln.split(separator);
    return SplitLn;
}

bool LoadingThread::saveValsToLists(QList<double> *TimeList, QList<double> *Amp1List, QStringList *SplitLn){
    // Convert QStrings of new line (SplitLn) to doubles and add them to TimeList and Amp1List
    // Return true if saving was successfull

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
    // Find expected header lines in csv file f to know where the actual data starts
    // Return true if headers were found in first 5 rows


//    // Display files and directories in current folder
//    QDir dir;
//    foreach(QFileInfo item, dir.entryInfoList() )
//        {
//            if(item.isDir())
//                qDebug() << "Dir: " << item.absoluteFilePath();
//            if(item.isFile())
//                qDebug() << "File: " << item.absoluteFilePath();
//        }

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    QString ln1;
    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        ln1 = f->readLine(0);

        if (lnInd == 5) {
            qDebug() << "Error in LoadingThread::readHeaderLines(QFile*)."
                        " Headers in csv file were not found in first 5 lines.";
            return false;
        }
        else if (ln1 == twoHeaders) {
            return true;
        }
        else if (ln1 == threeHeaders) {
            return true;
        }
    }
}

void LoadingThread::readOnly(){
    // Read one line of the csv file f and save the values as doubles to TimeList and Amp1List

    SplitLn = LoadOneLn(&f);

    mutex.lock();
    if (!saveValsToLists(&TimeList, &Amp1List, &SplitLn)) {
        qDebug() << "Error in LoadingThread::readOnly(). Saving values was not succesful";
    }
    mutex.unlock();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------




void LoadingThread::run(){
    // Read whole csv file f, save values to TimeList and Amp1List and emit signal LoadingStopped if finished

    //--------Prepare for reading-------------------------------------------------------------------------------

    f.setFileName("SurfaceData/" + path);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Error in LoadingThread::run(). Opening SurfaceData file was not successful";
    }

    if(!readHeaderLines(&f)){
            qDebug() << "Error in LoadingThread::run()."
                        " Reading header lines in function readHeaderLines(QFile*) was not successfull";
    }



    //--------Reading------------------------------------------------------------------------------------------

    while (!f.atEnd()){
        readOnly();
    }

    // Emit signal to set up staticChart and to add loaded data
    emit DataLoadingFinished(&TimeList, &Amp1List);

    // Emit signal to adjust UI
    emit LoadingStopped();

    f.close();
}
