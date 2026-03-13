#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include "SbPreCompile.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
  Q_OBJECT

public:
    QString getSearchingText(); //!<获取要搜索的文字。
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
