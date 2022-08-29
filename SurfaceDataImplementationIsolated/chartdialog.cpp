#include "chartdialog.h"
#include "ui_chartdialog.h"
#include "mainwindow.h"
#include "surfacedata.h"
#include "qcustomplot.h"

#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QTime>
#include <QtCore/QDebug>

#include <iostream>
#include <mainwindow.h>

// Könnte es sein, dass zwei verschiedene dynamicChartView bestehen und die Funktion (wenn keine Punkte der Series
// übernommen werden) das falsche ChartView Objekt nimmt? Weil bei unbenennung der Variable mit Rename Symbol wurde
// das ChartView nicht für den gesamten Code geändert

ChartDialog::ChartDialog(QWidget *parent)
{
    ui->setupUi(this);
}

ChartDialog::~ChartDialog()
{
    delete ui;
}

DynamicChart::DynamicChart(QGraphicsItem *parent, Qt::WindowFlags wFlags, SurfaceData *rThread) :
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    QObject::connect(rThread, SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(addDataPoint(QStringList, int)));
    dataSeries = NULL;
    dataAxis = new QtCharts::QValueAxis;
    xVal = 126;         // [t] = 1 s
    yVal = 53.3794;

    dataSeries = new QtCharts::QSplineSeries(this);     // data represented as spline charts
    QPen redPen(Qt::blue);                              // QPen defines how QPainter draws lines -> blue line
    redPen.setWidth(2);                                 // pen width
    dataSeries->setPen(redPen);                         // apply defined QPen to spline chart
    dataSeries->append(xVal, yVal);

    addSeries(dataSeries);              // adds series m_series to chart
    createDefaultAxes();                // create axes based on already added series
    setAxisX(dataAxis, dataSeries);     // setting up axis

    dataAxis->setTickCount(8);                  // number of grid lines (one value for every grid line)
    xAxisRange = 20;
    axisX()->setRange(xVal, xVal + xAxisRange);           // starting x Limits
    axisY()->setRange(50, 65);                  // y Limits
    axisX()->setTitleText("t [s]");
    axisY()->setTitleText("Amp. [mm]");
    xAxisStart = xVal;
    //debugVal = 0;
}

DynamicChart::~DynamicChart()
{

}

void DynamicChart::addDataPoint(QStringList SplitLn, int colNum)
{
    bool ok1, ok2;
    xVal = SplitLn[0].toDouble(&ok1)/1000;        // [t] = 1 ms -> 1 s
    yVal = SplitLn[1].toDouble(&ok2);

    if(ok1 == false || ok2 == false){
        qDebug() << "----Error in Chart::addDataPoint function----";
    }

    if(xVal >= xAxisStart + xAxisRange){
        scroll(plotArea().width()*0.75, 0);
        xAxisStart += xAxisRange/2;
    }

//    scroll(1, 0);

    QPointF m_point(xVal, yVal);
    dataSeries->append(m_point);     // add point to series

    //qDebug() << dataSeries->at(debugVal);
    //debugVal += 1;
}


