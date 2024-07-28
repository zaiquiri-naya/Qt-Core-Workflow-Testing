#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include "threadmanager.h"

QtMessageHandler originalHandler = nullptr;

// File Logger: Logs all messages to a file with formatting
void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen("log.txt", "a");
    if (f) {
        fprintf(f, "%s\n", qPrintable(message));
        fflush(f);
    }
}

// Console Logger: Prints only info messages to standard output without formatting
void logInfoToConsole(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (type == QtInfoMsg) {
        fprintf(stdout, "%s\n", qPrintable(msg));
        fflush(stdout);
    }
}

// Combined Logger: Calls both logToFile and logInfoToConsole
void combinedLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    logToFile(type, context, msg); // Log to file
    logInfoToConsole(type, context, msg); // Log info to console

    // Call the original handler if it exists
    // if (originalHandler) {
    //    originalHandler(type, context, msg);
    //}
}

int main(int argc, char *argv[])
{
    originalHandler = qInstallMessageHandler(combinedLogger); // Install the handler

    QCoreApplication a(argc, argv);

    QThread::currentThread()->setObjectName("Main Thread");
    qInfo() << "Application Starting on" << QThread::currentThread();

    ThreadManager *threadManager = new ThreadManager(&a);
    threadManager->startThreads(); // Start all threads

    qDebug() << "Finished starting threads on" << QThread::currentThread();

    // Run the event loop
    return a.exec();
}
