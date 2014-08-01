//******************************************************************************
/*! \file thema_loader.cpp Implementation of \ref ThemaLoader_C
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
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QThreadPool>

// Declaration
#include "thema_loader.h"

// Project includes
#include "common.h"
#include "data/thema.h"
#include "log4qt/log_defines.h"
#include "version.h"


//******************************************************************************
/*! \brief Constructor.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] thema_parent : Instance of the parent object to thema objects.
 *  \param[in] parent : Instance of parent object.
 ******************************************************************************/
ThemaLoader_C::ThemaLoader_C(QObject *thema_parent, QObject *parent) : QObject(parent),
    _thema_parent(thema_parent)
{
    setAutoDelete(false);
}

//******************************************************************************
/*! \brief Starting point of the thema loader thread.
 *
 *  \details All thema files in the defailt directory are loaded and \ref
 *  ThemaLoader_C::themaLoaded "themaLoaded" signal is emitted.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void ThemaLoader_C::run()
{
    LOG_DEBUG("ThemaLoader_C::run");
    QDir root_thema_dir(_root_dir);
    QStringList nameFilters;
    nameFilters<<"*.AKL";
    QFileInfoList thema_files= root_thema_dir.entryInfoList(nameFilters,QDir::Files | QDir::NoSymLinks|QDir::NoDotAndDotDot);
    int file_count = thema_files.count();
    int files_loaded_count = 0;
    emit updateProgress(0.0);
    foreach(QFileInfo thema_file, thema_files) {
        Thema_C* thema = loadThema(thema_file.absoluteFilePath());
        if(thema) {
            emit themaLoaded(thema);
        }
        ++files_loaded_count;
        emit updateProgress( (double)files_loaded_count/file_count );
    }
    emit finishedLoading();
}

//******************************************************************************
/*! \brief Sets the root directory where all thema files are located and shall be
 *  loaded after calling \ref ThemaLoader_C::startLoading.
 *
 *  \details Path is not checked for existence thus path must exist.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] root_thema_dir_path : Root directory path where all thema files are located
 *
 ******************************************************************************/
void ThemaLoader_C::setRootThemaDir(QString root_thema_dir_path)
{
    _root_dir = root_thema_dir_path;
}

//******************************************************************************
/*! \brief Loads a thema from the thema file and returns the \ref Thema_C "thema"
 *  instance.
 *
 *  \details Null is returned if thema loading fails.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] file_path : Absolute path to thema file.
 *  \param[in] defered : True if \ref Word_C "words" loading is to be defered.
 *
 *  \return Thema_C* : Instance of loaded \ref Thema_C "thema".
 ******************************************************************************/
Thema_C *ThemaLoader_C::loadThema(QString file_path, bool defered)
{
    Thema_C* thema = new Thema_C();
    if(thema->read(file_path,defered)) {
        LOG_INFO(QString("Thema loader :: Loaded thema %1").arg(file_path));
        if(_thema_parent) {
            thema->moveToThread(_thema_parent->thread());
        }
    } else {
        LOG_WARN(QString("Thema loader :: Loading failed thema %1").arg(file_path));
        delete thema;
        thema = 0;
    }
    return thema;
}

//******************************************************************************
/*! \brief Call this method to start thema loading from default directory asynchronously.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] auto_delete : True to auto delete the instance after thema loading is
 *  finished. Default is true
 ******************************************************************************/
void ThemaLoader_C::startLoading(bool auto_delete)
{
    setAutoDelete(auto_delete);
    LOG_INFO(QString("Thema loader :: Starting loading"));
    QThreadPool::globalInstance()->start(this);
}
