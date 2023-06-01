#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <surfacedata.h>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

namespace Ui {
class ChartDialog;
}

class ChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartDialog();
    ~ChartDialog();

private:
    Ui::ChartDialog *ui;
};


class DynamicChart: public QtCharts::QChart
{
    Q_OBJECT
public:
    DynamicChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0, QTextStream *logStream = 0, double* frst_A = 0, double* frst_t = 0);
    virtual ~DynamicChart();

    QTextStream *logStreamPtr;
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
    void addDataPoint(double, double);
    void addFilteredDataPoint(double, double);
    //void on_adjustYaxis(double*);

private:

};


#endif // CHARTDIALOG_H
