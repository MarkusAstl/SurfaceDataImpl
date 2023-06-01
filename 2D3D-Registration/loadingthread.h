#ifndef LOADINGTHREAD_H
#define LOADINGTHREAD_H
#include <QtCore>
#include <QMutex>

class LoadingThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadingThread(QObject *parent = 0);
    void run();

    QString path;
    QFile f;
    QMutex mutex;
    bool DataLoadingFinished;
    QStringList SplitLn;
    QList<double> TimeList;
    QList<double> Amp1List;

    void readOnly();
    bool readHeaderLines(QFile*);
    bool saveValsToLists(QList<double>*, QList<double>*, QStringList*);
    QStringList LoadOneLn(QFile*);

signals:
    void LoadingStopped();
};

#endif // LOADINGTHREAD_H
