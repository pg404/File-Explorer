#include "task.h"
#include "run.h"
#include "ui_task.h"

Task::Task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    this->showProcess();
    QObject::connect(ui->showProcessButton,SIGNAL(clicked(bool)),this,SLOT(showProcess()));
    QObject::connect(ui->killButton,SIGNAL(clicked(bool)),this,SLOT(killProcess()));
    QObject::connect(ui->startNewBoutton,SIGNAL(clicked(bool)),this,SLOT(startProcess()));
}

Task::~Task()
{
    delete ui;
}

void Task::startProcess()
{
    run *newRun = new run;
    newRun->show();
}

void Task::showProcess()
{
       ui->listProcessWidget->clear();
       QProcess process;
       process.start("tasklist.exe");
       if(process.waitForFinished())
       {
           QString result = QString::fromLocal8Bit(process.readAll());
           QString lineResult;
           while (!result.isEmpty())
           {
               for (int i=0;i<result.length();i++)
               if (result.at(i)== '\n')
                 {
                        lineResult=result.mid(0,i+1);
                        result.remove(0,i+1);
                        QListWidgetItem *item =new QListWidgetItem;
                        item->setText(lineResult);
                        ui->listProcessWidget->addItem(item);
                        break;
                 }

           }

       }

}
void Task::killProcess()
{
    QListWidgetItem *item = ui->listProcessWidget->item(ui->listProcessWidget->currentRow());
    QString getProcessText = item->text();
    QString willKillProcess;
    bool isSuccessKill=false;
    for (int i=0; i<getProcessText.length(); i++)
    {
        if (getProcessText.at(i)=='.'&&getProcessText.at(i+1)=='e'&&getProcessText.at(i+2)=='x'
                                     &&getProcessText.at(i+3)=='e')
        {
            willKillProcess= getProcessText.mid(0,i+4);
            QProcess process;
            process.execute("TASKKILL /IM "+willKillProcess+" /F");
            process.close();
            ui->listProcessWidget->takeItem(ui->listProcessWidget->currentRow());
            QMessageBox::information(this,"sucess",QStringLiteral("成功结束进程！"));
            isSuccessKill = true;
            break;
        }

    }
    if (!isSuccessKill)
        QMessageBox::information(this,"error",QStringLiteral("无法结束系统进程！"));
}
