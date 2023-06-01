#ifndef PLOTTINGTHREAD_H
#define PLOTTINGTHREAD_H
#include <QtCore>

//#include "mainwindow.h"
#include "surfacedata.h"
#include "loadingthread.h"
#include "chartdialog.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

#include <QListWidgetItem>


class PlottingThread  :  public QThread
{
    Q_OBJECT
public:
    explicit PlottingThread(SurfaceData *rThread, LoadingThread *lThread, Ui::MainWindow *ui, QObject *parent = 0);

    ChartDialog *chartDialog;
    DynamicChart *dynamicChart;
    DynamicChart *filterChart;
    QCustomPlot *staticChart;

    QObject *parentPtr;
    SurfaceData *rThreadPtr;
    LoadingThread *lThreadPtr;
    Ui::MainWindow *uiPtr;

public slots:
    void on_CreateChart(double, double, double);
    void phaseChanged(int*);
    void addAvgData(QList<double>*, QList<double>*);
};

#endif // PLOTTINGTHREAD_H
