#include "thema_builder.h"
#include "ui_thema_builder.h"

#include <QDomDocument>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QFile>
#include <QShortcut>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

#include "data/thema.h"
#include "common.h"
#include "version.h"
#include "thema_loader.h"


#ifdef ENABLE_THEMA_BUILDER

ThemaBuilder_C::ThemaBuilder_C(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThemaBuilder_C),
    _edit_item(0)
{
    ui->setupUi(this);

    ui->_author_name_edit->setText(UserName());

    _thema = new Thema_C();

    // Connections
    connect(ui->_btn_box, SIGNAL(clicked(QAbstractButton*)), this, SLOT(OnDlgButtonClicked(QAbstractButton*)) );
    connect(ui->_open_btn,SIGNAL(clicked()), this,SLOT(OnLoad()) );
    connect(ui->_save_btn,SIGNAL(clicked()), this,SLOT(OnSave()) );
    connect(ui->_export_data_btn,SIGNAL(clicked()), this,SLOT(OnExport()) );
    connect(ui->_import_data_btn,SIGNAL(clicked()), this,SLOT(OnImport()) );
    connect(ui->_add_btn,SIGNAL(clicked()), this,SLOT(OnAddClicked()) );
    connect(ui->_word_edit,SIGNAL(textChanged(QString)), this, SLOT(OnWordTextChanged(QString)));
    connect(ui->_thema_name_edit,SIGNAL(textChanged(QString)), this, SLOT(OnThemaNameChanged(QString)));
    connect(ui->_word_list,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(OnItemDoubleClicked(QListWidgetItem*)) );
    connect(ui->_word_list,SIGNAL(itemSelectionChanged()), this, SLOT(OnWordSelectionChanged()));
    connect(ui->_delete_btn,SIGNAL(clicked()), this, SLOT(OnDelete()) );

    connect(ui->_a_umlaut_btn, SIGNAL(clicked()), SLOT(InsertAUmlaut()));
    connect(ui->_o_umlaut_btn, SIGNAL(clicked()), SLOT(InsertOUmlaut()));
    connect(ui->_u_umlaut_btn, SIGNAL(clicked()), SLOT(InsertUUmlaut()));
    connect(ui->_eszett_btn, SIGNAL(clicked()), SLOT(InsertEszett()));

    // Shortcut for a & A umlaut chars
    QShortcut* a_umlaut_shortcut =  new QShortcut(this);
    a_umlaut_shortcut->setKey(QKeySequence(Qt::ALT + Qt::Key_A));
    connect(a_umlaut_shortcut, SIGNAL(activated()), SLOT(InsertAUmlaut()));

    QShortcut* caps_a_umlaut_shortcut =  new QShortcut(this);
    caps_a_umlaut_shortcut->setKey(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_A));
    connect(caps_a_umlaut_shortcut, SIGNAL(activated()), SLOT(InsertAUmlaut()));


    // Shortcut for o & O umlaut chars
    QShortcut* o_umlaut_shortcut =  new QShortcut(this);
    o_umlaut_shortcut->setKey(QKeySequence(Qt::ALT + Qt::Key_O));
    connect(o_umlaut_shortcut, SIGNAL(activated()), SLOT(InsertOUmlaut()));

    QShortcut* caps_o_umlaut_shortcut =  new QShortcut(this);
    caps_o_umlaut_shortcut->setKey(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_O));
    connect(caps_o_umlaut_shortcut, SIGNAL(activated()), SLOT(InsertOUmlaut()));


    // Shortcut for u & U umlaut chars
    QShortcut* u_umlaut_shortcut =  new QShortcut(this);
    u_umlaut_shortcut->setKey(QKeySequence(Qt::ALT + Qt::Key_U));
    connect(u_umlaut_shortcut, SIGNAL(activated()), SLOT(InsertUUmlaut()));

    QShortcut* caps_u_umlaut_shortcut =  new QShortcut(this);
    caps_u_umlaut_shortcut->setKey(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_U));
    connect(caps_u_umlaut_shortcut, SIGNAL(activated()), SLOT(InsertUUmlaut()));


    // Shortcut for eszett char
    QShortcut* eszett_shortcut =  new QShortcut(this);
    eszett_shortcut->setKey(QKeySequence(Qt::ALT + Qt::Key_S));
    connect(eszett_shortcut, SIGNAL(activated()), SLOT(InsertEszett()));

    // Disable buttons. Enable only when pre-conditions are met.
    ui->_add_btn->setEnabled(false);
    ui->_save_btn->setEnabled(false);
    ui->_export_data_btn->setEnabled(false);
    ui->_delete_btn->setEnabled(false);
}

ThemaBuilder_C::~ThemaBuilder_C()
{
    delete ui;
    delete _thema;
}

void ThemaBuilder_C::keyPressEvent(QKeyEvent *e)
{
    if(e && e->key() == Qt::Key_Shift) {
        SetUmlautUpperCase(true);
    }
}

void ThemaBuilder_C::keyReleaseEvent(QKeyEvent *e)
{
    if(e && e->key() == Qt::Key_Shift) {
        SetUmlautUpperCase(false);
    }
}

void ThemaBuilder_C::OnDlgButtonClicked(QAbstractButton *btn)
{
    if(ui->_btn_box->standardButton(btn) == QDialogButtonBox::Close) {
        close();
    }
}

void ThemaBuilder_C::OnLoad()
{
    static QString last_open_path = QDir::homePath();
    QString file_path = QFileDialog::getOpenFileName(this,tr("Select file name"),
                                                     last_open_path,
                                                     tr("Thema files (*.AKL);; All files (*.*)"));
    if(!file_path.isEmpty()) {
        ThemaLoader_C loader;
        Thema_C* new_thema = loader.LoadThema(file_path);
        if(new_thema) {
            if(_thema) {
                delete _thema;
            }
            ResetUI();
            _words_set.clear();
            _thema = new_thema;
            PopulateUI(_thema);
            last_open_path = file_path;
        }
    }

}

void ThemaBuilder_C::OnAddClicked()
{
    QString text = ui->_word_edit->text().trimmed();
    QString desc = ui->_desc_edit->text().trimmed();

    Q_ASSERT(!text.isEmpty());
    Q_ASSERT(_thema);

    Article_C::Artikel article = Article_C::NA;
    if(ui->_die_radio->isChecked()) {
        article = Article_C::DIE;
    } else if(ui->_der_radio->isChecked()) {
        article = Article_C::DER;
    } else if(ui->_das_radio->isChecked()) {
        article = Article_C::DAS;
    }

    if(_edit_item) {
        Word_C* edit_word = _edit_item->data(Qt::UserRole).value<Word_C*>();
        edit_word->_artikel = article;
        _words_set.remove(edit_word->_text);
        edit_word->_text = text;
         _words_set.insert(edit_word->_text);
        edit_word->_description = desc;
        UpdateItem(_edit_item);
        SetWordUiState(ADD_STATE);
    } else {
        Word_C* new_word = new Word_C();
        new_word->_artikel = article;
        new_word->_text = text;
        new_word->_description = desc;
        if(!AddWordToThema(new_word)) {
            delete new_word;
        }
    }

    ui->_word_edit->setText("");
    ui->_desc_edit->setText("");
}

void ThemaBuilder_C::OnWordTextChanged(QString new_text)
{
    ui->_add_btn->setEnabled(!new_text.isEmpty());
}

void ThemaBuilder_C::OnThemaNameChanged(QString new_text)
{
    ui->_save_btn->setEnabled(!new_text.isEmpty());
}

void ThemaBuilder_C::OnItemDoubleClicked(QListWidgetItem *item)
{
    if(item)
    {
        Word_C* word = item->data(Qt::UserRole).value<Word_C*>();
        if(word) {
            _edit_item = item;

            ui->_word_edit->setText(word->GetWordText());
            ui->_desc_edit->setText(word->GetDescription());

            switch(word->GetArtikel()) {
            case Article_C::DAS:
                ui->_das_radio->setChecked(true);
                break;
            case Article_C::DER:
                ui->_der_radio->setChecked(true);
                break;
            case Article_C::DIE:
                ui->_die_radio->setChecked(true);
                break;
            case Article_C::NA:
                ui->_na_radio->setChecked(true);
                break;
            default:
                break;
            }

            SetWordUiState(UPDATE_STATE);
        }
    }
}

void ThemaBuilder_C::OnWordSelectionChanged()
{
    QList<QListWidgetItem*> selected_items = ui->_word_list->selectedItems();
    ui->_delete_btn->setEnabled(selected_items.count() > 0);
}

void ThemaBuilder_C::OnDelete()
{
    if(_thema) {
        QList<QListWidgetItem*> selected_items = ui->_word_list->selectedItems();
        foreach( QListWidgetItem* item, selected_items ) {
            if(item)
            {
                if(_edit_item == item) {
                    SetWordUiState(ADD_STATE);

                }
                Word_C* word = item->data(Qt::UserRole).value<Word_C*>();
                if(word) {
                    _thema->_words.remove(_thema->_words.indexOf(word));
                    delete word;
                    delete item;
                }
            }
        }
        UpdateUI();
    }
}

void ThemaBuilder_C::InsertAUmlaut()
{
    if(QApplication::keyboardModifiers() & Qt::SHIFT) {
        InsertSplText("Ä");
    } else {
        InsertSplText("ä");
    }
}

void ThemaBuilder_C::InsertOUmlaut()
{
    if(QApplication::keyboardModifiers() & Qt::SHIFT) {
        InsertSplText("Ö");
    } else {
        InsertSplText("ö");
    }
}

void ThemaBuilder_C::InsertUUmlaut()
{
    if(QApplication::keyboardModifiers() & Qt::SHIFT) {
        InsertSplText("Ü");
    } else {
        InsertSplText("ü");
    }
}

void ThemaBuilder_C::InsertEszett()
{
    InsertSplText("ß");
}

void ThemaBuilder_C::UpdateItem(QListWidgetItem *item)
{
    if(item) {
        Word_C* word = item->data(Qt::UserRole).value<Word_C*>();
        if(word) {
            QString listItemText;
            if(word->_artikel == Article_C::NA) {
                listItemText = word->_text;
            } else {
                listItemText = QString("%1 %2").arg(Article_C::ArtikelText(word->_artikel)).arg(word->_text);
            }
            item->setText(listItemText);
        }
    }
}

void ThemaBuilder_C::OnSave()
{
    static QString last_save_path = QDir::homePath() + QDir::separator() + "untitled.AKL";
    QString save_file = QFileDialog::getSaveFileName(this,tr("Select file name"),
                                                     last_save_path,
                                                     tr("Thema files (*.AKL);; All files (*.*)"));

    if(!save_file.isEmpty()) {
        QFile file(save_file);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            if(Write(&file)) {
                last_save_path = save_file;
            }
        } else {
            qDebug()<<QString("cannot write file %1:\n%2.") .arg(save_file) .arg(file.errorString());
        }
    }
}

void ThemaBuilder_C::OnExport()
{
    static QString last_save_path = QDir::homePath() + QDir::separator() + "untitled.csv";
    QString save_file = QFileDialog::getSaveFileName(this,tr("Select file name"),
                                                     last_save_path,
                                                     tr("CSV files (*.csv);; All files (*.*)"));
    if(!save_file.isEmpty()) {
        QFile file(save_file);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            if(Export(&file)) {
                last_save_path = save_file;
            } else {
                QMessageBox::critical(this,tr("Export failed"), tr("Invalid file or permissions"));
            }
        } else {
            qDebug()<<QString("cannot write file %1:\n%2.") .arg(save_file) .arg(file.errorString());
        }
    }
}

void ThemaBuilder_C::OnImport()
{
    static QString last_open_path = QDir::homePath();
    QString file_path = QFileDialog::getOpenFileName(this,tr("Select file to import"),
                                                     last_open_path,
                                                     tr("CSV files (*.csv);; All files (*.*)"));

    if(!file_path.isEmpty()) {
        QFile file(file_path);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            if(Import(&file)) {
                last_open_path = file_path;
            } else {
                QMessageBox::critical(this,tr("Import failed"), tr("Invalid file or format"));
            }
        } else {
            qDebug()<<QString("cannot Read file %1:\n%2.") .arg(file_path) .arg(file.errorString());
        }
    }
}

bool ThemaBuilder_C::Write(QIODevice* pDevice)
{
    Q_ASSERT(_thema);

    QDomDocument domDocument("DerArtikel");

    QTextStream out(pDevice);
    QDomElement root = domDocument.createElement("Root");
    root.setAttribute("Version", QString::number(APP_VERSION));

    _thema->_text = ui->_thema_name_edit->text().trimmed();
    _thema->_translation =  ui->_thema_tr_name_edit->text().trimmed();
    _thema->_author = ui->_author_name_edit->text().trimmed();
    _thema->Write(root);

    domDocument.appendChild(root);
    domDocument.save(out, 4);

    return true;
}

bool ThemaBuilder_C::Export(QIODevice *pDevice)
{
    Q_ASSERT(_thema);
    bool success = false;
    if(pDevice) {
        QTextStream out(pDevice);
        foreach(Word_C* word, _thema->_words) {
            out<<word->_artikel<<";"<<word->_text<<";"<<word->_description<<"\n";
        }

        pDevice->close();
        success = true;
    }
    return success;
}

bool ThemaBuilder_C::Import(QIODevice *pDevice)
{
    bool success = false;
    if(pDevice) {
        QTextStream stream(pDevice);
        QString line;
        int line_count = 0;
        do {
            line = stream.readLine();
            ++line_count;
            QStringList word_data = line.split(";");
            if(word_data.count() > 2) {

                bool ok = false;
                int article_code =  word_data.at(0).toInt(&ok);
                Article_C::Artikel article = Article_C::INVALID;
                if(ok && (article_code >= Article_C::DER) && (article_code< Article_C::INVALID)) {
                    article = (Article_C::Artikel)article_code;
                } else {
                    qDebug()<<"Invalid article code, Line # "<<line_count;
                    continue;
                }
                Word_C* new_word = new Word_C();
                new_word->_artikel = article;
                new_word->_text = word_data.at(1);
                new_word->_description = word_data.at(2);
                if(!AddWordToThema(new_word)) {
                    delete new_word;
                }
            }
        } while (!line.isNull());
        pDevice->close();
        success = true;
    }
    return success;
}

bool ThemaBuilder_C::AddWordToList(Word_C* new_word)
{
    bool success = false;
    if(new_word) {
        QListWidgetItem* list_item = new QListWidgetItem(ui->_word_list);
        list_item->setData(Qt::UserRole,QVariant::fromValue<Word_C*>(new_word));
        UpdateItem(list_item);
        ui->_word_list->addItem(list_item);
        UpdateUI();
        success = true;
    }
    return success;
}

bool ThemaBuilder_C::AddWordToThema(Word_C *new_word)
{
    bool success = false;
    if(new_word) {
        if(_words_set.contains(new_word->_text)) {
            QMessageBox::warning(this,tr("Duplicate"), tr("%1 already exists. Duplicate words are not allowed.").arg(new_word->_text));
        } else {
            AddWordToList(new_word);
            _thema->_words.append(new_word);
            _words_set.insert(new_word->_text);
            success = true;
        }
    }
    return success;
}

void ThemaBuilder_C::ResetUI()
{
    ui->_thema_name_edit->setText("");
    ui->_thema_tr_name_edit->setText("");
    ui->_author_name_edit->setText("");
    ui->_word_edit->setText("");
    ui->_desc_edit->setText("");
    ui->_na_radio->setChecked(true);
    ui->_word_list->clear();
    SetWordUiState(ADD_STATE);
}

void ThemaBuilder_C::UpdateUI()
{
    if(ui->_word_list->count() > 0) {
        ui->_export_data_btn->setEnabled(true);
    } else {
        ui->_export_data_btn->setEnabled(false);
    }
}

void ThemaBuilder_C::PopulateUI(Thema_C *thema)
{
    if(thema) {
        ui->_thema_name_edit->setText(thema->GetText());
        ui->_thema_tr_name_edit->setText(thema->GetTrText());
        if(thema->Author().isEmpty()) {
            ui->_author_name_edit->setText(UserName());
        } else {
            ui->_author_name_edit->setText(thema->Author());
        }
        foreach(Word_C* word, thema->_words) {
            AddWordToList(word);
        }
    }
}

void ThemaBuilder_C::SetWordUiState(ThemaBuilder_C::WordUIState new_state)
{
    if(new_state == ADD_STATE) {
        _edit_item = 0;
        ui->_add_btn->setText(tr("Add"));
        ui->_word_edit->setText("");
        ui->_desc_edit->setText("");
        ui->_na_radio->setChecked(true);
    } else {
        ui->_add_btn->setText(tr("Update"));
    }
}

void ThemaBuilder_C::InsertSplText(QString str)
{
    QWidget* widget = QApplication::focusWidget();
    QLineEdit* editor = dynamic_cast<QLineEdit*>(widget);
    if(editor) {
        editor->insert(str);
    }
}

void ThemaBuilder_C::SetUmlautUpperCase(bool upper_case)
{
    if(upper_case) {
        ui->_a_umlaut_btn->setText("Ä");
        ui->_o_umlaut_btn->setText("Ö");
        ui->_u_umlaut_btn->setText("Ü");
    } else {
        ui->_a_umlaut_btn->setText("ä");
        ui->_o_umlaut_btn->setText("ö");
        ui->_u_umlaut_btn->setText("ü");
    }
}

QString ThemaBuilder_C::UserName()
{
    QString user_name;
    user_name = getenv("USER");
    if(user_name.isEmpty()) {
        user_name = getenv("USERNAME");
    }
    if(user_name.isEmpty()) {
        user_name = "Anonymous Andy";
    }
    return user_name;
}

#endif
