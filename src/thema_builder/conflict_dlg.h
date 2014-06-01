#ifndef CONFLICT_DLG_H
#define CONFLICT_DLG_H

#include <QDialog>

namespace Ui {
class ConflictDlg_UIC;
}
class Word_C;
class ConflictDlg_C : public QDialog
{
    Q_OBJECT

public:
    explicit ConflictDlg_C(Word_C& old_word, Word_C& new_word, QWidget *parent = 0);
    ~ConflictDlg_C();

public:
    bool updateArticle() const;
    bool updateDescription() const;
    QString extraDescription() const;

protected:
    void moveEvent(QMoveEvent *event);

private slots:
    void onOptionsChanged();

private:
    Ui::ConflictDlg_UIC *ui;
};

#endif // CONFLICT_DLG_H
