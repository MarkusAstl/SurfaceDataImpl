// SurfaceData thread is for reading breathing data dynamically from a csv file and applying phase recognition

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

SurfaceData::SurfaceData(QObject *parent, Ui::MainWindow *ui)   :
    QThread(parent), uiPtr(ui)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------
// Functions


bool SurfaceData::saveValsToLists(QList<double> *TimeList, QList<double> *Amp1List, QStringList *SplitLn){
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

bool SurfaceData::createOutputFile(QList<double>* TimeList, QVector<int>* PhaseList){
    // Create csv file for testing phase recognition
    QString testOutputFileName = "SurfaceData/Measured/MEASURED_" + path;
    fout.setFileName(testOutputFileName);
    if (!fout.open(QIODevice::WriteOnly|QIODevice::Text)) {
        qDebug() << "Error in SurfaceData::createOutputFile()."
                    " Creating output file was not successful";
    }
    QTextStream stream(&fout);

    if (TimeList->size() == PhaseList->size()){
        for(int i = 0; i < TimeList->size(); i++) {
            stream << TimeList->at(i) << ',' << PhaseList->at(i) << "\n";
        }
        fout.close();
        return true;
    }
    else{
        qDebug() << "Error in SurfaceData::createOutputFile()."
                    "List sizes are not matching: " << "TimeList: " << TimeList->size() << "; "<< "PhaseList: " << PhaseList->size();
        return false;
    }
}

QStringList SurfaceData::readOneLn(QFile *f){
    // f: csv file with amplitude & time values of real time data
    // Read one line in file and seperate it into QStringList of its values
    // SplitLn: [timeVal, Amp1Val, Amp2Val] -> also without Amp2Val possible

    QString separator(";");
    QString ln = f->readLine();
    QStringList SplitLn = ln.split(separator);
    return SplitLn;
}

void SurfaceData::filterNewDatapoint(QList<double> *Amp1List, QList<double> *smthdAmp1List, double *aValue){
    // Amp1List: raw ampl vals;  smthdAmp1List: filtered ampl vals;  avalue: parameter representing cutoff frequency
    // Left sided exponential moving avergage filter applied on newest datapoint

    double newSmthdVal = *aValue * Amp1List->rbegin()[0] + (1 - *aValue) * smthdAmp1List->rbegin()[0];
    smthdAmp1List->append(newSmthdVal);
}


void SurfaceData::phaseRecognition(double* t, double* t_lastMax, double* dT, int* phase, double* amp,
                                   double* compareAmp, double *validAmpDif, bool* phaseRecogActive){
    // t: newest time val; t_lastMax: time val of last detected max; dT: last breathing cycle duration
    // phase: current breathing phase; phaseRecogActive: true if there is no fallback/error
    // Assume current phase by projecting 13 phase durations from last cycle into the future starting at time of last Max
    // Causes a fallback (phaseRecogActive=false & phase=0) if no new max is detected for longer than t = dT * 1.3
    // Adjusts phases from 11 to 13 to valid phases from 1 to 3

    double dt = *t - *t_lastMax;
    *phase = int(dt/ *dT) + 1;

    if (*phase > 10) {
        if (*phase >= 14){  // Fallback
            *phase = 0;
            *phaseRecogActive = false;
            logStream << "FP\n";
        }
        else{
            *phase = *phase - 10;
        }
    }

    if (*amp < *compareAmp - *validAmpDif || *amp > *compareAmp + *validAmpDif) {       // Fallback
        *phase = 0;
        *phaseRecogActive = false;
        logStream << "FA\n";
    }
}


bool SurfaceData::newMaxDetected(QList<double>* AmpList, bool* maxIsLast, int* lastMinInd, int* lastMaxInd){
    // Check if monotony changed in last 3 datapoints
    bool newMax = false;

    if (*maxIsLast && AmpList->rbegin()[2] < AmpList->rbegin()[1] && AmpList->rbegin()[2] < AmpList->rbegin()[0]){
        *maxIsLast = false;
        *lastMinInd = AmpList->size() - 3;
        logStream << "Mn\n";
    }

    else if (!*maxIsLast && AmpList->rbegin()[2] > AmpList->rbegin()[1] && AmpList->rbegin()[2] > AmpList->rbegin()[0]){
        if (abs(AmpList->rbegin()[2]-AmpList->at(*lastMinInd)) > 0.3*abs(AmpList->at(*lastMinInd)-AmpList->at(*lastMaxInd))){
            newMax = true;
            logStream << "v";
        }
        *maxIsLast = true;
        logStream << "Mx\n";
    }
    return newMax;
}

bool SurfaceData::checkBreathCycleDur(double* T, double* T_planning, double* T_old, bool* phaseRecogActive) {
    // Breath cycle duration must be in certain interval to be valid -> otherwise fallback
    if (*T < 0.5 * *T_planning || *T < 0.75* *T_old || *T > 1.25 * *T_old){            // 0.5 because of hysteresis
        *phaseRecogActive = false;      // Fallback
        logStream << "FT" << "\n";
    }
    else {
        *phaseRecogActive = true;
    }
    return *phaseRecogActive;
}

bool SurfaceData::readHeaderLines(QFile* f){
    // Find expected header lines in csv file f to know where the actual data starts
    // headersFound: true if headers were found in first 5 rows


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

    bool headersFound = false;

    QString lnStr;
    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        lnStr = f->readLine(0);

        if (lnInd == 5) {
            qDebug() << "Error in LoadingThread::readHeaderLines(QFile*)."
                        " Headers in csv file were not found in first 5 lines.";
            break;
        }
        else if (lnStr == twoHeaders || lnStr == threeHeaders) {
            headersFound = true;
            break;
        }
    }

    return headersFound;
}

void SurfaceData::readOnly(){
    // Read one line of the csv file f and save the values as doubles to TimeList and Amp1List

    // Read one line
    SplitLn = readOneLn(&f);

    // Save line in lists (TimeList, Amp1List)
    mutex.lock();
    if (!saveValsToLists(&TimeList, &Amp1List, &SplitLn)) {
        logStream << "Error in SurfaceData::readOnly()."
                     " Saving values was not succesful";
    }
    mutex.unlock();
}

void SurfaceData::setFilterParams(){
    // Set all parameters for one sided exponential moving average filter

    fs = 1/(TimeList[1]-TimeList[0]);
    aValue = qSqrt( qPow( qCos(2*M_PI* fc/ fs), 2) - 4*qCos(2*M_PI* fc/ fs) + 3) + qCos(2*M_PI* fc/ fs) - 1;

    // First n (=windowSize) values cannot be filtered
    for (int u = 0; u < Amp1List.size()-1; u++){
         smthdAmp1List.append(Amp1List[u]);
    }

    filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);
    emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

    // Phase Recognition can be started from now on
    phaseRecogActive = true;

    // First datapoint is needed for axis range in data plot
    double frst_t = TimeList.rbegin()[0];
    double frst_A = Amp1List.rbegin()[0];
    emit CreateChart(frst_t, frst_A, smthdAmp1List.rbegin()[0]);

    logStream << "fc" << fc << "fs" << fs << "a"<< aValue << "\n";
}

void SurfaceData::extremum_search(QList<double>* data, QList<int>* max_ind_list, QList<int>* min_ind_list, bool* maxIsLast){
// Find all significant local extrema in measured data without fallbacks

    bool is_max;
    bool is_min;
    bool search_max = true;
    int search_range = 10;
    int q = search_range;
    int q_max;
    int q_min;
    double a_diff;
    double a_min_diff = 0.6;
    bool skip;
    int num_of_extr = 0;

    while(q <= data->size()-1){
        if(search_max){
            skip = false;
            is_max = false;
            q_max = q - search_range;

            // inspect if q_max is a local max
            if(data->at(q_max) > data->at(q)){
                for(int dq = 1; dq < search_range; ++dq){

                    if(data->at(q_max) > data->at(q-dq)){

                        a_diff = abs(data->at(q_max) - data->at(q-dq));
                        if(a_diff >= a_min_diff){
                            is_max = true;
                        }
                    }
                    else{
                        is_max = false;
                        ++q;
                        skip = true;
                        break;
                    }
                }

                while(!skip && !is_max && q < data->size()-1){
                    ++q;

                    if(data->at(q_max) > data->at(q)){
                        a_diff = abs(data->at(q_max) - data->at(q));

                        if(a_diff >= a_min_diff){
                            is_max = true;
                        }
                    }
                    else {
                        q = q + search_range;
                        break;
                    }
                }

                if(is_max){
                    max_ind_list->push_back(q_max);
                    *maxIsLast = true;
                    ++ num_of_extr;

                    is_max = false;
                    search_max = false;
                    ++q;
                }
            }

            else{
                ++q;
                //continue;
            }
        }

        if(!search_max){
            skip = false;
            is_min = false;
            q_min = q - search_range;

            if(data->at(q_min) < data->at(q)){

                for(int dq = 1; dq < search_range; ++dq){

                    if(data->at(q_min) < data->at(q-dq)){

                        a_diff = abs(data->at(q_min) - data->at(q-dq));
                        if(a_diff >= a_min_diff){
                            is_min = true;
                        }
                    }
                    else{
                        is_min = false;
                        ++q;
                        skip = true;
                        break;
                    }
                }

                while(!skip && !is_min && q < data->size()-1){
                    ++q;

                    if(data->at(q_min) < data->at(q)){
                        a_diff = abs(data->at(q_min) - data->at(q));

                        if(a_diff >= a_min_diff){
                            is_min = true;
                        }
                    }
                    else {
                        q = q + search_range;
                        break;
                    }
                }

                if(is_min){
                    min_ind_list->push_back(q_min);
                    *maxIsLast = false;
                    ++ num_of_extr;

                    is_min = false;
                    search_max = true;
                    ++q;
                }
            }

            else{
                ++q;
                //continue;
            }
        }
    }
}

void SurfaceData::ReadIter(){
    if (iteration <= windowSize){
        readOnly();

        // Emit signal to update time and ampl values in UI
        emit LnReadingFinished(TimeList.rbegin()[0], Amp1List.rbegin()[0]);

        //PhaseList.append(0);
        iteration = iteration + 1;

        //Write log file
        logStream << TimeList.rbegin()[0] << "\n";
    }
    else{
        disconnect(IterTimer, &QTimer::timeout, this, &SurfaceData::ReadIter);
        connect(IterTimer, &QTimer::timeout, this, &SurfaceData::ReadFiltIter);
        setFilterParams();
        ReadFiltIter();
    }
}

void SurfaceData::ReadFiltIter(){
    if (!PhaseRecogStarted){
        readOnly();

        // Emit signal to update time and ampl values in UI
        emit LnReadingFinished(TimeList.rbegin()[0], Amp1List.rbegin()[0]);

        filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);
        emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

        //Write log file
        logStream << TimeList.rbegin()[0] << "\n";
    }
    else{
        // -------------PREPARE FOR PHASE RECOGNITION----------------------------------------------------------------------

        // Find all extrema in data that have been measured until now and save them into max and min list
        extremum_search(&smthdAmp1List, &maxIndList, &minIndList, &maxIsLast);
        newMaxInd = maxIndList.rbegin()[0];
        oldMaxInd = maxIndList.rbegin()[1];
        lastMinInd = minIndList.rbegin()[0];

        // Find last breath cycle duration and phase duration
        T = TimeList[newMaxInd] - TimeList[oldMaxInd];
        dT = T /10.0;

        if (newMaxInd > lastMinInd){
            maxIsLast = true;
        }
        else {
            maxIsLast = false;
        }

        dAcheck = abs(smthdAmp1List[newMaxInd]-smthdAmp1List[lastMinInd])/4;

        // -------------CALCULATE X AXIS SHIFT CAUSED BY ONE SIDED FILTERING-----------------------------------------------

        // Use Moving Average Filter to smooth data
        int halfWindowSize = windowSize / 2;
        double intervalMean;
        QList<double> smthdMovAvgList = Amp1List.mid(0, halfWindowSize);
        for(int i = halfWindowSize; i < Amp1List.size()-halfWindowSize; i++){
            intervalMean = 0;
            for(int j = i - halfWindowSize; j <= i + halfWindowSize; j++){
                intervalMean = intervalMean + Amp1List[j];
            }
            smthdMovAvgList.append(intervalMean / windowSize);
        }

        // Find last maxima
        QList<int> MovAvgMaxIndList;
        QList<int> MovAvgMinIndList;
        bool MovAvgMaxIsLast;
        extremum_search(&smthdMovAvgList, &MovAvgMaxIndList, &MovAvgMinIndList, &MovAvgMaxIsLast);

        // Calculate shift
        double shiftSum = 0;
        double diff;
        if (MovAvgMaxIndList.size() < 3 || maxIndList.size() < 3) {
            logStream << "no3PeaksForPR";
            DBG_ERROR("Error: FIRE requires three maxima for starting phase recognition.");
        }
        for (int l = 0; l < 3; l++){
            diff = qFabs(TimeList[MovAvgMaxIndList.rbegin()[l]] - TimeList[maxIndList.rbegin()[l]]);
            shiftSum = shiftSum + diff;
        }
        filterShift = shiftSum / 3.0;
        correctedNewMaxTime = TimeList[newMaxInd] - filterShift;

        logStream << "PRS:" << TimeList[oldMaxInd] << ";" << TimeList[lastMinInd]
                        << ";" << TimeList[newMaxInd] << ";" << filterShift << "\n";

        if (uiPtr->DebugModeRadioButton->isChecked()){
            disconnect(IterTimer, &QTimer::timeout, this, &SurfaceData::ReadFiltIter);
            sizeBeforePR = TimeList.size();
            connect(IterTimer, &QTimer::timeout, this, &SurfaceData::ReadFiltPhaseDebugIter);
            ReadFiltPhaseDebugIter();
        }
        else{
            disconnect(IterTimer, &QTimer::timeout, this, &SurfaceData::ReadFiltIter);
            connect(IterTimer, &QTimer::timeout, this, &SurfaceData::ReadFiltPhasePerformIter);
            ReadFiltPhasePerformIter();
        }
    }
}

void SurfaceData::ReadFiltPhaseDebugIter(){

    if (!f.atEnd()){
        readOnly();

        // Emit signal to update time and ampl values in UI
        emit LnReadingFinished(TimeList.rbegin()[0], Amp1List.rbegin()[0]);

        filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);

        emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

        // Check if there is a new max
        if (newMaxDetected(&smthdAmp1List, &maxIsLast, &lastMinInd, &newMaxInd)){

            // Update indices of last 2 max
            oldMaxInd = newMaxInd;
            newMaxInd = TimeList.size()-3;

            // Update time of newest (T) and last (T_old) breathing cycle
            T_old = T;
            T = abs(TimeList[newMaxInd] - TimeList[oldMaxInd]);

            // Valid amplitude interval around datapoint
            dAcheck = abs(smthdAmp1List[newMaxInd]-smthdAmp1List[lastMinInd])/4;

            if (!checkBreathCycleDur(&T, &T_planning, &T_old, &phaseRecogActive)){
                // Fallback for significant changes in duration of breathing cycle
                phase = 0;
            }
            else {
                // Update phase borders
                dT = T /10.0;
                correctedNewMaxTime = TimeList[newMaxInd] - filterShift;
            }
        }

        // Do phase recognition
        if (phaseRecogActive){
            localInd = smthdAmp1List.size() - newMaxInd;
            phaseRecognition(&TimeList.rbegin()[0], &correctedNewMaxTime, &dT, &phase, &smthdAmp1List.rbegin()[0],
                    &smthdAmp1List[oldMaxInd+localInd], &dAcheck, &phaseRecogActive);
        }

        emit showCurrentPhase(&phase);

        PhaseList.append(phase);

        //Write log file
        logStream << TimeList.rbegin()[0] << ";" << phase << "\n";

    }

    else{
        logFile.close();

        //Create csv file with measured phase for each time value
        for (int l = 0; l < sizeBeforePR; ++l) {
                PhaseList.prepend(0);
            }
        if(!createOutputFile(&TimeList, &PhaseList)){
            logStream << "Error in SurfaceData::run(). Writing csv file was not successful";
        }
        else{
            qDebug() << "Writing csv file was successfull";
        }

        IterTimer->stop();
    }
}

void SurfaceData::ReadFiltPhasePerformIter(){
    if (!f.atEnd()){
        timer.start();

        readOnly();

        filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);

        // Check if there is a new max
        if (newMaxDetected(&smthdAmp1List, &maxIsLast, &lastMinInd, &newMaxInd)){

            // Update indices of last 2 max
            oldMaxInd = newMaxInd;
            newMaxInd = TimeList.size()-3;

            // Update time of newest (T) and last (T_old) breathing cycle
            T_old = T;
            T = abs(TimeList[newMaxInd] - TimeList[oldMaxInd]);

            // Minimum amplitude difference
            dAcheck = abs(smthdAmp1List[newMaxInd]-smthdAmp1List[lastMinInd])/4;

            if (!checkBreathCycleDur(&T, &T_planning, &T_old, &phaseRecogActive)){
                // Fallback for significant changes in duration of breathing cycle
                phase = 0;
            }
            else {
                // Update phase borders
                dT = T /10.0;
                correctedNewMaxTime = TimeList[newMaxInd] - filterShift;
            }
        }

        // Do phase recognition
        if (phaseRecogActive){
            localInd = smthdAmp1List.size() - newMaxInd;
            phaseRecognition(&TimeList.rbegin()[0], &correctedNewMaxTime, &dT, &phase, &smthdAmp1List.rbegin()[0],
                    &smthdAmp1List[oldMaxInd+localInd], &dAcheck, &phaseRecogActive);
        }

        //Write log file
        logStream << TimeList.rbegin()[0] << ";" << phase << "\n";

        elapsed = timer.nsecsElapsed();

        // Save longest runtime
        double elapsedDouble = static_cast<double>(elapsed);
        if (elapsedDouble > maxElapsed){
            maxElapsed = elapsedDouble;
        }

        elapsedList.insert_rows(elapsedList.n_elem, 1);
        elapsedList(elapsedList.n_elem - 1) = elapsedDouble;

//            if (elapsedDouble >= 5000000){
//                qDebug() << elapsedDouble << " ms at " << TimeList.rbegin()[0];
//            }

        //qDebug() << "runtime: " << elapsedDouble << " ms";
    }
    else{
        // Display longest runtime
        qDebug() << "Longest runtime: " << maxElapsed << " ns";
        logStream << "tMax" << maxElapsed << "ns;tMean" << arma::mean(elapsedList) << "ns;tStd" << arma::stddev(elapsedList) << "ns";
        logFile.close();
        IterTimer->stop();
    }
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------




void SurfaceData::run(){

}





//    //--------Reading only-------------------------------------------------------------------------------------

//    while (iteration <= windowSize){

//        readOnly();

//        // Emit signal to update time and ampl values in UI
//        emit LnReadingFinished(TimeList.rbegin()[0], Amp1List.rbegin()[0]);

//        PhaseList.append(0);
//        iteration = iteration + 1;

//        elapsed = timer.elapsed();
//        double elapsedDouble = static_cast<double>(elapsed);

//        // Sleep for certain time to simulate dynamical reading
//        msleep(sleepingTime-elapsedDouble);

//        //Write log file
//        logStream << TimeList.rbegin()[0] << "\n";
//    }


//    //--------Set parameters for filtering---------------------------------------------------------------------

//    setFilterParams();

//    //--------Reading and filtering----------------------------------------------------------------------------
//    while (!PhaseRecogStarted){
//        readOnly();

//        // Emit signal to update time and ampl values in UI
//        emit LnReadingFinished(TimeList.rbegin()[0], Amp1List.rbegin()[0]);

//        PhaseList.append(0);

//        filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);
//        emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

//        elapsed = timer.elapsed();
//        double elapsedDouble = static_cast<double>(elapsed);

//        // Sleep for certain time to simulate dynamical reading
//        msleep(sleepingTime-elapsedDouble);

//        //Write log file
//        logStream << TimeList.rbegin()[0] << "\n";
//    }

//    // -------------PREPARE FOR PHASE RECOGNITION----------------------------------------------------------------------

//    // Find all extrema in data that have been measured until now and save them into max and min list
//    extremum_search(&smthdAmp1List, &maxIndList, &minIndList, &maxIsLast);
//    newMaxInd = maxIndList.rbegin()[0];
//    oldMaxInd = maxIndList.rbegin()[1];
//    lastMinInd = minIndList.rbegin()[0];

//    // Find last breath cycle duration and phase duration
//    T = TimeList[newMaxInd] - TimeList[oldMaxInd];
//    dT = T /10.0;

//    if (newMaxInd > lastMinInd){
//        maxIsLast = true;
//    }
//    else {
//        maxIsLast = false;
//    }

//    dAcheck = abs(smthdAmp1List[newMaxInd]-smthdAmp1List[lastMinInd])/2;

//    // -------------CALCULATE X AXIS SHIFT CAUSED BY ONE SIDED FILTERING-----------------------------------------------

//    // Use Moving Average Filter to smooth data
//    int halfWindowSize = windowSize / 2;
//    double intervalMean;
//    QList<double> smthdMovAvgList = Amp1List.mid(0, halfWindowSize);
//    for(int i = halfWindowSize; i < Amp1List.size()-halfWindowSize; i++){
//        intervalMean = 0;
//        for(int j = i - halfWindowSize; j <= i + halfWindowSize; j++){
//            intervalMean = intervalMean + Amp1List[j];
//        }
//        smthdMovAvgList.append(intervalMean / windowSize);
//    }

//    // Find last maxima
//    QList<int> MovAvgMaxIndList;
//    QList<int> MovAvgMinIndList;
//    bool MovAvgMaxIsLast;
//    extremum_search(&smthdMovAvgList, &MovAvgMaxIndList, &MovAvgMinIndList, &MovAvgMaxIsLast);

//    // Calculate shift
//    double shiftSum = 0;
//    double diff;
//    for (int l = 0; l < 3; l++){
//        diff = qFabs(smthdMovAvgList[MovAvgMaxIndList.rbegin()[l]] - smthdAmp1List[maxIndList.rbegin()[l]]);
//        shiftSum = shiftSum + diff;
//    }
//    filterShift = shiftSum / 3.0;
//    correctedNewMaxTime = TimeList[newMaxInd] - filterShift;

//    logStream << "PRS:" << TimeList[oldMaxInd] << ";" << TimeList[lastMinInd]
//                    << ";" << TimeList[newMaxInd] << ";" << filterShift << "\n";



//    //--------Reading, filtering and phase recognition---------------------------------------------------------

//    if (uiPtr->DebugModeRadioButton->isChecked()){

//        // Debug mode
//        while (!f.atEnd()){

//            readOnly();

//            // Emit signal to update time and ampl values in UI
//            emit LnReadingFinished(TimeList.rbegin()[0], Amp1List.rbegin()[0]);

//            filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);

//            emit FilteringFinished(TimeList.rbegin()[0], smthdAmp1List.rbegin()[0]);

//            // Check if there is a new max
//            if (newMaxDetected(&smthdAmp1List, &maxIsLast, &lastMinInd, &newMaxInd)){

//                // Update indices of last 2 max
//                oldMaxInd = newMaxInd;
//                newMaxInd = TimeList.size()-3;

//                // Update time of newest (T) and last (T_old) breathing cycle
//                T_old = T;
//                T = abs(TimeList[newMaxInd] - TimeList[oldMaxInd]);

//                // Valid amplitude interval around datapoint
//                dAcheck = abs(smthdAmp1List[newMaxInd]-smthdAmp1List[lastMinInd])/2;

//                if (!checkBreathCycleDur(&T, &T_planning, &T_old, &phaseRecogActive)){
//                    // Fallback for significant changes in duration of breathing cycle
//                    phase = 0;
//                }
//                else {
//                    // Update phase borders
//                    dT = T /10.0;
//                    correctedNewMaxTime = TimeList[newMaxInd] - filterShift;
//                }

//                //Only for validating phase recognition
//                PhaseList.append(IntervalPhaseList);
//                IntervalPhaseList.clear();
//            }

//            // Do phase recognition
//            if (phaseRecogActive){
//                localInd = smthdAmp1List.size() - newMaxInd;
//                phaseRecognition(&TimeList.rbegin()[0], &correctedNewMaxTime, &dT, &phase, &smthdAmp1List.rbegin()[0],
//                        &smthdAmp1List[oldMaxInd+localInd], &dAcheck, &phaseRecogActive);
//            }

//            emit showCurrentPhase(&phase);

//            elapsed = timer.elapsed();
//            double elapsedDouble = static_cast<double>(elapsed);

//            // Sleep for certain time to simulate dynamical reading
//            msleep(sleepingTime-elapsedDouble);

//            IntervalPhaseList.append(phase);

//            //Write log file
//            logStream << TimeList.rbegin()[0] << ";" << phase << "\n";
//        }

//        // Create csv file with measured phase for each time value
//        PhaseList.append(IntervalPhaseList);
//        if(!createOutputFile()){
//            logStream << "Error in SurfaceData::run(). Writing csv file was not successful";
//        }
//        else{
//            qDebug() << "Writing csv file was successfull";
//        }
//    }

//    // Performance mode
//    else{

//        while (!f.atEnd()){
//            timer.start();

//            readOnly();

//            filterNewDatapoint(&Amp1List, &smthdAmp1List, &aValue);

//            // Check if there is a new max
//            if (newMaxDetected(&smthdAmp1List, &maxIsLast, &lastMinInd, &newMaxInd)){

//                // Update indices of last 2 max
//                oldMaxInd = newMaxInd;
//                newMaxInd = TimeList.size()-3;

//                // Update time of newest (T) and last (T_old) breathing cycle
//                T_old = T;
//                T = abs(TimeList[newMaxInd] - TimeList[oldMaxInd]);

//                // Minimum amplitude difference
//                dAcheck = abs(smthdAmp1List[newMaxInd]-smthdAmp1List[lastMinInd])/4;

//                if (!checkBreathCycleDur(&T, &T_planning, &T_old, &phaseRecogActive)){
//                    // Fallback for significant changes in duration of breathing cycle
//                    phase = 0;
//                }
//                else {
//                    // Update phase borders
//                    dT = T /10.0;
//                    correctedNewMaxTime = TimeList[newMaxInd] - filterShift;
//                }
//            }

//            // Do phase recognition
//            if (phaseRecogActive){
//                localInd = smthdAmp1List.size() - newMaxInd;
//                phaseRecognition(&TimeList.rbegin()[0], &correctedNewMaxTime, &dT, &phase, &smthdAmp1List.rbegin()[0],
//                        &smthdAmp1List[oldMaxInd+localInd], &dAcheck, &phaseRecogActive);
//            }

//            elapsed = timer.elapsed();

//            // Save longest runtime
//            double elapsedDouble = static_cast<double>(elapsed);
//            if (elapsedDouble > maxElapsed){
//                maxElapsed = elapsedDouble;
//            }
////            if (elapsedDouble >= 5000000){
////                qDebug() << elapsedDouble << " ns at " << TimeList.rbegin()[0];
////            }

//            //qDebug() << "runtime: " << elapsedDouble << " ns";

//            // Sleep for certain time to simulate dynamical reading
//            msleep(sleepingTime-elapsedDouble);

//            //Write log file
//            logStream << TimeList.rbegin()[0] << ";" << phase << "\n";
//        }

//        // Display longest runtime
//        qDebug() << "Longest runtime: " << maxElapsed << " ns";
//        logStream << "tMax" << maxElapsed << "ns";
//    }

//    qDebug() << "Letzter Zeit Wert: " << TimeList.rbegin()[0];
//    //logFile.close();




