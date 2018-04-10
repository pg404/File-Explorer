#ifndef FILTERDAILOG_H
#define FILTERDAILOG_H

#include <QDialog>

namespace Ui {
class FilterDailog;
}

class FilterDailog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDailog(QWidget *parent = 0);
    ~FilterDailog();

private:
    Ui::FilterDailog *ui;
};

#endif // FILTERDAILOG_H
