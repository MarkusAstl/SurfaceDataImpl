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

    // reading surface data during treatment
    rThread = new SurfaceData(this);
    connect(rThread,SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(onLnReadingFinished(QStringList, int)));

    // loading surface data from planning CT
    lThread = new SurfaceData(this);
    connect(lThread,SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(onLoadingFinished(QStringList, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLnReadingFinished(QStringList SplitLn, int colNum)
{
    QString timeVal = SplitLn[0];
    ui->timeLabel_4->setText(timeVal);

    QString amp1Val = SplitLn[1];

    if(colNum == 2){
        amp1Val.remove(amp1Val.size()-4, amp1Val.size()-1);
    }

    ui->amp1Label_4->setText(amp1Val);

    if(colNum == 3){
        QString amp2Val = SplitLn[2];
        ui->amp2Label_4->setText(amp2Val);
    }
}

void MainWindow::onLoadingFinished(QStringList SplitLn, int colNum)
{
    QString timeValAvg = SplitLn[0];
    ui->timeLabelAvg_4->setText(timeValAvg);

    QString amp1ValAvg = SplitLn[1];

    if(colNum == 2){
        amp1ValAvg.remove(amp1ValAvg.size()-4, amp1ValAvg.size()-1);
    }

    ui->amp1LabelAvg_4->setText(amp1ValAvg);

    if(colNum == 3){
        QString amp2ValAvg = SplitLn[2];
        ui->amp2LabelAvg_4->setText(amp2ValAvg);
    }
}


void MainWindow::on_StartReadingButton_4_clicked()
{
    // Started

    // Read surface data
    rThread->path = MainWindow::path2;
    rThread->sleepingTime = MainWindow::dtSurfData;
    rThread->start();

    // Create dialog window
    chartDialog = new ChartDialog(this);        // "this" points at MainWindow as a parent
    chartDialog->setDisabled(false);            // enables input events

    // Create dynamic chart
    dynamicChart = new DynamicChart(0, 0, rThread);
    dynamicChart->setTitle("Real time data");
    dynamicChart->setFont(QFont("sans", 10, QFont::Bold));
    dynamicChart->legend()->hide();
    dynamicChart->setAnimationOptions(QChart::AllAnimations);      // Enable 'AllAnimations' for the chart

    // Create QChartView widget which displays the dynamic chart
    QChartView *dynamicChartView = new QChartView(dynamicChart);
    dynamicChartView->setRenderHint(QPainter::Antialiasing);       // Enable render hint 'Antialiasing' for chart view

    // Set layout
    chartDialog->setLayout(new QVBoxLayout());
    chartDialog->layout()->addWidget(dynamicChartView);     // add dynamic chartView into the layout
    chartDialog->layout()->addWidget(staticChart);      // add static chartView
    chartDialog->resize(700, 500);
    chartDialog->show();
}

void MainWindow::on_StopReadingButton_4_clicked()
{
    // Stopped
    rThread->Stop = true;
}

void MainWindow::on_LoadingButton_4_clicked()
{
    // Started

    staticChart = new QCustomPlot();
    staticChart->setFixedSize(700,210);

    QObject::connect(lThread, SIGNAL(DataReadingFinished(QList<double>*, QList<double>*, QList<double>*, int)),
                     this, SLOT(addAvgData(QList<double>*, QList<double>*, QList<double>*, int)));

    lThread->path = MainWindow::path1;
    lThread->sleepingTime = 0;
    lThread->start();


}

void MainWindow::on_StopLoadingButton_4_clicked()
{
    // Stopped
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

    // Add data
    QVector<double> StatTimeVec = TimeListPtr->toVector();
    QVector<double> StatAmp1Vec = Amp1ListPtr->toVector();

    double qStart = StatTimeVec[0];
    for(int i = 0; i<StatTimeVec.size(); ++i){
        StatTimeVec[i] = (StatTimeVec[i]-qStart)/1000;      // [t] = 1 ms -> 1 s & t starts with 0
    }

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


