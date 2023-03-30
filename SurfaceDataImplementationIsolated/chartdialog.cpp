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

ChartDialog::ChartDialog(QWidget *parent)
{
    ui->setupUi(this);
}

ChartDialog::~ChartDialog()
{
    delete ui;
}

DynamicChart::DynamicChart(QGraphicsItem *parent, Qt::WindowFlags wFlags, SurfaceData *rThread, double *frst_t, double *frst_A) :
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
//    // If a new line was read the data series of the graph will be updated and x axis will be scrolled if necessary
//    QObject::connect(rThread, SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(addDataPoint(QStringList, int)));

    //dataSeries = NULL;

    // Define data series for spline chart
    dataSeries = new QtCharts::QSplineSeries(this);

    // Set drawing pen
    QPen drawPen(Qt::blue);                              // QPen defines how QPainter draws lines -> blue line
    drawPen.setWidth(2);
    dataSeries->setPen(drawPen);                         // apply defined QPen to spline chart

    // Set axis
//    xVal = 126;         // [t] = 1 s
//    yVal = 53.3794;
    QPointF frst_point(*frst_t, *frst_A);
    dataSeries->append(frst_point);
    axisX = new QtCharts::QValueAxis;
    axisY = new QtCharts::QValueAxis;
    axisX->setTickCount(16);                      // number of grid lines (one value for every grid line)
    addAxis(axisX, Qt::AlignBottom);
    addAxis(axisY, Qt::AlignLeft);
    xAxisRange = 20;
    yAxisRange = 10;
    axisX->setRange(*frst_t, *frst_t + xAxisRange);
    axisY->setRange(*frst_A - yAxisRange*0.5, *frst_A + yAxisRange*0.5);
    axisX->setTitleText("t [s]");
    axisY->setTitleText("Amp. [mm]");
    xAxisStart = *frst_t;
    yAxisMiddle = *frst_A;

    // Add series to chart
    addSeries(dataSeries);
    dataSeries->attachAxis(axisX);
    dataSeries->attachAxis(axisY);
}

DynamicChart::~DynamicChart()
{

}

void DynamicChart::addDataPoint(QStringList SplitLn)
{
    // Convert QString to double
    bool ok1, ok2;
    xVal = SplitLn[0].toDouble(&ok1)/1000;        // [t] = 1 ms -> 1 s
    yVal = SplitLn[1].toDouble(&ok2);

    // Check if convertion worked
    if(ok1 == false || ok2 == false){
        qDebug() << "Error in DynamicChart::addDataPoint(QStringList). Converting QString to double was"
                    "not successfull";
    }

    // Shift x axis range if data exceeds graph
    if(xVal >= xAxisStart + xAxisRange){
        scroll(plotArea().width()*0.5, 0);
        xAxisStart += xAxisRange/2;
    }

    // Add point to plotting series
    QPointF m_point(xVal, yVal);
    dataSeries->append(m_point);
}

void DynamicChart::addFilteredDataPoint(double t, double A)
{
    // Shift x axis range if data exceeds graph
    if(t >= xAxisStart + xAxisRange){
        scroll(plotArea().width()*0.5, 0);
        xAxisStart += xAxisRange/2;
    }

    // Add point to plotting series
    QPointF m_point(t, A);
    dataSeries->append(m_point);
}


void DynamicChart::on_adjustYaxis(double* newAmp)
{
    axisY->setRange(*newAmp - yAxisRange*0.5, *newAmp + yAxisRange*0.5);
}
