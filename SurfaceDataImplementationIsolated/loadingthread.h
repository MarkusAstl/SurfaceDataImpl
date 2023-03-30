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

    QStringList SplitLn;
    QList<double> TimeList;
    QList<double> Amp1List;

    QMutex mutex;

    void readOnly();
    bool readHeaderLines(QFile*);

    bool saveValsToLists(QList<double>*, QList<double>*, QStringList*);
    QStringList LoadOneLn(QFile*);

signals:
    void DataLoadingFinished(QList<double>*, QList<double>*);
    void LoadingStopped();
};

#endif // LOADINGTHREAD_H
