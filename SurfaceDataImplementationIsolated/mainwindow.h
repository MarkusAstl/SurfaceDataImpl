#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "surfacedata.h"
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
    SurfaceData *lThread;
    SurfaceData *prThread;

    QString path1 = "SurfaceData/Sarah_4D_CT_dummy2.csv";                          // two columns
    QString path2 = "SurfaceData/2021_11_30_Sarah_RespSignal_20211130_171956_clean.csv";    // three columns

    ChartDialog *chartDialog;
    DynamicChart *dynamicChart;
    DynamicChart *filterChart;
    QCustomPlot *staticChart;

    int dtSurfData = 75;

public slots:
    void onLnReadingFinished(QStringList, int);
    void onLoadingFinished(QStringList, int);
    void addAvgData(QList<double>*, QList<double>*, QList<double>*, int);
    void phaseChanged(int*);
    void on_CreateChart(double*, double*, double*);

private:
    Ui::MainWindow *ui;

private slots:
    void on_StartReadingButton_4_clicked();
    void on_StopReadingButton_4_clicked();
    void on_LoadingButton_4_clicked();
    void on_StopLoadingButton_4_clicked();
    void on_StartPhaseRecognitionButton_clicked();
    void extremum_search(QList<double>*, QList<int>*, QList<int>*, bool*);
    void on_fc_slider_sliderMoved(int position);

signals:
    void adjustYaxis(double*);
};
#endif // MAINWINDOW_H
