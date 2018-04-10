#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class Task;
}

class Task : public QWidget
{
    Q_OBJECT

public:
    explicit Task(QWidget *parent = 0);
    ~Task();

private:
    Ui::Task *ui;

private slots:
    void showProcess();
    void startProcess();
    void killProcess();
};

#endif // TASK_H
