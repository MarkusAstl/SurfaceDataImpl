#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <surfacedata.h>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;            // access both classes only via QtCharts::QSplineSeries & QtCharts::QValueAxis
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

namespace Ui {
class ChartDialog;              // Ui::ChartDialog is NOT same as ChartDialog but it is a private part of it for setting
}                               //      up the user interface

class ChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartDialog(QWidget *parent = 0);      // explicit -> Constructor is no Conversion Constructor
    ~ChartDialog();                                 // default QWidget as argument is a nullpointer

private:
    Ui::ChartDialog *ui;        // Define pointer to class for user interface by using namespace Ui
};


class DynamicChart: public QtCharts::QChart
{
    Q_OBJECT
public:
    DynamicChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0, SurfaceData *rThread = 0, double* frst_A = 0, double* frst_t = 0);
    virtual ~DynamicChart();

    QtCharts::QChartView *dynamicChartView;
    QtCharts::QSplineSeries *dataSeries;
    QtCharts::QValueAxis *axisX;
    QtCharts::QValueAxis *axisY;
    qreal xVal;
    qreal yVal;
    int xAxisRange;
    int yAxisRange;
    qreal xAxisStart;
    qreal yAxisMiddle;
    QPen drawPen;
    //int debugVal;

public slots:
    void addDataPoint(QStringList);
    void addFilteredDataPoint(double, double);
    void on_adjustYaxis(double*);

private:

};


#endif // CHARTDIALOG_H
