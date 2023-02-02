#include "surfacedata.h"

#include <QtCore>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <Qstring>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QtMath>

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

bool filterNewDatapoint(QList<double> *Amp1List, QList<double> *smthdAmp1List, int *windowSize, QList<double> *h, double *aValue){
    // Low pass filter construncted with convolution using a left sided exponential moving avergage filter

    int N = Amp1List->size();
    double convSum = 0.0;
    double convProd = 1.0;

    for(int convInd = 0; convInd < *windowSize; ++convInd){
        convProd = *aValue * (Amp1List->at(N-convInd-1) * h->at(convInd));
        convSum = convProd+convSum;
    }

    smthdAmp1List->append(convSum);
    return true;
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


void SurfaceData::phaseRecognition(bool* maxIsLast, double* t, QList<double>* globalBorderTimes, int* phase)
{
    // Adjust to monotony
    int base;
    if (*maxIsLast){
        base = 5;
    }
    else{
        base = 0;
    }

    // Determine phase
    if (*t < globalBorderTimes->at(0)){
        *phase = base + 1;
    }
    else if(*t >= globalBorderTimes->at(0) && *t < globalBorderTimes->at(1)){
        *phase = base + 2;
    }
    else if(*t >= globalBorderTimes->at(1) && *t < globalBorderTimes->at(2)){
        *phase = base + 3;
    }
    else if(*t >= globalBorderTimes->at(2) && *t < globalBorderTimes->at(3)){
        *phase = base + 4;
    }
    else if(*t >= globalBorderTimes->at(3) && *t < globalBorderTimes->at(4)){
        *phase = base + 5;
    }
    else if(*t >= globalBorderTimes->at(4) && *t < globalBorderTimes->at(5)){
        *phase = abs(base - 6);
    }
    else if(*t >= globalBorderTimes->at(5) && *t < globalBorderTimes->at(6)){
        *phase = abs(base - 6);
    }
    else if(*t >= globalBorderTimes->at(6) && *t < globalBorderTimes->at(7)){
        *phase = abs(base - 7);
    }
    else if(*t >= globalBorderTimes->at(7) && *t < globalBorderTimes->at(8)){
        *phase = abs(base - 8);
    }
    else if(*t >= globalBorderTimes->at(8) && *t < globalBorderTimes->at(9)){
        *phase = abs(base - 9);
    }
    else if(*t >= globalBorderTimes->at(5)){
        *phase = abs(base - 10);
    }

    qDebug() << "t = " << *t << " -> phase = " << *phase;
}



bool SurfaceData::monotonyChanged(double* p1, double* p2, double* p3, bool* maxIsLast){

    // Check if monotony changed in last 3 datapoints
    bool newExtremum = false;

    if (*maxIsLast && *p3 < *p2 && *p3 < *p1){
        newExtremum = true;
        *maxIsLast = false;
    }
    else if (!*maxIsLast && *p3 > *p2 && *p3 > *p1){
        newExtremum = true;
        *maxIsLast = true;
    }

    return newExtremum;
}

void SurfaceData::calcPhaseBordersByTime(QList<double>* data, QList<double>* TimeList, QList<double>* BorderTimes,
                      QList<double>* globalBorderTimes, int* A3ind, int* A2ind, int* A1ind){
    // A1...last extremum, A2...second last extremum, A3...third last extremum


    double T = TimeList->at(*A1ind) - TimeList->at(*A3ind);
    double T_half = TimeList->at(*A1ind) - TimeList->at(*A2ind);
    qDebug() << "Period time of phase border calculation interval from " << TimeList->at(*A3ind) << " to " <<
                TimeList->at(*A1ind) << " = " << T;
    qDebug() << "Future Interval should be:  " << TimeList->at(*A1ind) << " to " << TimeList->at(*A1ind)+T_half << " to " << TimeList->at(*A1ind)+T;

    // Calculate Ammplitude Difference for one phase
    double dA = data->at(*A2ind) - data->at(*A1ind);
    double dAphase = dA / 5.0;

    // Phases will be updated for only one half of the breathing cycle
    QList<double> newBorderTimes(BorderTimes->mid(5,9));

    // Locate borders between phases
    int phaseInd = 1;
    int i = *A2ind;

    while(i <= *A1ind && phaseInd <= 5){
        double dA_iteration = abs(data->at(i) - data->at(*A2ind));

        if (dA_iteration >= abs(phaseInd*dAphase)){
            double globalBorderTime = TimeList->at(i);
            double localBorderTime = globalBorderTime - TimeList->at(*A2ind);
            newBorderTimes.append(localBorderTime);
            phaseInd = phaseInd + 1;
        }

        i = i+1;
    }

    // Update phase border times
    *BorderTimes = newBorderTimes;

    QList<double> newGlobalBorderTimes = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    double globalBorderTime = TimeList->at(*A1ind);
    for (int pInd = 0; pInd < 5; pInd++){
        newGlobalBorderTimes[pInd] = globalBorderTime + BorderTimes->at(pInd);
    }

    globalBorderTime = newGlobalBorderTimes[4];
    for (int pInd = 5; pInd < 10; pInd++){
        newGlobalBorderTimes[pInd] = globalBorderTime + BorderTimes->at(pInd);
    }

    *globalBorderTimes = newGlobalBorderTimes;
}



void SurfaceData::run(){

//    // Display files and directories in current folder
//    QDir dir;
//    foreach(QFileInfo item, dir.entryInfoList() )
//        {
//            if(item.isDir())
//                qDebug() << "Dir: " << item.absoluteFilePath();
//            if(item.isFile())
//                qDebug() << "File: " << item.absoluteFilePath();
//        }

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

    // Iteration index for start of filtering
    int iteration = 0;

    //
    prReady = false;
    QMutex mutex;               // Lock lists and columns to avoid acessing same storage twice at the same time


    // Read data line by line
    while (!f.atEnd()){
        iteration = iteration + 1;

        // Read one line
        QStringList SplitLn = readOneLn(f);

        // Stop if stop button was clicked
        if(this->Stop) break;

        // Emit signal to update time and ampl values in UI
        emit LnReadingFinished(SplitLn, colNum);        // ??? Maybe emit two similar signals instead of only one

        // Save line in lists (TimeList, Amp1List, Amp2List)
        mutex.lock();
        if (!saveValsToLists(TimeList, Amp1List, Amp2List, SplitLn, colNum)) {
            qDebug() << f.errorString();
        }
        mutex.unlock();


        if (iteration > windowSize && filtering){

            // Filter data
            if (!filterNewDatapoint(&Amp1List, &smthdAmp1List, &windowSize, &h, &aValue)) {     // !!!! Add mutex if prThread is defined
                qDebug() << f.errorString();
            }

            // Wait till initial parameters for phase recognition are set
            if(prReady){

                dt = TimeList.rbegin()[0] - TimeList[peak1Ind];

                // Update parameters if new extremum is detected
                if (monotonyChanged(&smthdAmp1List.rbegin()[0], &smthdAmp1List.rbegin()[1], &smthdAmp1List.rbegin()[2], maxIsLast)){

                    if(dt < 0.8 * BorderTimes[4]){
                        *maxIsLast = !*maxIsLast;
                    }
                    else{
                        peak3Ind = peak2Ind;
                        peak2Ind = peak1Ind;
                        peak1Ind = smthdAmp1List.size()-4;

                        qDebug() << "New extremum at t = " << TimeList[peak1Ind];

                        mutex.lock();
                        calcPhaseBordersByTime(&smthdAmp1List, &TimeList, &BorderTimes, &globalBorderTimes, &peak3Ind, &peak2Ind, &peak1Ind);
                        mutex.unlock();

                        //emit monotonyCheckDone(&smthdAmp1List, &this->BorderTimes, &this->peak3Ind, &this->peak2Ind, &this->peak1Ind);
                        qDebug() << globalBorderTimes;
                    }
                }

                if(dt > 1.3 * BorderTimes[4]){
                    phase = 0;
                    prReady = false;
                }

                // Do phase recognition
                phaseRecognition(maxIsLast, &TimeList.rbegin()[0], &globalBorderTimes, &phase);
                emit showCurrentPhase(&phase);
            }
        }

        // Set parameters for filtering
        else if (iteration == windowSize){
            double fs = 1/(TimeList[1]-TimeList[0]);
            aValue = qSqrt( qPow( qCos(2*M_PI* fc/ fs), 2) - 4*qCos(2*M_PI* fc/ fs) + 3) + qCos(2*M_PI* fc/ fs) - 1;
            fValue = 1-aValue;

            for (int k = 0; k <= windowSize; k++){
                h.append(qPow(fValue, k));
            }

            if (!filterNewDatapoint(&Amp1List, &smthdAmp1List, &windowSize, &h, &aValue)) {
                qDebug() << f.errorString();
            }

            double firstVal = smthdAmp1List[0];
            for (int u = 1; u < windowSize; u++){
                 smthdAmp1List.append(firstVal);
            }

            // Initialize parameters for phase recognition
            maxIsLast = new bool;
            //phase = new int;
        }

        // Sleep for certain time to simulate dynamical reading
        this->msleep(this->sleepingTime);
    }

    // Emit signal to start addAvgData() to adjust staticChart layout and to add the data
    emit DataReadingFinished(&TimeList, &Amp1List, &Amp2List, colNum);
}


