#include "conflict_dlg.h"
#include "ui_conflict_dlg.h"
#include "data/word.h"
#include "article.h"

ConflictDlg_C::ConflictDlg_C(Word_C &old_word, Word_C &new_word, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConflictDlg_C)
{
    ui->setupUi(this);
    QString warning_text = QString("Word \"%1\" already exists.").arg(new_word.GetWordText());
    bool isSame = true;
    if(old_word.GetArtikel() != new_word.GetArtikel()) {
        warning_text.append(QString("\n\nOld article is \"%1\" and new article is \"%2\".")
                .arg(Article_C::ArtikelText(old_word.GetArtikel()))
                .arg(Article_C::ArtikelText(new_word.GetArtikel())) );
        isSame = false;
    } else {
        ui->_update_article_chk->setVisible(false);
    }

    if(old_word.GetDescription() != new_word.GetDescription()) {
        warning_text.append(QString("\n\nOld description is \"%1\" and new description is \"%2\".")
                .arg(old_word.GetDescription())
                .arg(new_word.GetDescription()) );
        isSame = false;
    } else {
        ui->_update_translation_chk->setVisible(false);
    }

    if(isSame) {
        warning_text.append("\n Same words are not allowed.");
        ui->_btn_box->setStandardButtons(QDialogButtonBox::Ok);
    }
    ui->_msg_lbl->setText(warning_text);
    adjustSize();
}

ConflictDlg_C::~ConflictDlg_C()
{
    delete ui;
}

bool ConflictDlg_C::updateArticle() const
{
    return ui->_update_article_chk->isChecked();
}

bool ConflictDlg_C::updateTranslation() const
{
    return ui->_update_translation_chk->isChecked();
}
