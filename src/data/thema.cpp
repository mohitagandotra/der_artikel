#include "thema.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QBuffer>
#include <QMessageBox>

#include "version.h"
#include "common.h"

/*!
 \brief

 \param parent
*/
Thema_C::Thema_C(QObject *parent): QObject(parent),
    _text(""),
    _translation(""),
    _experience_points(0),
    _selected(false),
    _state(RUSTY)
{
    _icon =  QPixmap("qrc:/res/resources/thema_generic.png");
}

/*!
 \brief

*/
Thema_C::~Thema_C()
{
    ClearWords();
}

/*!
 \brief

 \param file_path
*/
void Thema_C::SetFilePath(QString file_path)
{
    _file_path = file_path;
}

/*!
 \brief

 \param element
 \return bool
*/
bool Thema_C::Read(const QDomElement &element, bool defered)
{
    bool success = false;
    ClearWords();
    if(!element.isNull()) {
        _text = element.firstChildElement("ThemaText").text();
        success = !_text.isEmpty();

        _translation = element.firstChildElement("Translation").text();
        _author = element.firstChildElement("Author").text();

        QByteArray icon_data = QByteArray::fromBase64(element.firstChildElement("Icon").text().toUtf8());
        UpdateIcon(icon_data);

        bool ok = false;

        QDomElement dom_date_time = element.firstChildElement("LastPlayed");
        if(!dom_date_time.isNull()) {
            qint64 msecs = dom_date_time.text().toLongLong(&ok);
            if(ok) {
                _last_played = QDateTime::fromMSecsSinceEpoch(msecs);
            }
        }

        QDomElement dom_last_updated = element.firstChildElement("LastUpdated");
        if(!dom_last_updated.isNull()) {
            qint64 msecs = dom_last_updated.text().toLongLong(&ok);
            if(ok) {
                _last_updated = QDateTime::fromMSecsSinceEpoch(msecs);
                success = true;
            } else {
                success = false;
            }
        }

        if(success) {
            QDomElement dom_experience = element.firstChildElement("ExperiencePoints");
            if(!dom_experience.isNull()) {
                ok= false;
                int experience = dom_experience.text().toInt(&ok);
                if(ok) {
                    AddExperiencePoints(experience);
                    success = true;
                    if(_last_played.isValid()) {
                        int lapsed_days = _last_played.daysTo(QDateTime::currentDateTime());
                        while( ((lapsed_days--) > 0) ) {
                            // Progressively deduct experience points.
                            switch (_state) {
                            case INERT:
                                DeductExperiencePoints(2);
                                break;
                            case GOLD:
                                DeductExperiencePoints(5);
                                break;
                            case SILVER:
                                DeductExperiencePoints(10);
                                break;
                            default:
                                DeductExperiencePoints(20);
                                break;
                            }
                        }
                    }
                } else {
                    success = false;
                }
            }
        }

        if(success) {
            QDomElement words_root_node = element.firstChildElement("Words");

            QDomNode word_node = words_root_node.firstChild();

            while(!word_node.isNull()) {
                if(defered) {
                    _words.append(0);
                } else {
                    Word_C* word = new Word_C(this);
                    if(!word->Read(word_node.toElement())) {
                        delete word;
                        qDebug()<<"Invalid Word in thema.";
                    } else {
                        _words.append(word);
                    }
                }
                word_node = word_node.nextSibling();
            }
        }

    }


    return success;
}

bool Thema_C::Read(QString thema_file_path, bool defered)
{
    if(thema_file_path.isEmpty()) {
        thema_file_path = _file_path;
    }
    ResetThema();

    bool success = true;
    QFile thema_file(thema_file_path);
    if(thema_file.open(QFile::ReadOnly)) {
        QDataStream data_stream(&thema_file);
        QByteArray xml_data;
        data_stream>>xml_data;
        xml_data = qUncompress(xml_data);
        QDomDocument thema_doc;
        QString error_msg;
        int error_line;
        int error_col;
        if(thema_doc.setContent(xml_data, &error_msg, &error_line, &error_col)) {
            //parse the file and read the thema.
            QDomElement root = thema_doc.firstChildElement("Root");
            QDomAttr versionAttr = root.attributeNode("Version");
            QString versionStr = versionAttr.value();

            bool ok = false;
            int version = versionStr.toInt(&ok);
            if(ok) {
                if(version <= APP_VERSION) {

                    QDomNode domNode = root.firstChild();
                    while (!domNode.isNull()) {
                        if(domNode.nodeName().compare("Thema") == 0) {
                            if(!Read(domNode.toElement(),defered)) {
                                success = false;
                                qDebug()<<"Invalid Thema.";
                            } else {
                                SetFilePath(thema_file_path);
                            }
                        }
                        domNode = domNode.nextSibling();
                    }

                } else {
                    qDebug()<<"Cannot parse thema file. This version is not supported : "<<version;
                    QMessageBox::critical(0, tr("Invalid thema file version."), tr("Cannot parse thema file. This version is not supported : %1.").arg(version));
                }
            }
        } else {
            qDebug()<<"Invalid thema file. "<<error_msg<<" Line : "<<error_line<<" Col: "<<error_col;
            QMessageBox::critical(0, tr("Invalid thema file."), tr("Cannot parse thema file. Check logs for futher details."));
        }

    } else {
        qDebug()<<thema_file.errorString();
    }

    thema_file.close();

    return success;
}


/*!
 \brief

 \param element
 \return bool
*/
bool Thema_C::Write(QDomElement &element)
{
    bool success = false;

    if( !element.isNull()) {
        QDomDocument domDocument = element.ownerDocument();

        if(!domDocument.isNull() && !_text.isEmpty() && !_translation.isEmpty()) {
            QDomElement dom_thema = domDocument.createElement("Thema");

            QDomElement dom_text = domDocument.createElement("ThemaText");
            QDomText text_thema = domDocument.createTextNode(_text);
            dom_text.appendChild(text_thema);
            dom_thema.appendChild(dom_text);

            QDomElement dom_translation = domDocument.createElement("Translation");
            QDomText translation_thema = domDocument.createTextNode(_translation);
            dom_translation.appendChild(translation_thema);
            dom_thema.appendChild(dom_translation);

            QDomElement dom_author = domDocument.createElement("Author");
            QDomText author_text = domDocument.createTextNode(_author);
            dom_author.appendChild(author_text);
            dom_thema.appendChild(dom_author);

            QDomElement dom_icon = domDocument.createElement("Icon");
            QByteArray icon_arr = IconData().toBase64();
            QString icon_data = QString::fromUtf8(icon_arr);
            QDomText icon_text = domDocument.createTextNode(icon_data);
            dom_icon.appendChild(icon_text);
            dom_thema.appendChild(dom_icon);

            QDomElement dom_experience = domDocument.createElement("ExperiencePoints");
            QDomText text_experience = domDocument.createTextNode(QString::number(_experience_points));
            dom_experience.appendChild(text_experience);
            dom_thema.appendChild(dom_experience);

            if(_last_played.isValid()) {
                QDomElement dom_last_played = domDocument.createElement("LastPlayed");
                QDomText text_last_played = domDocument.createTextNode(QString::number(_last_played.toMSecsSinceEpoch()));
                dom_last_played.appendChild(text_last_played);
                dom_thema.appendChild(dom_last_played);
            }

            QDomElement dom_last_updated = domDocument.createElement("LastUpdated");
            QDomText text_last_updated = domDocument.createTextNode(QString::number(_last_updated.toMSecsSinceEpoch()));
            dom_last_updated.appendChild(text_last_updated);
            dom_thema.appendChild(dom_last_updated);

            QDomElement dom_words_root = domDocument.createElement("Words");

            QList<Word_C*>::Iterator iter =_words.begin();
            while(iter != _words.end()) {
                Word_C* word = *iter;
                if(word) {
                    word->Write(dom_words_root);
                }
                ++iter;
            }

            dom_thema.appendChild(dom_words_root);
            element.appendChild(dom_thema);

            success = true;
        }
    }

    return success;
}

/*!
 \brief

 \param file_path
*/
void Thema_C::Save(QString file_path)
{
    QString save_file = file_path.isEmpty() ? _file_path : file_path;

    if(!save_file.isEmpty()) {
        QFile file(save_file);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            if(Write(&file)) {
                _file_path = save_file;
            }
        } else {
            qDebug()<<QString("cannot write file %1:\n%2.") .arg(save_file) .arg(file.errorString());
        }
    }
}

/*!
 \brief

*/
void Thema_C::ClearWords()
{
    foreach(Word_C* word, _words) {
        delete word;
    }
    _words.clear();
}

/*!
 \brief

 \param pDevice
 \return bool
*/
bool Thema_C::Write(QIODevice* pDevice)
{
    QDomDocument domDocument("DerArtikel");

    QDataStream out(pDevice);
    QDomElement root = domDocument.createElement("Root");
    root.setAttribute("Version", QString::number(APP_VERSION));

    Write(root);

    domDocument.appendChild(root);
    QByteArray xml_data = domDocument.toByteArray(4);
    out<<qCompress(xml_data,ARTIKEL::COMPRESSION_LEVEL);
    return true;
}

/*!
 \brief

*/
void Thema_C::UpdateThemaState()
{
    State_TP state;
    if(_experience_points > 499) {
        state = INERT;
    } else if(_experience_points > 249) {
        state = GOLD;
    } else if(_experience_points > 99) {
        state = SILVER;
    } else {
        state= RUSTY;
    }

    if(_state != state) {
        _state = state;
        emit stateChanged();
    }
}

/*!
 \brief

 \return QByteArray
*/
QByteArray Thema_C::IconData() const
{
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream data_stream(&buffer);
    data_stream<<_icon;
    return data;
}

/*!
 \brief

 \param data
*/
void Thema_C::UpdateIcon(QByteArray data)
{
    if(data.size() > 0) {
        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadOnly);
        QDataStream data_stream(&buffer);
        data_stream>>_icon;
    }
}

void Thema_C::ResetThema()
{
    _text = "";
    _translation = "";
    _experience_points = 0;
    _selected = false;
    _state = RUSTY;
    _icon =  QPixmap("qrc:/res/resources/thema_generic.png");
    _last_played = QDateTime();
    _last_updated = QDateTime();
    _file_path = "";
    ClearWords();
}

/*!
 \brief

 \param points
*/
void Thema_C::AddExperiencePoints(int points)
{
    _experience_points += points;
    if(_experience_points < 0) {
        _experience_points = 0;
    }

    if(points !=0) {
        UpdateThemaState();
        emit experiencePointsChanged();
    }
}

/*!
 \brief

 \param points
*/
void Thema_C::DeductExperiencePoints(int points)
{
    AddExperiencePoints(-points);
}

/*!
 \brief

*/
void Thema_C::ClearUserInput()
{
    foreach(Word_C* word, _words) {
        word->SetUserArtikel(Article_C::INVALID);
    }
}

/*!
 \brief

 \param last_played
*/
void Thema_C::setLastPlayed(const QDateTime &Last_played)
{
    _last_played = Last_played;
}

void Thema_C::setLastUpdated(const QDateTime &last_updated)
{
    _last_updated = last_updated;
}

/*!
 \brief

 \param selected
 \param type
*/
void Thema_C::setSelected(bool selected, SelectionType_TP type)
{
    if(_selected != selected) {
        _selected = selected;
        emit selectionChanged(type);
    }
}

/*!
 \brief

 \return unsigned int
*/
unsigned int Thema_C::GetWordCount() const
{
    return (uint)_words.count();
}

/*!
 \brief

 \param index
 \return const Word_C
*/
const Word_C *Thema_C::GetWord(int index) const
{
    Word_C* word = 0;
    if(index > -1 && index < _words.count()) {
        word = _words.at(index);
    }
    return word;
}


