#include "chartdialog.h"
#include "ui_chartdialog.h"
#include "mainwindow.h"
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
