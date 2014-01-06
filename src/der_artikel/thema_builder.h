#ifndef THEMA_BUILDER_H
#define THEMA_BUILDER_H



#ifdef ENABLE_THEMA_BUILDER

#include <QWidget>
#include <QSet>

class Thema_C;

namespace Ui {
class ThemaBuilder_C;
}
class QIODevice;
class QAbstractButton;
class QListWidgetItem;
class Word_C;
class QKeyEvent;

class ThemaBuilder_C : public QWidget
{
    Q_OBJECT

enum WordUIState {
    ADD_STATE,
    UPDATE_STATE
};

public:
    explicit ThemaBuilder_C(QWidget *parent = 0);
    ~ThemaBuilder_C();

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);

private slots:
    void OnDlgButtonClicked(QAbstractButton* btn);
    void OnLoad();
    void OnSave();
    void OnExport();
    void OnImport();
    void OnAddClicked();
    void OnWordTextChanged(QString new_text);
    void OnThemaNameChanged(QString new_text);
    void OnItemDoubleClicked(QListWidgetItem *item);
    void OnWordSelectionChanged();
    void OnDelete();

    void InsertAUmlaut();
    void InsertOUmlaut();
    void InsertUUmlaut();
    void InsertEszett();

private:
    void UpdateItem(QListWidgetItem* item);
    bool Write(QIODevice* pDevice);
    bool Export(QIODevice* pDevice);
    bool Import(QIODevice* pDevice);
    bool AddWordToList(Word_C *new_word);
    bool AddWordToThema(Word_C *new_word);
    void ResetUI();
    void UpdateUI();
    void PopulateUI(Thema_C* thema);
    void SetWordUiState(WordUIState new_state);
    void InsertSplText(QString str);
    void SetUmlautUpperCase( bool upper_case);
    QString UserName();

private:
    Ui::ThemaBuilder_C *ui;
    Thema_C* _thema;
    QSet<QString> _words_set;
    QListWidgetItem* _edit_item;
};
#endif // ENABLE_THEMA_BUILDER

#endif // THEMA_BUILDER_H
