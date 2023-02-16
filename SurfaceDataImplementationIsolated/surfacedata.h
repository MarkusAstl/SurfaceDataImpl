#ifndef SURFACEDATA_H
#define SURFACEDATA_H

#include <QThread>

class SurfaceData : public QThread
{
    Q_OBJECT
public:
    explicit SurfaceData(QObject *parent = 0);

    void run();
    bool Stop = false;

    bool filtering;

    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;

    QList<int> maxIndList;
    QList<int> minIndList;

    int oldMaxInd;
    int newMaxInd;

    double T;
    double dT;
    double T_old;
    int phase;
    double dt;
    QList<double> BorderTimes;
    QList<double> globalBorderTimes;
    int lastMinInd;
    int lastMaxInd;
    bool maxIsLast;
    bool extrDetectionActive;
    bool phaseRecogActive;

    int windowSize = 7;
    double fc = 0.33;
    double fs;
    double T_planning = 3.5;
    double aValue;
    double fValue;
    QList<double> h;    // convolution function h for filtering
    QList<double> smthdAmp1List;

    QString path;
    int sleepingTime;  

public slots:
    void phaseRecognition(double*, double*, double*, int*, bool*, bool*);
    bool newMaxDetected(QList<double>*, bool*, int*, int*);
    bool checkBreathCycleDur(double*, double*, double*);

signals:
    void LnReadingFinished(QStringList, int);
    void DataReadingFinished(QList<double>*, QList<double>*, QList<double>*, int);
    void FilteringFinished(double, double);
    void monotonyCheckDone(QList<double>*, QList<double>*, int*, int*, int*);
    void showCurrentPhase(int*);
    void CreateChart(double*, double*, double*);
};

#endif // SURFACEDATA_H
