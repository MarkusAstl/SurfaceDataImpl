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

ChartDialog::ChartDialog()
{
    ui->setupUi(this);
}

ChartDialog::~ChartDialog()
{
    delete ui;
}

DynamicChart::DynamicChart(QGraphicsItem *parent, Qt::WindowFlags wFlags, QTextStream *logStream, double *frst_t, double *frst_A) :
    QChart(QChart::ChartTypeCartesian, parent, wFlags), logStreamPtr(logStream)
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
    xAxisRange = 40;
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

void DynamicChart::addDataPoint(double xVal, double yVal)
{
    // xVal: Time value, yVal: Amplitude value
    // Converts doubles to qreals, adjusts x axis interval if neccessary and adds new datapoint to DynamicChart of raw data

    qreal x = static_cast<qreal>(xVal);
    qreal y = static_cast<qreal>(yVal);

    // Shift x axis range if data exceeds graph
    if(x >= xAxisStart + xAxisRange){
        scroll(plotArea().width(), 0);
        xAxisStart += xAxisRange;
        dataSeries->clear();
    }

    // Add new point to data series
    QPointF m_point(x, y);
    dataSeries->append(m_point);
}

void DynamicChart::addFilteredDataPoint(double t, double A)
{
    // t: new time value;  A: corresponding filtered amplitude value
    // Adjusts x axis range if neccessary and adds new filtered datapoint to DynamicChart of filtered Data

    qreal x = static_cast<qreal>(t);
    qreal y = static_cast<qreal>(A);

    // Shift x axis range if data exceeds graph
    if(x >= xAxisStart + xAxisRange){
        scroll(plotArea().width(), 0);
        xAxisStart += xAxisRange;
        dataSeries->clear();
    }

    // Add point to plotting series
    QPointF m_point(x,y);
    dataSeries->append(m_point);
}


//void DynamicChart::on_adjustYaxis(double* newAmp)
//{
//    // newAmp: The new value which should be in the middle of the y axis
//    // Adjusts y axis range if an amplitude value exceeds the axis range

//    axisY->setRange(*newAmp - yAxisRange*0.5, *newAmp + yAxisRange*0.5);
//}
