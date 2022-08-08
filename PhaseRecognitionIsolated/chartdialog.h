#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

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


#endif // CHARTDIALOG_H
