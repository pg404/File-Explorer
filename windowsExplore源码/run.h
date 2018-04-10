#ifndef RUN_H
#define RUN_H

#include <QDialog>
#include <QProcess>
namespace Ui {
class run;
}

class run : public QDialog
{
    Q_OBJECT

public:
    explicit run(QWidget *parent = 0);
    ~run();

private:
    Ui::run *ui;

private slots:
    void runprocess();
    void cancelon();
};

#endif // RUN_H
