#ifndef DEBUGTHREAD_H
#define DEBUGTHREAD_H
#include <QtCore>


class DebugThread  :  public QThread
{
    Q_OBJECT
public:
    explicit DebugThread(QObject *parent = 0);
};

#endif // DEBUGTHREAD_H
