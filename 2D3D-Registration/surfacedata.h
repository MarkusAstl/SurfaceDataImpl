#ifndef SURFACEDATA_H
#define SURFACEDATA_H

#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QElapsedTimer>
#include <QMutex>
#include <armadillo>

#include "ui_mainwindow.h"

class SurfaceData : public QThread
{
    Q_OBJECT
public:
    explicit SurfaceData(QObject *parent = 0, Ui::MainWindow *ui = 0);

    void run();
    bool Stop = false;

    Ui::MainWindow *uiPtr;

    // Reading
    QMutex mutex;
    QElapsedTimer timer;
    QTimer* IterTimer;
    QString path;
    QFile f;
    int sleepingTime;     //75;
    int iteration = 1;

    // Debug
    qint64 elapsed;
    double sumElapsed = 0.0;
    double n_elapsed = 0.0;
    arma::vec elapsedList;
    double maxElapsed = 0.00000000000;
    QFile fout;
    QFile logFile;
    QTextStream stream;
    QTextStream logStream;

    // Value lists
    QStringList SplitLn;
    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> smthdAmp1List;
    //QVector<int> PhaseList;
    //QVector<int> IntervalPhaseList;
    QList<double> BorderTimes;
    QList<double> globalBorderTimes;

    // Indexing
    QList<int> maxIndList;
    QList<int> minIndList;
    int oldMaxInd;
    int newMaxInd;
    int lastMinInd;
    bool maxIsLast;

    // Filter shift correction
    double filterShift;
    double correctedNewMaxTime;

    // Phase recognition
    bool PhaseRecogStarted = false;
    double T;
    double dT;
    double T_old;
    double dAcheck;
    int phase = 0;
    double dt;
    bool phaseRecogActive;
    int localInd;

    // Filtering
    int windowSize = 5;
    double fc = 0.33;           // only default, can be adjusted in UI
    double fs;
    double T_planning = 3.5;    // Will be extracted from planning CT
    double aValue;
    double fValue;



public slots:
    void readOnly();
    //bool createOutputFile();
    bool readHeaderLines(QFile*);
    void phaseRecognition(double*, double*, double*, int*, double*, double*, double*, bool*);
    bool newMaxDetected(QList<double>*, bool*, int*, int*);
    bool checkBreathCycleDur(double*, double*, double*, bool*);
    void setFilterParams();
    void filterNewDatapoint(QList<double> *, QList<double> *, double *);
    void extremum_search(QList<double>*, QList<int>*, QList<int>*, bool*);
    void ReadIter();
    void ReadFiltIter();
    void ReadFiltPhaseDebugIter();
    void ReadFiltPhasePerformIter();

private slots:
    bool saveValsToLists(QList<double>*, QList<double>*, QStringList*);
    QStringList readOneLn(QFile*);

signals:
    void LnReadingFinished(double, double);
    void FilteringFinished(double, double);
    void monotonyCheckDone(QList<double>*, QList<double>*, int*, int*, int*);
    void showCurrentPhase(int*);
    void CreateChart(double, double, double);
};

#endif // SURFACEDATA_H
