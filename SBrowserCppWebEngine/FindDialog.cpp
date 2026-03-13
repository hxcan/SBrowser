#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \brief FindDialog::getSearchingText 获取要搜索的文字。
 * \return 要搜索的文字。
 */
QString FindDialog::getSearchingText()
{
    return ui->txt2FindlineEdit->text ();
} //QString FindDialog::getSearchingText() //!<获取要搜索的文字。

