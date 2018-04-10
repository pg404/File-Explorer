#include "filterdailog.h"
#include "ui_filterdailog.h"

FilterDailog::FilterDailog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDailog)
{
    ui->setupUi(this);
}

FilterDailog::~FilterDailog()
{
    delete ui;
}
