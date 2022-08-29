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

    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;

    QString path;
    int sleepingTime;

signals:
    void LnReadingFinished(QStringList, int);
    void DataReadingFinished(QList<double>*, QList<double>*, QList<double>*, int);
};

#endif // SURFACEDATA_H
