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

    int peak1Ind;
    int peak2Ind;
    int peak3Ind;
    int peak4Ind;

    int phase;
    double dt;
    QList<double> BorderTimes;
    QList<double> globalBorderTimes;
    bool* maxIsLast;
    bool prReady;

    int windowSize = 9;
    double fc = 0.333;
    double aValue;
    double fValue;
    QList<double> h;    // convolution function h for filtering
    QList<double> smthdAmp1List;

    QString path;
    int sleepingTime;  

public slots:
    void calcPhaseBordersByTime(QList<double>*, QList<double>*, QList<double>*, QList<double>*, int*, int*, int*);
    void phaseRecognition(bool*, double*, QList<double>*, int*);
    bool monotonyChanged(double*, double*, double*, bool*);

signals:
    void LnReadingFinished(QStringList, int);
    void DataReadingFinished(QList<double>*, QList<double>*, QList<double>*, int);
    void monotonyCheckDone(QList<double>*, QList<double>*, int*, int*, int*);
    void showCurrentPhase(int*);
};

#endif // SURFACEDATA_H
