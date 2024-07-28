#include "threadmanager.h"
#include <QTimer>
#include <QTextStream>
#include <QCoreApplication>

ThreadManager::ThreadManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Thread Manager constructed";
    // Initialize threads and their respective tasks
    // Threads will be created and started in startThreads()
}

ThreadManager::~ThreadManager()
{
    qDebug() << "Thread Manager is being destroyed";
    stopThreads(); // Ensure all threads are stopped
}

void ThreadManager::startThreads()
{
    QMutexLocker locker(&mutex); // Ensure thread-safe operations

    // Create and start threads for different tasks
    QThread *inputThread = new QThread(this);
    QThread *outputThread = new QThread(this);

    setupThread(inputThread, "InputThread", inputTask);
    setupThread(outputThread, "OutputThread", outputTask);

    threads.append(inputThread);
    threads.append(outputThread);
}

void ThreadManager::stopThreads()
{
    QMutexLocker locker(&mutex); // Ensure thread-safe operations

    foreach (QThread *thread, threads) {
        if (thread->isRunning()) {
            thread->quit();
            thread->wait();
        }
        delete thread;
    }
    threads.clear();
}

void ThreadManager::setupThread(QThread *thread, const QString &name, void (*function)())
{
    thread->setObjectName(name);
    QObject::connect(thread, &QThread::started, function);
    thread->start();
}

void ThreadManager::inputTask()
{
    // Simulate input task
    qDebug() << "Input task running on thread" << QThread::currentThread()->objectName();
    QTextStream stream(stdin);
    QString line;
    while (stream.readLineInto(&line)) {
        qInfo() << "You entered" << line;
        if (line.trimmed().toLower() == "q") {
            emit QCoreApplication::instance()->quit();
            break; // Exit the loop if the user enters 'q'
        } else {
            qInfo() << "Enter q to exit.";
        }
    }
}

void ThreadManager::outputTask()
{
    // Simulate output task
    qDebug() << "Output task running on thread" << QThread::currentThread()->objectName();
    QTimer *timer = new QTimer();
    timer->setInterval(5000);
    int count = 0;
    QObject::connect(QThread::currentThread(),&QThread::finished,timer,&QObject::deleteLater);
    QObject::connect(timer, &QTimer::timeout, [=]() mutable {
        count++;
        if (count > 10) {
            qInfo() << "Stopping after hitting maximum count";
            emit QCoreApplication::instance()->quit();
        } else {
            qInfo() << "Hello World" << count;
        }
    });
    timer->start();
}
