//16:08

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include <QtMath>
#include <QTextStream>
#include <QString>
#include <armadillo>
#include <QLocale>

// Surface data reading
#include <surfacedata.h>
#include <loadingthread.h>

#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <new>

#include <chrono>
#include <thread>

// Surface data chart
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "qcustomplot.h"
#include <QLayout>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // reading surface data dynamically during treatment
    rThread = new SurfaceData(this);    // SurfaceData is a QThread object

    // loading surface data from planning CT
    lThread = new LoadingThread(this);

    // Updates phase in UI
    connect(rThread,SIGNAL(showCurrentPhase(int*)), this, SLOT(phaseChanged(int*)));

    // Show initial cutoff frequency
    QString fc_text = QString::number(rThread->fc) + " Hz";
    ui->fc_label->setText(fc_text);
    int start_fc = rThread->fc * 100;
    ui->fc_slider->setSliderPosition(start_fc);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CreateChart(double frst_t, double frst_A, double frstFilt_A)
{
    // Create dialog window to show real time and planning CT data
    chartDialog = new ChartDialog(this);        // Define new QDialog object; "this" points at MainWindow as a parent
    chartDialog->setDisabled(false);            // enables input events

    // Create and adjust dynamic chart
    dynamicChart = new DynamicChart(0, 0, rThread, &frst_t, &frst_A);     // QChart object; starts constructor of DynamicChart
    dynamicChart->setTitle("Measured Data");
    dynamicChart->setTitleFont(QFont("sans", 10, QFont::Bold));
    dynamicChart->legend()->hide();
    dynamicChart->setAnimationOptions(QChart::AllAnimations);      // Enable 'AllAnimations' for the chart

    // Create QChartView widget which displays the dynamic chart
    QChartView *dynamicChartView = new QChartView(dynamicChart);
    dynamicChartView->setRenderHint(QPainter::Antialiasing);       // Enable render hint 'Antialiasing' for chart view

    // If a new line was read the data series of the graph will be updated and x axis will be scrolled if necessary
    QObject::connect(rThread, SIGNAL(LnReadingFinished(QStringList)), dynamicChart, SLOT(addDataPoint(QStringList)));

    //---------------------------------------------------------------------------------------------------------
    // Plot filtered data
    filterChart = new DynamicChart(0, 0, rThread, &frst_t, &frstFilt_A);
    filterChart->setTitle("Filtered Data");
    filterChart->setTitleFont(QFont("sans", 10, QFont::Bold));
    filterChart->legend()->hide();
    filterChart->setAnimationOptions(QChart::AllAnimations);
    QChartView *filterChartView = new QChartView(filterChart);
    filterChartView->setRenderHint(QPainter::Antialiasing);
    QObject::connect(rThread, SIGNAL(FilteringFinished(double, double)), filterChart, SLOT(addFilteredDataPoint(double, double)));
    connect(this,SIGNAL(adjustYaxis(double*)), filterChart, SLOT(on_adjustYaxis(double*)));
    //---------------------------------------------------------------------------------------------------------

    // Add static and dynamic chart into one dialog window
    chartDialog->setLayout(new QVBoxLayout());
    chartDialog->layout()->addWidget(dynamicChartView);     // add dynamic chartView into the layout
    chartDialog->layout()->addWidget(filterChartView);      // add filter chartView
    chartDialog->layout()->addWidget(staticChart);      // add static chartView
    chartDialog->resize(1000, 750);


    // Show dialog window
    chartDialog->show();
}


void MainWindow::on_StartReadingButton_clicked()
{
    ui->StartReadingButton->setText("Reading..");
    ui->StartReadingButton->setEnabled(false);
    ui->StopReadingButton->setEnabled(true);
    ui->StartPhaseRecognitionButton->setEnabled(true);

    // Define path of real time data file
    rThread->path = MainWindow::rPath;

    // Start reading of data
    rThread->filtering = true;                          // filter data in real time and enable phase recognition
    rThread->start();                                   // starts SurfaceData::run() for rThread

    // As soon as first datapoint was filtered create the data chart
    connect(rThread,SIGNAL(CreateChart(double, double, double)), this, SLOT(on_CreateChart(double, double, double)));
}

void MainWindow::phaseChanged(int* phase)
{
    QString phaseLabelStr = QString::number(*phase);
    ui->phaseLabel->setText(phaseLabelStr);
}

void MainWindow::on_StopReadingButton_clicked()
{
    // Stop reading of real time data
    rThread->fout.close();
    rThread->terminate();
}

void MainWindow::on_LoadingButton_clicked()
{    
    ui->LoadingButton->setText("Loading..");
    ui->StopLoadingButton->setEnabled(true);

    lThread->path = MainWindow::lPath;

    // Create chart for UI showing data from planning CT
    staticChart = new QCustomPlot();
    staticChart->setFixedSize(950,210);

    // Set points as decimal separator
    staticChart->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // Add data from file to chart as soon as reading is finished
    QObject::connect(lThread, SIGNAL(DataLoadingFinished(QList<double>*, QList<double>*)),
                     this, SLOT(addAvgData(QList<double>*, QList<double>*)));

    // Adjust UI
    QObject::connect(lThread, SIGNAL(LoadingStopped()), this, SLOT(on_LoadingStopped()));

    // Start reading of data
    lThread->start();                       // starts SurfaceData::run() for lThread
}

void MainWindow::on_LoadingStopped(){
    ui->LoadingButton->setText("Loaded");
    ui->StopLoadingButton->setEnabled(false);
    ui->StartReadingButton->setEnabled(true);
    ui->fc_slider->setEnabled(true);
}

void MainWindow::on_StopLoadingButton_clicked()
{
    on_LoadingStopped();

    // Stop reading of planning CT data
    lThread->terminate();
}

void MainWindow::addAvgData(QList<double>* TimeListPtr, QList<double>* Amp1ListPtr)
{
    // Set chart title
    QCPTextElement *staticTitle = new QCPTextElement(staticChart);
    staticTitle->setText("Average Surface Data");
    staticTitle->setFont(QFont("sans", 10, QFont::Bold));
    staticChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    staticChart->plotLayout()->addElement(0, 0, staticTitle); // place the title in the empty cell we've just created

    // Set Pen
    QPen stPen;
    stPen.setWidth(1);
    stPen.setColor(Qt::blue);
    staticChart->addGraph();
    staticChart->graph(0)->setPen(stPen);

    // Convert data to requested type
    QVector<double> StatTimeVec = TimeListPtr->toVector();
    QVector<double> StatAmp1Vec = Amp1ListPtr->toVector();

    // Adjust time values
    double qStart = StatTimeVec[0];
    for(int i = 0; i<StatTimeVec.size(); ++i){
        StatTimeVec[i] = (StatTimeVec[i]-qStart);      // t starts with 0
    }

    // Add data to graph
    staticChart->graph(0)->setData(StatTimeVec, StatAmp1Vec);

    // Define position parameters for layout
    double amp1Min = *std::min_element(StatAmp1Vec.begin(), StatAmp1Vec.end());
    double amp1Max = *std::max_element(StatAmp1Vec.begin(), StatAmp1Vec.end());
    qreal xStatAxisRange = StatTimeVec.back() - StatTimeVec[0];

    // Add vertical line and phase number
    QPen borderPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    borderPen.setWidth(1);
    qreal dPhase = xStatAxisRange/10;
    for(int veLnInd = 0; veLnInd <= 9; ++veLnInd){
        QCPItemStraightLine *statBorderLine = new QCPItemStraightLine(staticChart);
        statBorderLine->point1->setCoords(dPhase*veLnInd, 1);  // location (x,y) of point 1 in plot coordinate
        statBorderLine->point2->setCoords(dPhase*veLnInd, 2);  // location (x,y) of point 2 in plot coordinate
        statBorderLine->setPen(borderPen);
        QCPItemText *textLabel = new QCPItemText(staticChart);
        textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
        textLabel->position->setCoords(dPhase*veLnInd+dPhase*0.5, amp1Max+(amp1Max-amp1Min)*0.2); // place position at center/top of axis rect
        textLabel->setText(QString::number(veLnInd+1, 10));
        textLabel->setFont(QFont(font().family(), 8));
    }

    // Adjust Axes
    staticChart->xAxis->setLabel("t [s]");
    staticChart->yAxis->setLabel("Amp. [mm]");
    staticChart->xAxis->setRange(0.0, xStatAxisRange);
    staticChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    staticChart->yAxis->setRange(amp1Min-(amp1Max-amp1Min)*0.15, amp1Max+(amp1Max-amp1Min)*0.3);
    staticChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    staticChart->replot();
}


void MainWindow::extremum_search(QList<double>* data, QList<int>* max_ind_list, QList<int>* min_ind_list, bool* maxIsLast){

    bool is_max;
    bool is_min;
    bool search_max = true;
    int search_range = 10;
    int q = search_range;
    int q_max;
    int q_min;
    double a1_diff;
    double a1_min_diff = 0.6;
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

                        a1_diff = abs(data->at(q_max) - data->at(q-dq));
                        if(a1_diff >= a1_min_diff){
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
                        a1_diff = abs(data->at(q_max) - data->at(q));

                        if(a1_diff >= a1_min_diff){
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

                        a1_diff = abs(data->at(q_min) - data->at(q-dq));
                        if(a1_diff >= a1_min_diff){
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
                        a1_diff = abs(data->at(q_min) - data->at(q));

                        if(a1_diff >= a1_min_diff){
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

void MainWindow::on_StartPhaseRecognitionButton_clicked()
{
    ui->StartPhaseRecognitionButton->setEnabled(false);

    // -------------PREPARE FOR PHASE RECOGNITION----------------------------------------------------------------------

    // Find all extrema in data that have been measured until now and save them into max and min list
    extremum_search(&rThread->smthdAmp1List, &rThread->maxIndList, &rThread->minIndList, &rThread->maxIsLast);
    rThread->newMaxInd = rThread->maxIndList.rbegin()[0];
    rThread->oldMaxInd = rThread->maxIndList.rbegin()[1];
    rThread->lastMinInd = rThread->minIndList.rbegin()[0];

    // Find last breath cycle duration and phase duration
    rThread->T = rThread->TimeList[rThread->newMaxInd] - rThread->TimeList[rThread->oldMaxInd];
    rThread->dT = rThread->T /10.0;

    if (rThread->newMaxInd > rThread->lastMinInd){
        rThread->maxIsLast = true;
    }
    else {
        rThread->maxIsLast = false;
    }

    // -------------CALCULATE X AXIS SHIFT CAUSED BY FILTERING---------------------------------------------------------

    // Use Moving Average Filter to smooth data
    int halfWindowSize = rThread->windowSize / 2;
    double intervalMean;
    QList<double> smthdMovAvgList = rThread->Amp1List.mid(0, halfWindowSize);
    for(int i = halfWindowSize; i < rThread->Amp1List.size()-halfWindowSize; i++){
        intervalMean = 0;
        for(int j = i - halfWindowSize; j <= i + halfWindowSize; j++){
            intervalMean = intervalMean + rThread->Amp1List[j];
        }
        smthdMovAvgList.append(intervalMean / rThread->windowSize);
    }

    // Find last maxima
    QList<int> MovAvgMaxIndList;
    QList<int> MovAvgMinIndList;
    bool MovAvgMaxIsLast;
    extremum_search(&smthdMovAvgList, &MovAvgMaxIndList, &MovAvgMinIndList, &MovAvgMaxIsLast);

    // Calculate shift
    double shiftSum = 0;
    double diff;
    for (int l = 0; l < 3; l++){
        diff = qFabs(smthdMovAvgList[MovAvgMaxIndList.rbegin()[l]] - rThread->smthdAmp1List[rThread->maxIndList.rbegin()[l]]);
        shiftSum = shiftSum + diff;
    }
    rThread->filterShift = shiftSum / 3.0;
    rThread->correctedNewMaxTime = rThread->TimeList[rThread->newMaxInd] - rThread->filterShift;


    // -------------START PHASE RECOGNITION----------------------------------------------------------------------------

    // Parameters are set -> Phase recognition can be started
    rThread->PhaseRecogStarted = true;
    chartDialog->reject();

    // Dislay phase recognition starting time for test documentation
    qDebug() << rThread->TimeList.rbegin()[0];
}


void MainWindow::on_fc_slider_sliderMoved(int position)
{
    double new_fc = position / 100.0;
    int N = rThread->Amp1List.size();
    double new_Amp = 0.0;
    double convProd = 1.0;

    rThread->aValue = qSqrt( qPow( qCos(2*M_PI* new_fc/ rThread->fs), 2) - 4*qCos(2*M_PI* new_fc/ rThread->fs) + 3)
            + qCos(2*M_PI* new_fc/ rThread->fs) - 1;

    for(int convInd = 0; convInd < rThread->windowSize; ++convInd){
        convProd = rThread->aValue * (rThread->Amp1List[N-1-convInd] * qPow((1-rThread->aValue), convInd));
        new_Amp = convProd+new_Amp;
    }

    QString fc_text = QString::number(new_fc, 'f', 2);
    fc_text = fc_text + " Hz";
    ui->fc_label->setText(fc_text);
}


void MainWindow::on_SelectReadingFileButton_clicked()
{
    QString rFullPath = QFileDialog::getOpenFileName(this, "Select Surface Data", QDir::currentPath()+"/SurfaceData", "All Files (*.*)");
    if (rFullPath.isEmpty()){
        qDebug() << "Error in MainWindow::on_SelectReadingFileButton_clicked()."
                    "Selecting surface data file was not successfull.";
    }
    else {
        QFileInfo rPathInfo(rFullPath);
        rPath = rPathInfo.fileName();
    }
    ui->ReadingFileLabel->setText(rPath);
}

void MainWindow::on_SelectLoadingFileButton_clicked()
{
    QString lFullPath = QFileDialog::getOpenFileName(this, "Select 4D CT Data", QDir::currentPath()+"/SurfaceData", "All Files (*.*)");
    if (lFullPath.isEmpty()){
        qDebug() << "Error in MainWindow::on_SelectLoadingFileButton_clicked()."
                    "Selecting 4d CT data file was not successfull.";
    }
    else {
        QFileInfo lPathInfo(lFullPath);
        lPath = lPathInfo.fileName();
    }
    ui->LoadingFileLabel->setText(lPath);
}
