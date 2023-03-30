#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "surfacedata.h"
#include "loadingthread.h"
#include "chartdialog.h"
#include "qcustomplot.h"
#include <armadillo>

#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SurfaceData *rThread;
    LoadingThread *lThread;

    QString rPath;
    QString lPath;

    ChartDialog *chartDialog;
    DynamicChart *dynamicChart;
    DynamicChart *filterChart;
    QCustomPlot *staticChart;

public slots:
    void addAvgData(QList<double>*, QList<double>*);
    void phaseChanged(int*);
    void on_CreateChart(double, double, double);

private:
    Ui::MainWindow *ui;

private slots:
    void on_StartReadingButton_clicked();
    void on_StopReadingButton_clicked();
    void on_LoadingButton_clicked();
    void on_StopLoadingButton_clicked();
    void on_StartPhaseRecognitionButton_clicked();
    void extremum_search(QList<double>*, QList<int>*, QList<int>*, bool*);
    void on_fc_slider_sliderMoved(int position);
    void on_SelectReadingFileButton_clicked();
    void on_LoadingStopped();

    void on_SelectLoadingFileButton_clicked();

signals:
    void adjustYaxis(double*);
};
#endif // MAINWINDOW_H
