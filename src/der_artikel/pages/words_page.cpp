//******************************************************************************
/*! \file words_page.cpp Implementation of \ref SomeClass
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
#include <QQmlContext>
#include <QQuickItem>

// Interface for this file
#include "words_page.h"

// Framework and lib includes
#include "log4qt/log_defines.h"

// Project includes
#include "algo/result_algo.h"
#include "algo/easy_result_algo.h"
#include "algo/moderate_result_algo.h"
#include "algo/strict_result_algo.h"
#include "message_bar.h"
#include "settings.h"
#include "thema_model.h"

//******************************************************************************
/*! \brief Constructor
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] page_manager : \ref CPageManager_C instance.
 *  \param[in] root_context : QML root context instance.
 *  \param[in] settings : Application's \ref Settings_C  instance.
 *  \param[in] parent : Parent instance.
 ******************************************************************************/
WordsPage_C::WordsPage_C(Manager_C &page_manager, QQmlContext &root_context, Settings_C& settings, QObject *parent):
    Page_C(Manager_C::WORDS_PAGE,page_manager, parent),
    _root_context(root_context),
    _settings(settings),
    _info_mode(false),
    _selected_article(Article_C::INVALID),
    _result_algo(0)
{
    _root_context.setContextProperty("words_page",this);
    setSelectedArticle(Article_C::DER);
}

//******************************************************************************
/*! \brief Called when page enters. Selected thema is loaded and words are added.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] prev_page_id : Previous page id.
 ******************************************************************************/
void WordsPage_C::enter(Manager_C::PageId_TP prev_page_id)
{
    Q_UNUSED(prev_page_id)
    Thema_C* thema = _page_manager.themaModel()->GetSelectedThema();
    Q_ASSERT(thema);

    MessageBar_C::showMsgAsync(tr("Loading thema ..."),"");

    thema->Read("",false);

    if(_page_manager.gameLevel() == Manager_C::PRACTICE) {
        // Add words to page.
        addWords(thema,true);
        setInfoMode(true);
    } else {
        setInfoMode(false);
        createResultAlgo();
        Q_ASSERT(_result_algo);

        // Calculate play time
        int play_time = _result_algo->playTime(*thema);
        LOG_INFO(QString("Word page :: Play time set to %1").arg(play_time));
        QQuickItem* title_item = _page_manager.titleItem(_page_id);
        if(title_item) {
            title_item->setProperty("play_time",play_time);
            title_item->setProperty("timer_running",true);
        }

        // Add words to page.
        addWords(thema);
    }
    MessageBar_C::closeMsg();
}

//******************************************************************************
/*! \brief Called when page leaves.
 *
 *  \details If next page is Manager_C::RESULT_PAGE, result is calculated. word
 *  page's timer is stopped and user input is flushed.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] next_page_id : Next page id.
 ******************************************************************************/
void WordsPage_C::leave(Manager_C::PageId_TP next_page_id)
{
    if(next_page_id==Manager_C::RESULT_PAGE) {
        calculateResult();
    }
    clearWordItems();

    Thema_C* thema = _page_manager.themaModel()->GetSelectedThema();
    Q_ASSERT(thema);
    thema->clearUserInput();
    thema->clearWords();

    QQuickItem* title_item = _page_manager.titleItem(_page_id);
    if(title_item) {
        title_item->setProperty("timer_running",false);
    }
}

//******************************************************************************
/*! \brief Sets info mode state.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] info_mode : True to switch info mode on, false otherwise.
 ******************************************************************************/
void WordsPage_C::setInfoMode(bool info_mode)
{
    if(_info_mode != info_mode) {
        _info_mode = info_mode;
        if(_info_mode) {
            LOG_INFO("Word page :: Info mode set");
            setSelectedArticle(Article_C::NA);
        }
        emit infoModeChanged();
    }
}

//******************************************************************************
/*! \brief Sets the selected article.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] article : Selected article.
 ******************************************************************************/
void WordsPage_C::setSelectedArticle(Article_C::Artikel article)
{
    if(_selected_article != article) {
        _selected_article = article;
        LOG_INFO(QString("Word page :: User artikel changed to %1").arg(_selected_article));
        if(_selected_article != Article_C::NA) {
            setInfoMode(false);
        }
        emit selectedArticleChanged();
    }
}

//******************************************************************************
/*! \brief Called when word item on word's page is clicked.
 *
 *  \details User input is registered for the corresponding \ref Word_c instance.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void WordsPage_C::OnWordClicked()
{
    QObject* word_item = sender();
    Q_ASSERT(word_item);
    Word_C* word = _item_word_hash[word_item];

    if(word) {
        LOG_INFO(QString("Word page :: Word clicked %1").arg(word->wordText()));
        if(_info_mode) {
            MessageBar_C::showMsg(word->wordText(),word->description(),_settings.wordMsgTime());
        } else {
            word->setUserArtikel(_selected_article);
        }
    }
}

//******************************************************************************
/*! \brief Helper method to add words to QML.
 *
 *  \details In case of Manager_C::PRACTICE mode all words are assigned correct articles.
 *  \ref Word_C and its QML association is also updated.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] thema : Selected thema.
 *  \param[in] practice_mode : True if practice mode is applicable.
 ******************************************************************************/
void WordsPage_C::addWords(const Thema_C* thema, bool practice_mode)
{
    QList<Word_C*> words = thema->words();
    LOG_INFO("Word page :: Adding words");
    srand(QDateTime::currentMSecsSinceEpoch());
    while (words.count() > 0) {
        int index = rand()%words.count();
        Word_C* word = words.takeAt(index);
        if(practice_mode) {
            word->setUserArtikel(word->artikel());
        }
        QObject* word_item = addWord(*word);
        Q_ASSERT(word_item);
        _item_word_hash[word_item] = word;
        connect(word_item, SIGNAL(wordClicked()), this, SLOT(OnWordClicked()) );
    }
}

//******************************************************************************
/*! \brief Helper method to add a word to QML.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] word : Word instance to add to QML.
 *
 *  \return QObject* : Created QML item in word's page.
 ******************************************************************************/
QObject *WordsPage_C::addWord(Word_C& word)
{
    QVariant returned_value;
    LOG_DEBUG(QString("Word page :: Adding word %1").arg(word.wordText()));
    QMetaObject::invokeMethod(pageItem(), "addWord",
                              Q_RETURN_ARG(QVariant, returned_value),
                              Q_ARG(QVariant, QVariant::fromValue<QObject*>(&word)),
                              Q_ARG(QVariant, word.wordText()),
                              Q_ARG(QVariant, word.description()));
    QObject* word_item = returned_value.value<QObject*>();
    return word_item;
}

//******************************************************************************
/*! \brief Helper method to clear QML word items.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void WordsPage_C::clearWordItems()
{
    LOG_INFO("Word page :: Clearing word items.");
    foreach(QObject* word_item, _item_word_hash.keys()) {
        delete word_item;
    }
    _item_word_hash.clear();
}

//******************************************************************************
/*! \brief Helper method to create the result algo as per the selected difficulty
 *  level in thema page.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void WordsPage_C::createResultAlgo()
{
    LOG_INFO("Word page :: Creating result algo.");
    if(_result_algo) {
        delete _result_algo;
        _result_algo = 0;
    }
    switch (_page_manager.gameLevel()) {
        case Manager_C::EASY:
            _result_algo = new EasyResultAlgo_C();
            break;
        case Manager_C::MODERATE:
            _result_algo = new ModerateResultAlgo_C();
            break;
        case Manager_C::EXPERT:
            _result_algo = new StrictResultAlgo_C();
            break;
        default:
            break;
    }
}

//******************************************************************************
/*! \brief Helper method to calculate the result after the game finishes.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void WordsPage_C::calculateResult()
{
    Thema_C* current_thema = _page_manager.themaModel()->GetSelectedThema();
    if(current_thema) {
        LOG_INFO(QString("Word page :: Calculating result for %1").arg(current_thema->name()));
        _result_algo->calculate(*current_thema,*(_page_manager.currentResult()));
        current_thema->setLastPlayed(QDateTime::currentDateTimeUtc());
        current_thema->Save();
    }
}



