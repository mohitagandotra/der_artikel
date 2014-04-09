//******************************************************************************
/*! \file manager.h Implementation of \ref Manager_C
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
#include <QGuiApplication>
#include <QQuickItem>
#include <QQmlContext>
//#include <QMessageBox>
#include <QTimer>
#include <QDebug>

// Interface for this file
#include "manager.h"

#include "data/thema.h"
#include "thema_loader.h"
#include "settings.h"

#include "data/result.h"
#include "version.h"
#include "image_provider.h"

#include "pages/help_page.h"
#include "pages/words_page.h"
#include "pages/stats_page.h"
#include "pages/settings_page.h"
#include "pages/thema_page.h"

#include "log_defines.h"


/*!
 * \brief Construtor
 *
 * \param ref_root_context The root qml context.
 * \param parent Parent object instance
 */
Manager_C::Manager_C(QQmlContext& ref_root_context, QObject *parent) :
    QObject(parent),
    _settings(0),
    _root_context(ref_root_context),
    _root_item(0),
    _current_page(INVALID_PAGE),
    _current_result(0),
    _game_level(EASY),
    _thema_selected(false),
    _image_provider(new ImageProvider_C)
{    
    LOG_DEBUG("Manager_C::Construtor");
    _settings = new Settings_C(this);

    InitPages();

    _thema_model = new ThemaModel_C(this);
    connect(_thema_model,SIGNAL(themaSelectionChanged()), this, SLOT(onThemaSelectionChanged()));

    LoadDefaultThemas();
}

/*!
 \brief Destructor

*/
Manager_C::~Manager_C()
{
    LOG_DEBUG("Manager_C::Destructor");
    delete _current_result;
    delete _thema_model;
}

/*!
 * \brief Changes the current page displayed on the ui. Call this method to change the page.
 * The page will only be changed if the conditions are met for the last page to leave and
 * new page to enter.
 *
 * \param new_page The id of the new page to show.
 */
void Manager_C::setCurrentPage(Manager_C::PageId_TP new_page)
{
    LOG_DEBUG(QString("Manager_C::setCurrentPage : %1").arg(new_page));
    if(_current_page != new_page) {
        PageId_TP old_page = _current_page;

        bool continue_shift = true;
        Page_I* old_page_instance = _page_hash[old_page];
        Page_I* new_page_instance =_page_hash[new_page];

        if(old_page_instance && !old_page_instance->canLeave()) {
            LOG_INFO(QString("Manager :: Can not leave page %1").arg(old_page));
            continue_shift = false;
        }

        if(continue_shift && new_page_instance && !new_page_instance->canEnter()) {
            LOG_INFO(QString("Manager :: Can not enter page %1").arg(new_page));
            continue_shift = false;
        }

        if(continue_shift) {

            _current_page = new_page;

            if(old_page_instance) {
                LOG_INFO(QString("Manager :: Leaving page %1").arg(old_page));
                old_page_instance->leave(_current_page);
                LOG_INFO(QString("Manager :: Left page %1").arg(old_page));
            }

            if(new_page_instance) {
                LOG_INFO(QString("Manager :: Entering page %1").arg(new_page));
                new_page_instance->enter(old_page);
                LOG_INFO(QString("Manager :: Entered page %1").arg(new_page));
            }

            emit currentPageChanged(old_page,new_page);
        }
    }
}

/*!
 * \brief Sets the game level.
 *
 * \param game_level The new game level.
 */
void Manager_C::setGameLevel(Manager_C::GameLevel game_level)
{
    if(_game_level != game_level) {
        LOG_INFO(QString("Manager :: New game level set - %1").arg(game_level));
        _game_level = game_level;
        emit gameLevelChanged();
    }
}

/*!
 \brief

*/
void Manager_C::showMessage(QString title, QString message, int duration, MessageType type)
{
    Q_ASSERT(_settings);
    QMetaObject::invokeMethod(_root_item,"showMessage",Q_ARG(QVariant,title),Q_ARG(QVariant,message),Q_ARG(QVariant,duration),Q_ARG(QVariant,type));
    QTimer::singleShot(_settings->messageAnimTime() + 200,&_message_loop,SLOT(quit()));
    _message_loop.exec();
}

/*!
 \brief

*/
void Manager_C::closeMessage()
{
    Q_ASSERT(_settings);
    QMetaObject::invokeMethod(_root_item,"closeMessage");
    QTimer::singleShot(_settings->messageAnimTime(),&_message_loop,SLOT(quit()));
    _message_loop.exec();
}

/*!
 \brief

*/
QString Manager_C::versionString()
{
    return qApp->applicationVersion();
}

/*!
 * \brief Called when a new thema is loaded. \todo Move it to Thema model.
 *
 * \param new_thema
 */
void Manager_C::OnNewthemaLoaded(Thema_C *new_thema)
{
    Q_ASSERT(new_thema);
    Q_ASSERT(_thema_model);
    LOG_INFO(QString("Manager :: Thema %1 loaded").arg(new_thema->name()));
    new_thema->setParent(this);
    _thema_model->AddThema(new_thema);
    Q_ASSERT(_image_provider);
    _image_provider->AddImage(new_thema->name(),new_thema->GetIcon());
}

/*!
 \brief Called when thema selection is changed

*/
void Manager_C::onThemaSelectionChanged()
{
    Q_ASSERT(_thema_model);
    Thema_C* selected_thema = _thema_model->GetSelectedThema();
    if(selected_thema) {
        _thema_selected = true;
        LOG_INFO(QString("Manager :: Thema selection changed to %1").arg(selected_thema->name()));
    } else {
        _thema_selected = false;
        LOG_INFO(QString("Manager :: Thema selection changed to none"));
    }
    emit themaSelectionStateChanged();
}

/*!
 * \brief This method is called by the qml to set the Page QML item for the correspoding page.
 *
 * \param page_id The id of the page to which the item belongs.
 * \param item The page item.
 */
void Manager_C::setPageItem(Manager_C::PageId_TP page_id, QQuickItem *item)
{
    if(page_id != INVALID_PAGE && item) {
        LOG_DEBUG(QString("Setting QML page item for page %1").arg(page_id));
        _page_items_hash[page_id]._page_item = item;
    }
}

/*!
 * \brief Returns the page item of the page.
 *
 * \param page_id The id of the page.
 * \return QQuickItem The page item of the page.
 */
QQuickItem *Manager_C::pageItem(Manager_C::PageId_TP page_id)
{
    QQuickItem *item = 0;
    if(page_id != INVALID_PAGE) {
        if(_page_items_hash.contains(page_id)) {
            item = _page_items_hash[page_id]._page_item;
        } else {
            LOG_WARN(QString("Qml page item not avaiable for Page Id %1").arg(page_id));
        }
    }
    return item;
}

/*!
 * \brief This method is called by the qml to set the Panel QML item for the correspoding page.
 *
 * \param page_id The id of the page to which the item belongs.
 * \param item The panel item of the page.
 */
void Manager_C::setPanelItem(Manager_C::PageId_TP page_id, QQuickItem *item)
{
    if(page_id != INVALID_PAGE && item) {
        LOG_DEBUG(QString("Setting QML panel item for page %1").arg(page_id));
        _page_items_hash[page_id]._panel_item = item;
    }
}

/*!
 * \brief Returns the panel item of the page.
 *
 * \param page_id The id of the page.
 * \return QQuickItem The panel item of the page.
 */
QQuickItem *Manager_C::panelItem(Manager_C::PageId_TP page_id)
{
    QQuickItem *item = 0;
    if(page_id != INVALID_PAGE) {
        if(_page_items_hash.contains(page_id)) {
            item = _page_items_hash[page_id]._panel_item;
        } else {
            LOG_WARN(QString("Qml panel item not avaiable for Page Id %1").arg(page_id));
        }
    }
    return item;
}

/*!
 \brief This method is called by the qml to set the title QML item for the correspoding page.

 \param page_id The id of the page to which the item belongs.
 \param item The panel item of the page.
*/
void Manager_C::setTitleItem(Manager_C::PageId_TP page_id, QQuickItem *item)
{
    if(page_id != INVALID_PAGE && item) {
        LOG_DEBUG(QString("Setting QML title item for page %1").arg(page_id));
        _page_items_hash[page_id]._title_item = item;
    }
}

/*!
 * \brief Returns the title item of the page.
 *
 * \param page_id The id of the page.
 * \return QQuickItem The title item of the page.
 */
QQuickItem *Manager_C::titleItem(Manager_C::PageId_TP page_id)
{
    QQuickItem *item = 0;
    if(page_id != INVALID_PAGE ) {
        if(_page_items_hash.contains(page_id)) {
        item = _page_items_hash[page_id]._title_item;
        } else {
            LOG_WARN(QString("Qml title item not avaiable for Page Id %1").arg(page_id));
        }
    }
    return item;
}

/*!
 * \brief Quits the application
 *
 */
void Manager_C::quit()
{
//    QMessageBox::StandardButton res  =
//            QMessageBox::information(0,tr("Quit"),
//                                     tr("Do you realy want to quit"),
//                                     QMessageBox::Yes,QMessageBox::No);
    //if(res == QMessageBox::Yes) {
        LOG_INFO("Quitting Application");
        QGuiApplication::quit();
    //}
}

/*!
 \brief Start loading the default thema's. The thema present in the system.
*/
void Manager_C::LoadDefaultThemas()
{
    LOG_INFO("Manager :: Loading Thema files");
    _current_result = new Result_C(this);
    Q_ASSERT(_thema_model);
    _thema_model->clear();

    // thema_loader shall be deleted automatically.
    ThemaLoader_C* thema_loader = new ThemaLoader_C(this);
    connect(thema_loader, SIGNAL(themaLoaded(Thema_C*)), this, SLOT(OnNewthemaLoaded(Thema_C*)) );
    thema_loader->startLoading();
}

/*!
 \brief Initializes the Pages. Dependencies are injected.
*
*/
void Manager_C::InitPages()
{
    LOG_DEBUG("Manager_C::InitPages()");
    _page_hash[HELP_PAGE] = new HelpPage_C(*this, _root_context,this);
    _page_hash[WORDS_PAGE] = new WordsPage_C(*this, _root_context,this);
    _page_hash[STATS_PAGE] = new StatsPage_C(*this,_root_context,this);
    _page_hash[SETTINGS_PAGE] = new SettingsPage_C(*this,_root_context,this);
    _page_hash[THEMA_PAGE] = new ThemaPage_C(*this,this);
}

