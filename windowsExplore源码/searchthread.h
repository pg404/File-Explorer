#ifndef SRAECHTHREAD_H
#define SRAECHTHREAD_H

#include <QString>
#include <QDir>
#include <QThread>
#include <QDirIterator>
#include <QDebug>

class SearchThread:public QThread
{
    Q_OBJECT
public:
    SearchThread();
    SearchThread(QString path, QString text);
protected:
    void run();
    void stop();

private:
    volatile bool stopped;
    QString getPath;
    QString getSearchText;
    QStringList fileString;
signals:
    void sendData(QStringList);
};

#endif // SRAECHTHREAD_H
