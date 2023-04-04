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

    // Define data series for spline chart
    dataSeries = new QtCharts::QSplineSeries(this);

    // Set drawing pen
    QPen drawPen(Qt::blue);
    drawPen.setWidth(2);
    dataSeries->setPen(drawPen);

    // Set axis
    QPointF frst_point(*frst_t, *frst_A);       // Adjust axis range to first data point
    dataSeries->append(frst_point);
    axisX = new QtCharts::QValueAxis;
    axisY = new QtCharts::QValueAxis;
    axisX->setTickCount(16);                    // number of grid lines
    addAxis(axisX, Qt::AlignBottom);
    addAxis(axisY, Qt::AlignLeft);
    xAxisRange = 20;
    yAxisRange = 10;
    axisX->setRange(*frst_t, *frst_t + xAxisRange);
    axisY->setRange(*frst_A - yAxisRange*0.5, *frst_A + yAxisRange*0.5);
    axisX->setTitleText("t [s]");
    axisY->setTitleText("Amp. [mm]");
    xAxisStart = *frst_t;       // parameter for scroll function
    yAxisMiddle = *frst_A;

    // Add data series to chart
    addSeries(dataSeries);
    dataSeries->attachAxis(axisX);
    dataSeries->attachAxis(axisY);
}

DynamicChart::~DynamicChart()
{

}

void DynamicChart::addDataPoint(QStringList SplitLn)
{
    // SplitLn: New read line of csv file with [timeVal, ampVal, ..]
    // Converts QStrings to doubles, adjusts x axis range if neccessary and adds new datapoint to DynamicChart of raw data

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

    // Add new point to data series
    QPointF m_point(xVal, yVal);
    dataSeries->append(m_point);
}

void DynamicChart::addFilteredDataPoint(double t, double A)
{
    // t: new time value;  A: corresponding filtered amplitude value
    // Adjusts x axis range if neccessary and adds new filtered datapoint to DynamicChart of filtered Data

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
    // newAmp: The new value which should be in the middle of the y axis
    // Adjusts y axis range if an amplitude value exceeds the axis range

    axisY->setRange(*newAmp - yAxisRange*0.5, *newAmp + yAxisRange*0.5);
}
