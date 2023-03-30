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

#include <fstream>

#include <chrono>
#include <thread>

#include <QElapsedTimer>

SurfaceData::SurfaceData(QObject *parent)   :
    QThread(parent)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------
// Functions


bool SurfaceData::saveValsToLists(QList<double> *TimeList, QList<double> *Amp1List, QStringList *SplitLn){
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

QStringList SurfaceData::readOneLn(QFile *f){
    // Seperate Qstring of one line into QStringList of its values
    QString separator(";");
    QString ln = f->readLine();
    QStringList SplitLn = ln.split(separator);
    return SplitLn;
}

bool SurfaceData::filterNewDatapoint(QList<double> *Amp1List, QList<double> *smthdAmp1List, double *aValue){
    // Low pass filter construncted with convolution using a left sided exponential moving avergage filter
    double newSmthdVal = *aValue * Amp1List->rbegin()[0] + (1 - *aValue) * smthdAmp1List->rbegin()[0];
    smthdAmp1List->append(newSmthdVal);
    return true;
}


void SurfaceData::phaseRecognition(double* t, double* t_lastMax, double* dT, int* phase, bool* phaseRecogActive){
    // Determine phase
    double dt = *t - *t_lastMax;
    *phase = int(dt/ *dT) + 1;

    if (*phase > 10) {
        if (*phase >= 14){
            *phase = 0;                  // Fallback if t < t_lastMax
            *phaseRecogActive = false;
        }
        else{
            *phase = *phase - 10;       // Fallback if t > 13*dT
        }
    }
}


bool SurfaceData::newMaxDetected(QList<double>* AmpList, bool* maxIsLast, int* lastMinInd, int* lastMaxInd){
    // Check if monotony changed in last 3 datapoints
    bool newMax = false;
    if (*maxIsLast && AmpList->rbegin()[2] < AmpList->rbegin()[1] && AmpList->rbegin()[2] < AmpList->rbegin()[0]){
        *maxIsLast = false;
        *lastMinInd = AmpList->size() - 3;
    }
    else if (!*maxIsLast && AmpList->rbegin()[2] > AmpList->rbegin()[1] && AmpList->rbegin()[2] > AmpList->rbegin()[0]){
        if (abs(AmpList->rbegin()[2]-AmpList->at(*lastMinInd)) > 0.3*abs(AmpList->at(*lastMinInd)-AmpList->at(*lastMaxInd))){
            newMax = true;
        }
        *maxIsLast = true;
    }
    return newMax;
}

bool SurfaceData::checkBreathCycleDur(double* T, double* T_planning, double* T_old, bool* phaseRecogActive) {
    if (*T < 0.5 * *T_planning || *T < 0.75* *T_old || *T > 1.25 * *T_old){            // 0.5 because of hysteresis
        *phaseRecogActive = false;
    }
    else {
        *phaseRecogActive = true;
    }
    return *phaseRecogActive;
}

bool SurfaceData::readHeaderLines(QFile* f){
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
            qDebug() << "Error in SurfaceData::readHeaderLines(QFile*)."
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

bool SurfaceData::createOutputFile(){
    // Create csv file for testing phase recognition
    QString testOutputFileName = "SurfaceData/Measured/MEASURED_" + path + ".csv";
    fout.setFileName(testOutputFileName);
    if (!fout.open(QIODevice::WriteOnly|QIODevice::Text)) {
        qDebug() << "Error in SurfaceData::createOutputFile()."
                    " Creating output file was not successful";
    }
    QTextStream stream(&fout);

    if (TimeList.size() == PhaseList.size()){
        for(int i = 0; i < TimeList.size(); i++) {
            stream << TimeList[i] << ',' << PhaseList[i] << "\n";
        }
        return true;
    }
    else{
        return false;
    }

    fout.close();
    qDebug() << "Output file is closed";
}

void SurfaceData::readOnly(){

    // Read one line
    SplitLn = readOneLn(&f);

    // Emit signal to update time and ampl values in UI
    emit LnReadingFinished(SplitLn);

    // Save line in lists (TimeList, Amp1List)
    mutex.lock();
    if (!saveValsToLists(&TimeList, &Amp1List, &SplitLn)) {
        qDebug() << "Error in SurfaceData::readOnly()."
                    " Saving values was not succesful";
    }
    mutex.unlock();
}

void SurfaceData::setFilterParams(){
    fs = 1/(TimeList[1]-TimeList[0]);
    aValue = qSqrt( qPow( qCos(2*M_PI* fc/ fs), 2) - 4*qCos(2*M_PI* fc/ fs) + 3) + qCos(2*M_PI* fc/ fs) - 1;

    for (int u = 0; u < Amp1List.size()-1; u++){
         smthdAmp1List.append(Amp1List[u]);
    }

    if (!filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue)) {
        qDebug() << "Error in SurfaceData::setFilterParams()."
                    " Filtering in SurfaceData::filterNewDatapoint(QList<double> *, QList<double> *, "
                    "double *) was not successfull.";
    }
    emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

    phaseRecogActive = true;

    double frst_t = TimeList.rbegin()[0];
    double frst_A = Amp1List.rbegin()[0];
    emit CreateChart(frst_t, frst_A, smthdAmp1List.rbegin()[0]);
}

void SurfaceData::run(){

    //--------Prepare for reading-------------------------------------------------------------------------------

    f.setFileName("SurfaceData/" + path);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Error in SurfaceData::run(). Opening SurfaceData file was not successful";
    }

    // Read header lines of csv file and find first datapoint
    if(!readHeaderLines(&f)){
            qDebug() << "Error in SurfaceData::run()."
                        " Reading header lines in function readHeaderLines(QFile*) was not successfull";
    }

    // Initialize parameters
    PhaseRecogStarted = false;
    double maxElapsed = 0.00000000000;



    //--------Reading only-------------------------------------------------------------------------------------

    int iteration = 1;
    while (iteration <= windowSize){

        readOnly();
        iteration = iteration + 1;

        //qDebug() << "runtime: " << elapsed << " ns";
        msleep(sleepingTime);
    }


    //--------Set parameters for filtering---------------------------------------------------------------------

    setFilterParams();

    //--------Reading and filtering----------------------------------------------------------------------------
    while (!PhaseRecogStarted){

        readOnly();

        if (!filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue)) {
            qDebug() << "Error in SurfaceData::run()."
                        "Filtering in function filterNewDatapoint(QList<double> *, QList<double> *, double *) "
                        "during section 'Reading and filtering' was not successfull";
        }
        emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

        //qDebug() << "runtime: " << elapsed << " ns";
        msleep(sleepingTime);
    }



    //--------Reading, filtering and phase recognition---------------------------------------------------------

    while (!f.atEnd()){
        //timer.start();

        readOnly();

        if (!filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue)) {
            qDebug() << "Error in SurfaceData::run()."
                        "Filtering in function filterNewDatapoint(QList<double> *, QList<double> *, double *)"
                        "during section 'Reading, filtering and phase recognition' was not successfull";
        }

        // Check if there is a new max
        if (newMaxDetected(&smthdAmp1List, &maxIsLast, &lastMinInd, &newMaxInd)){

            // Update indices of last 2 max
            oldMaxInd = newMaxInd;
            newMaxInd = TimeList.size()-3;

            // Update time of newest (T) and last (T_old) breathing cycle
            T_old = T;
            T = abs(TimeList[newMaxInd] - TimeList[oldMaxInd]);

            if (!checkBreathCycleDur(&T, &T_planning, &T_old, &phaseRecogActive)){
                // Fallback for significant changes in duration of breathing cycle
                phase = 0;

                //Only for validating phase recognition
                IntervalPhaseList.fill(0);
                PhaseList.append(IntervalPhaseList);
                IntervalPhaseList.clear();
            }
            else {
                // Update phase borders
                dT = T /10.0;
                correctedNewMaxTime = TimeList[newMaxInd] - filterShift;
                PhaseList.append(IntervalPhaseList);
                IntervalPhaseList.clear();
            }
        }

        // Do phase recognition
        if (phaseRecogActive){
            phaseRecognition(&TimeList.rbegin()[0], &correctedNewMaxTime, &dT, &phase, &phaseRecogActive);
        }

        emit showCurrentPhase(&phase);

        // Sleep for certain time to simulate dynamical reading
        elapsed = timer.nsecsElapsed();

        // Save longest runtime
        double elapsedDouble = static_cast<double>(elapsed);
        if (elapsedDouble > maxElapsed){
            maxElapsed = elapsedDouble;
        }

        //qDebug() << "runtime: " << elapsed << " ns";
        msleep(sleepingTime);

        IntervalPhaseList.append(phase);
    }



    //--------Performance documentation------------------------------------------------------------------------

    // Display longest runtime
    qDebug() << "Longest runtime: " << maxElapsed << " ns";

    // Create csv file with measured phase for each time value
    PhaseList.append(IntervalPhaseList);
    if(!createOutputFile()){
        qDebug() << "Error in SurfaceData::run(). Writing csv file was not successful";
    }
}



