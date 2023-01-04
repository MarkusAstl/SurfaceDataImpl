#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include <QtMath>
#include <QTextStream>
#include <QString>
#include <armadillo>

// Surface data reading
#include <surfacedata.h>

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

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // reading surface data dynamically during treatment
    rThread = new SurfaceData(this);    // SurfaceData is a QThread object

    // loading surface data from planning CT
    lThread = new SurfaceData(this);

    // Updates time and ampl values in UI after a new line was read
    connect(rThread,SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(onLnReadingFinished(QStringList, int)));
    connect(lThread,SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(onLoadingFinished(QStringList, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLnReadingFinished(QStringList SplitLn, int colNum)
{
    // Update time value in UI
    QString timeVal = SplitLn[0];
    ui->timeLabel_4->setText(timeVal);

    // Define amp1 value
    QString amp1Val = SplitLn[1];

    // Delete regex expression at the end if amp1 is last value
    if(colNum == 2){
        amp1Val.remove(amp1Val.size()-4, amp1Val.size()-1);
    }

    // Update amp1 value in UI
    ui->amp1Label_4->setText(amp1Val);

    // Delete regex expression at the end if amp2 is last value
    if(colNum == 3){
        QString amp2Val = SplitLn[2];

        // Update amp2 value in UI if there is one
        ui->amp2Label_4->setText(amp2Val);
    }
}

void MainWindow::onLoadingFinished(QStringList SplitLn, int colNum)
{
    // Update time value in UI
    QString timeValAvg = SplitLn[0];
    ui->timeLabelAvg_4->setText(timeValAvg);

    // Define amp1 value
    QString amp1ValAvg = SplitLn[1];

    // Delete regex expression at the end if amp1 is last value
    if(colNum == 2){
        amp1ValAvg.remove(amp1ValAvg.size()-4, amp1ValAvg.size()-1);
    }

    // Update amp1 value in UI
    ui->amp1LabelAvg_4->setText(amp1ValAvg);

    // Delete regex expression at the end if amp2 is last value
    if(colNum == 3){
        QString amp2ValAvg = SplitLn[2];

        // Update amp2 value in UI if there is one
        ui->amp2LabelAvg_4->setText(amp2ValAvg);
    }
}


void MainWindow::on_StartReadingButton_4_clicked()
{
    // Define path of real time data file
    rThread->path = MainWindow::path2;

    // Read data with defined delay
    rThread->sleepingTime = MainWindow::dtSurfData;

    // Start reading of data
    rThread->start();                                   // starts SurfaceData::run() for rThread

    // Create dialog window to show real time and planning CT data
    chartDialog = new ChartDialog(this);        // Define new QDialog object; "this" points at MainWindow as a parent
    chartDialog->setDisabled(false);            // enables input events

    // Create and adjust dynamic chart
    dynamicChart = new DynamicChart(0, 0, rThread);     // QChart object; starts constructor of DynamicChart
    dynamicChart->setTitle("Real time data");
    dynamicChart->setFont(QFont("sans", 10, QFont::Bold));
    dynamicChart->legend()->hide();
    dynamicChart->setAnimationOptions(QChart::AllAnimations);      // Enable 'AllAnimations' for the chart

    // Create QChartView widget which displays the dynamic chart
    QChartView *dynamicChartView = new QChartView(dynamicChart);
    dynamicChartView->setRenderHint(QPainter::Antialiasing);       // Enable render hint 'Antialiasing' for chart view

    // Add static and dynamic chart into one dialog window
    chartDialog->setLayout(new QVBoxLayout());
    chartDialog->layout()->addWidget(dynamicChartView);     // add dynamic chartView into the layout
    chartDialog->layout()->addWidget(staticChart);      // add static chartView
    chartDialog->resize(700, 500);

    // Show dialog window
    chartDialog->show();
}

void MainWindow::on_StopReadingButton_4_clicked()
{
    // Stop reading of real time data
    rThread->Stop = true;
}

void MainWindow::on_LoadingButton_4_clicked()
{    
    // Create chart for UI showing data from planning CT
    staticChart = new QCustomPlot();
    staticChart->setFixedSize(700,210);

    // Add data from file to chart as soon as reading is finished
    QObject::connect(lThread, SIGNAL(DataReadingFinished(QList<double>*, QList<double>*, QList<double>*, int)),
                     this, SLOT(addAvgData(QList<double>*, QList<double>*, QList<double>*, int)));

    // Define path of planning CT file
    lThread->path = MainWindow::path1;

    // Read data with defined delay
    lThread->sleepingTime = 0;

    // Start reading of data
    lThread->start();                       // starts SurfaceData::run() for lThread
}

void MainWindow::on_StopLoadingButton_4_clicked()
{
    // Stop reading of planning CT data
    lThread->Stop = true;
}

void MainWindow::addAvgData(QList<double>* TimeListPtr, QList<double>* Amp1ListPtr, QList<double>* Amp2ListPtr, int colNum)
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
        StatTimeVec[i] = (StatTimeVec[i]-qStart)/1000;      // [t] = 1 ms -> 1 s & t starts with 0
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
    staticChart->xAxis->setRange(0, xStatAxisRange);
    staticChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    staticChart->yAxis->setRange(amp1Min-(amp1Max-amp1Min)*0.15, amp1Max+(amp1Max-amp1Min)*0.3);
    staticChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    staticChart->replot();
}


void MainWindow::extremum_search(QList<double>* data, QList<int>* max_ind_list, QList<int>* min_ind_list, bool* maxIsLast){

    bool is_max;
    bool is_min;
    bool search_max = true;
    int search_range = 20;
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
    QList<int> maxIndList;
    QList<int> minIndList;
    bool maxIsLast;
    extremum_search(&rThread->smthdAmp1List, &maxIndList, &minIndList, &maxIsLast);
    QList<double> phaseBorderTimes = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    int peak2Ind;
    int peak1Ind;
    int peak0Ind;

    if (maxIsLast == true){
        peak2Ind = maxIndList.rbegin()[0];
        peak1Ind = minIndList.rbegin()[0];
        peak0Ind = maxIndList.rbegin()[1];

    }
    else{
        peak2Ind = minIndList.rbegin()[0];
        peak1Ind = maxIndList.rbegin()[0];
        peak0Ind = minIndList.rbegin()[1];
    }

    QList<int> BorderTimes = {0,0,0,0,0,0,0,0,0,0};
    calcPhaseBorders(&rThread->smthdAmp1List, &BorderTimes, &peak0Ind, &peak1Ind);
    calcPhaseBorders(&rThread->smthdAmp1List, &BorderTimes, &peak1Ind, &peak2Ind);
}

void MainWindow::calcPhaseBorders(QList<double>* data, QList<int>* BorderTimes, int* A1ind, int* A2ind){

    double dA = data->at(*A1ind) - data->at(*A2ind);
    double dAphase = dA / 5;

    QList<int> newBorderTimes(BorderTimes->mid(5,9));

    int phase = 1;
    int i = *A1ind;

    while(i < *A2ind && phase <= 4){
        double dA_iteration = abs(data->at(i) - data->at(*A1ind));
        if (dA_iteration > abs(phase*dAphase)){
            newBorderTimes.append(i-1);
            phase = phase + 1;
        }

        i = i+1;
    }

    newBorderTimes.append(*A2ind);
    *BorderTimes = newBorderTimes;
}

