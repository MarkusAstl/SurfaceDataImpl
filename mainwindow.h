#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <readingthread.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ReadingThread *rThread;
    ReadingThread *lThread;

    QString path1 = "SurfaceData/BruMi-20210531-083409_clean.csv";                          // two columns
    QString path2 = "SurfaceData/2021_11_30_Sarah_RespSignal_20211130_171956_clean.csv";    // three columns

private:
    Ui::MainWindow *ui;

private slots:
    void on_StartReadingButton_clicked();
    void on_StopReadingButton_clicked();
    void on_LoadingButton_clicked();
    void on_StopLoadingButton_clicked();

public slots:
    void onLnReadingFinished(QStringList, int);
    void onLoadingFinished(QStringList, int);

};

#endif // MAINWINDOW_H
