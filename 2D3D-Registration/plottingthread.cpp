#include "plottingthread.h"
#include <QtCore>
#include <QDebug>
#include <QFont>

#include <surfacedata.h>
#include <loadingthread.h>
#include <mainwindow.h>
#include <qcustomplot.h>
#include "ui_mainwindow.h"

// Surface data chart
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "qcustomplot.h"
#include <QLayout>

QT_CHARTS_USE_NAMESPACE

PlottingThread::PlottingThread(SurfaceData *rThread, LoadingThread *lThread, Ui::MainWindow *ui, QObject *parent)   :
    parentPtr(parent), rThreadPtr(rThread), lThreadPtr(lThread), uiPtr(ui)
{

}

void PlottingThread::on_CreateChart(double frst_t, double frst_A, double frstFilt_A)
{
    // Create dialog window to show real time and planning CT data
    chartDialog = new ChartDialog();        // Define new QDialog object; "this" points at MainWindow as a parent
    chartDialog->setDisabled(false);            // enables input events
    chartDialog->setLayout(new QVBoxLayout());

    // Plot filtered data
    filterChart = new DynamicChart(0, 0, &rThreadPtr->logStream, &frst_t, &frstFilt_A);
    filterChart->setTitle("Filtered Data");
    filterChart->setTitleFont(QFont("sans", 10, QFont::Bold));
    filterChart->legend()->hide();
    filterChart->setAnimationOptions(QChart::AllAnimations);
    QChartView *filterChartView = new QChartView(filterChart);
    filterChartView->setRenderHint(QPainter::Antialiasing);
    QObject::connect(rThreadPtr, SIGNAL(FilteringFinished(double, double)), filterChart, SLOT(addFilteredDataPoint(double, double)));
    chartDialog->layout()->addWidget(filterChartView);      // add filter chartView

    // If Debugging Mode..
    if (uiPtr->DebugModeRadioButton->isChecked()){

        // Plot raw data
        dynamicChart = new DynamicChart(0, 0, &rThreadPtr->logStream, &frst_t, &frst_A);     // QChart object; starts constructor of DynamicChart
        dynamicChart->setTitle("Measured Data");
        dynamicChart->setTitleFont(QFont("sans", 10, QFont::Bold));
        dynamicChart->legend()->hide();
        dynamicChart->setAnimationOptions(QChart::AllAnimations);      // Enable 'AllAnimations' for the chart
        QChartView *dynamicChartView = new QChartView(dynamicChart);
        dynamicChartView->setRenderHint(QPainter::Antialiasing);       // Enable render hint 'Antialiasing' for chart view
        QObject::connect(rThreadPtr, SIGNAL(LnReadingFinished(double, double)), dynamicChart, SLOT(addDataPoint(double, double)));
        chartDialog->layout()->addWidget(dynamicChartView);     // add dynamic chartView into the layout

        // Plot data from planning CT
        staticChart = new QCustomPlot();
        staticChart->setFixedSize(950,210);
        staticChart->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));   // Set points as decimal separator
        if (lThreadPtr->DataLoadingFinished){
            addAvgData(&lThreadPtr->TimeList, &lThreadPtr->Amp1List);
        }
        chartDialog->layout()->addWidget(staticChart);      // add static chartView

        chartDialog->resize(1000, 750);
    }

    // If Performance Mode..
    else{
        // Plot only filtered data
        chartDialog->resize(1000, 250);
    }

    // Show dialog window
    chartDialog->show();
}

void PlottingThread::phaseChanged(int* phase)
{
    // Show current phase in UI of MainWindow
    QString phaseLabelStr = QString::number(*phase);
    uiPtr->phaseLabel->setText(phaseLabelStr);
}

void PlottingThread::addAvgData(QList<double>* TimeListPtr, QList<double>* Amp1ListPtr)
{
    // Set chart title
    QCPTextElement *staticTitle = new QCPTextElement(staticChart);
    staticTitle->setText("Average Surface Data");
    staticTitle->setFont(QFont("sans", 10, QFont::Bold));
    staticChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    staticChart->plotLayout()->addElement(0, 0, staticTitle); // place the title in the empty cell we've just created

    // Set drawing pen
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
        textLabel->setFont(QFont("Arial",8));
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
