#include "run.h"
#include "ui_run.h"

run::run(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::run)
{
    ui->setupUi(this);
    this->setMaximumSize(380,176);
    this->setMinimumSize(380,176);
    QObject::connect(ui->submitButton,SIGNAL(clicked()),this,SLOT(runprocess()));
    QObject::connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelon()));
}

run::~run()
{
    delete ui;
}

void run::runprocess()
{
    QProcess *startp =new QProcess;
    QString processn = ui->cmdLineEdit->text();
    startp->start(processn.trimmed());
    ui->cmdLineEdit->clear();;
    this->close();
}

void run::cancelon()
{
    this->close();
}
