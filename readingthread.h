#ifndef READINGTHREAD_H
#define READINGTHREAD_H

#include <QThread>

class ReadingThread : public QThread
{
    Q_OBJECT
public:
    explicit ReadingThread(QObject *parent = 0);

    void run();
    bool Stop = false;

    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;

    QString path;
    int sleepingTime;
    bool readDynBool;

signals:
    void LnReadingFinished(QStringList, int);
};

#endif // READINGTHREAD_H
