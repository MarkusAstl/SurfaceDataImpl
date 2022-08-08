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
