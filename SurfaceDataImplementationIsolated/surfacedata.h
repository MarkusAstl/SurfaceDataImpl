#ifndef SURFACEDATA_H
#define SURFACEDATA_H

#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QElapsedTimer>
#include <QMutex>

class SurfaceData : public QThread
{
    Q_OBJECT
public:
    explicit SurfaceData(QObject *parent = 0);

    void run();
    bool Stop = false;

    QElapsedTimer timer;
    qint64 elapsed;

    bool filtering;
    bool PhaseRecogStarted;
    QMutex mutex;

    QStringList SplitLn;
    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;
    QVector<int> PhaseList;
    QVector<int> IntervalPhaseList;

    QList<int> maxIndList;
    QList<int> minIndList;

    int oldMaxInd;
    int newMaxInd;

    double filterShift;
    double correctedNewMaxTime;

    QFile fout;
    QTextStream stream;

    double T;
    double dT;
    double T_old;
    int phase = 0;
    double dt;
    QList<double> BorderTimes;
    QList<double> globalBorderTimes;
    int lastMinInd;
    int lastMaxInd;
    bool maxIsLast;
    bool phaseRecogActive;

    int windowSize = 5;
    double fc = 0.33;
    double fs;
    double T_planning = 3.5;
    double aValue;
    double fValue;
    QList<double> h;    // convolution function h for filtering
    QList<double> smthdAmp1List;

    QString path;
    QFile f;
    int sleepingTime = 75;

public slots:
    void readOnly();
    bool createOutputFile();
    bool readHeaderLines(QFile*);
    void phaseRecognition(double*, double*, double*, int*, bool*);
    bool newMaxDetected(QList<double>*, bool*, int*, int*);
    bool checkBreathCycleDur(double*, double*, double*, bool*);
    void setFilterParams();
    bool filterNewDatapoint(QList<double> *, QList<double> *, double *);

private slots:
    bool saveValsToLists(QList<double>*, QList<double>*, QStringList*);
    QStringList readOneLn(QFile*);

signals:
    void LnReadingFinished(QStringList);
    void FilteringFinished(double, double);
    void monotonyCheckDone(QList<double>*, QList<double>*, int*, int*, int*);
    void showCurrentPhase(int*);
    void CreateChart(double, double, double);
};

#endif // SURFACEDATA_H
