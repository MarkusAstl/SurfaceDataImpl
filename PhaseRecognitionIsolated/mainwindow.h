#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

#include "chartdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QCustomPlot *IntpDataChart;
    QCustomPlot *RawDataChart;
    QCustomPlot *SmthDataChart;
    QCustomPlot *PhaseRecogChart;
    ChartDialog *chartDialog;

private slots:
    void on_Button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
