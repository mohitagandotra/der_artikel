//******************************************************************************
/*! \file settings.cpp Implementation of \ref Settings_C
 *
 *  \author Vikas Pachdha
 *
 *  \copyright Copyright (C) 2014 Vikas Pachdha, Mohita Gandotra.
 * Contact: http://www.vikaspachdha.com
 *
 * This file is part of the application der_artikel.
 *
 *  \copyright GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *  \copyright GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 *
 ******************************************************************************/
// System includes
#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

// Interface for this file
#include "settings.h"

// Framework and lib includes
#include "data/common.h"
#include "log4qt/log_defines.h"


static const int MIN_WORD_MSG_TIME = 500; /*! Minimum time for info mode in word's page*/
static const int MAX_WORD_MSG_TIME = 5000; /*! Maximum time for info mode in word's page */
static const int MSG_ANIM_TIME = 400; /*! Message bar default animation time */


//******************************************************************************
/*! \brief Constructor.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] parent : Parent object instance.
 ******************************************************************************/
Settings_C::Settings_C(QObject *parent) :
    QObject(parent),
    _current_language(ENGLISH),
    _sound_level(0.1),
    _word_message_time(1200)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    loadSettings();
    LOG_DEBUG("Settings_C::constructor");
    updateLangauge();
    _thema_remote_path = "vystosi.com/der_artikel/thema";
#ifdef NO_GRAPHICAL_EFFECTS
    LOG_INFO("No graphical effects");
#else
    LOG_INFO("Graphical effects enabled");
#endif
}

Settings_C::~Settings_C()
{
    saveSettings();
}

//******************************************************************************
/*! \brief Sets the language used for UI.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] language : New language selected.
 *
 *  \see \ref Settings_C::Language_TP
 ******************************************************************************/
void Settings_C::setLanguage(Settings_C::Language_TP language)
{
    if(_current_language != language) {
        LOG_INFO(QString("Language id changed to %1").arg(language));
        _current_language = language;
        updateLangauge();
        emit languageChanged();
    }
}

//******************************************************************************
/*! \brief Called when language is changed.Loads new translations as per the
 *  selected language.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void Settings_C::updateLangauge()
{
    QString language_code = "en";
    switch( _current_language ) {
    case GERMAN:
        QLocale::setDefault(QLocale::German);
        language_code = "de";
        break;
    case HINDI:
        QLocale::setDefault(QLocale::Hindi);
        language_code = "hin";
        break;
    default:
        QLocale::setDefault(QLocale::English);
        break;
    }

    foreach (QTranslator* translator, _installed_translators) {
        QGuiApplication::removeTranslator(translator);
        delete translator;
    }
    _installed_translators.clear();

    QDir lang_dir = ARTIKEL::GetResourcePath("languages");
    QStringList nameFilters;
    nameFilters<<"*.qm";

    QFileInfoList lang_files = lang_dir.entryInfoList(nameFilters,QDir::Files | QDir::NoSymLinks|QDir::NoDotAndDotDot);
    foreach(QFileInfo file, lang_files) {
        if(file.baseName().endsWith(language_code,Qt::CaseInsensitive)) {
            QTranslator* translator = new QTranslator;
            if(translator->load(file.absoluteFilePath())) {
                QGuiApplication::installTranslator(translator);
                _installed_translators<<translator;
            }
        }
    }

}

//******************************************************************************
/*! \brief Saves user settings to INI file.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void Settings_C::saveSettings()
{
    QSettings settings;
    settings.beginGroup("appSettings");
    settings.setValue("language",_current_language);
    settings.setValue("soundLevel",_sound_level);
    settings.setValue("wordMessageTime",_word_message_time);
    settings.setValue("remotePath",_thema_remote_path);
    settings.endGroup();
}

//******************************************************************************
/*! \brief Loads user settings from INI file.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void Settings_C::loadSettings()
{
    QSettings settings;
    settings.beginGroup("appSettings");
    _current_language = (Language_TP)settings.value("language",ENGLISH).toInt();
    _sound_level = settings.value("soundLevel",0.1).toDouble();
    _word_message_time = settings.value("wordMessageTime",1200).toInt();
    _thema_remote_path = settings.value("remotePath","www.vystosi.com/der_artikel/themas").toString();
    settings.endGroup();
}

//******************************************************************************
/*! \brief Sets sound level.
 *
 *  \details Value range between 0 to 1, 0 being minimum and 1 being maximum.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] sound_level : New sound level.
 ******************************************************************************/
void Settings_C::setSoundLevel(double sound_level)
{
    if(qAbs(sound_level-_sound_level) > 0.001 ) {
        _sound_level = sound_level;
        emit soundLevelChanged();
    }
}

//******************************************************************************
/*! \brief Returns word message time string.
 *
 *  \details The string is used in UI.
 *
 *  \author Vikas Pachdha
 *
 *  \return QString : Word message time string.
 ******************************************************************************/
QString Settings_C::wordMsgTimeStr() const
{
    return QString::number(_word_message_time);
}

//******************************************************************************
/*! \brief Sets word message time string.
 *
 *  \details The string is used in UI.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] new_time : New word message time in msecs.
 ******************************************************************************/
void Settings_C::setWordMsgTimeStr(QString new_time_str)
{
    bool ok = false;
    int msecs = new_time_str.toInt(&ok);
    if(ok) {
        setWordMsgTime(msecs);
    }
}

//******************************************************************************
/*! \brief Sets word message time. Overrides default value.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] new_time : New word message time in msecs.
 ******************************************************************************/
void Settings_C::setWordMsgTime(int new_time)
{
    if(new_time < 500) {
        new_time = 500;
    }
    if(new_time > 5000) {
        new_time = 5000;
    }
    if(_word_message_time != new_time) {
        _word_message_time = new_time;
        emit wordMsgTimeChanged();
    }
}

//******************************************************************************
/*! \brief Minimimum time that can be set for info mode in word's page.
 *
 *  \author Vikas Pachdha
 *
 *  \return type : Minimum word message time in msecs.
 ******************************************************************************/
int Settings_C::minWordMsgTime() const
{
    return MIN_WORD_MSG_TIME;
}

//******************************************************************************
/*! \brief Maximimum time that can be set for info mode in word's page.
 *
 *  \author Vikas Pachdha
 *
 *  \return type : Maximum word message time in msecs.
 ******************************************************************************/
int Settings_C::maxWordMsgTime() const
{
    return MAX_WORD_MSG_TIME;
}

//******************************************************************************
/*! \brief Message animation time.
 *
 *  \author Vikas Pachdha
 *
 *  \return int : Animation time for message bar in msecs.
 ******************************************************************************/
int Settings_C::messageAnimTime() const
{
    return MSG_ANIM_TIME;
}

//******************************************************************************
/*! \brief Sets remote thema file's folder.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] url_str : Remote folder path.
 ******************************************************************************/
void Settings_C::setThemaRemotePath(QString url_str)
{
    if(url_str != _thema_remote_path) {
        LOG_INFO(QString("Remote thema folder path changed to %1").arg(url_str));
        _thema_remote_path = url_str;
        emit themaRemotePathChanged();
    }
}

//******************************************************************************
/*! \brief Returns true if graphical effects are to be used in QML. Windows seems
 *  to have problem with OpenGl.
 *
 *  \author Vikas Pachdha
 *
 *  \return bool : Returns true if graphical effects are to be used, false otherwise.
 ******************************************************************************/
bool Settings_C::enableEffects() const
{
#ifdef NO_GRAPHICAL_EFFECTS
    return false;
#else
    return true;
#endif
}

