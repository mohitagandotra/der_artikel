//******************************************************************************
/*! \file settings.h Declaration of \ref Settings_C
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
#ifndef SETTINGS_H
#define SETTINGS_H

// System includes
#include <QObject>
#include <QHash>
#include <QUrl>

// Forward declaration
class QTranslator;

//******************************************************************************
/*! \brief Application settings.
 *
 *  \details Settings are serialized and loaded on application start.
 *
 *  \author Vikas Pachdha
 *
 *  \todo Create template settings like silent, custom and loud.
 *  \todo Sound levels not working.
 ******************************************************************************/
class Settings_C : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Language_TP language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString i18n_empty_string READ i18EmptyString NOTIFY languageChanged)
    Q_PROPERTY(double sound_level READ soundLevel WRITE setSoundLevel NOTIFY soundLevelChanged)
    Q_PROPERTY(QString word_msg_time_str READ wordMsgTimeStr WRITE setWordMsgTimeStr NOTIFY wordMsgTimeChanged)
    Q_PROPERTY(int word_msg_time READ wordMsgTime WRITE setWordMsgTime NOTIFY wordMsgTimeChanged)
    Q_PROPERTY(int min_word_msg_time READ minWordMsgTime CONSTANT)
    Q_PROPERTY(int max_word_msg_time READ maxWordMsgTime CONSTANT)
    Q_PROPERTY(int message_anim_time READ messageAnimTime CONSTANT)
    Q_PROPERTY(QString thema_remote_path READ themaRemotePath WRITE setThemaRemotePath NOTIFY themaRemotePathChanged)
    Q_PROPERTY(bool enable_effects READ enableEffects CONSTANT)
    Q_ENUMS(Language_TP)

public:

    //! UI languages.
    enum Language_TP {
        INVALID_LANGUAGE,
        ENGLISH,
        GERMAN,
        HINDI
    };

public:
    explicit Settings_C(QObject *parent = 0);
    ~Settings_C();

public:

    //! Returns current language selected.
    Language_TP language() const { return _current_language; }
    void setLanguage(Language_TP language);

    //! Current sound level.
    double soundLevel() const { return _sound_level; }
    void setSoundLevel(double sound_level);

    QString wordMsgTimeStr() const;
    void setWordMsgTimeStr(QString new_time_str);

    //! Message time for info mode in Word's page.
    int wordMsgTime() const {return _word_message_time; }
    void setWordMsgTime(int new_time);

    int minWordMsgTime() const;
    int maxWordMsgTime() const;

    int messageAnimTime() const;

    //! Remote server path for updating thema files.
    QString themaRemotePath() const { return _thema_remote_path; }
    void setThemaRemotePath(QString url_str);

    bool enableEffects() const;

    void saveSettings();
    void loadSettings();

signals:
    void languageChanged();
    void soundLevelChanged();
    void wordMsgTimeChanged();
    void themaRemotePathChanged();

private:
    void updateLangauge();
    //! Returns empty string. Hack used to language change in QML
    QString i18EmptyString() { return ""; }


private:
    //! Selected language.
    Language_TP _current_language;
    //! Installed language translators.
    QList <QTranslator*> _installed_translators;
    //! Sound level.
    double _sound_level;
    //! Message time for info mode.
    int _word_message_time;
    //! Remote thema file's folder.
    QString _thema_remote_path;
};

#endif // SETTINGS_H
