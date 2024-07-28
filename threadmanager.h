#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

class ThreadManager : public QObject
{
    Q_OBJECT

public:
    explicit ThreadManager(QObject *parent = nullptr);
    ~ThreadManager();

    void startThreads();
    void stopThreads();

private:
    QVector<QThread*> threads;
    QMutex mutex; // For thread-safe operations

    void setupThread(QThread *thread, const QString &name, void (*function)());
    static void inputTask();   // Static member functions for thread tasks
    static void outputTask();

signals:
    void quitApplication();

};

#endif // THREADMANAGER_H
