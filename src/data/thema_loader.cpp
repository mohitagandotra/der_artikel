#include "thema_loader.h"

#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QThreadPool>
#include <QDebug>
#include "common.h"
#include "version.h"
#include "data/thema.h"
#include "log_defines.h"

/*!
 \brief

 \param thema_parent
 \param parent
*/
ThemaLoader_C::ThemaLoader_C(QObject *thema_parent, QObject *parent) : QObject(parent),
    _thema_parent(thema_parent)
{
    setAutoDelete(false);
}

/*!
 \brief

*/
void ThemaLoader_C::run()
{
    LOG_DEBUG("ThemaLoader_C::run");
    QDir root_thema_dir = ARTIKEL::GetResourcePath("thema");
    QStringList nameFilters;
    nameFilters<<"*.AKL";
    QFileInfoList thema_files= root_thema_dir.entryInfoList(nameFilters,QDir::Files | QDir::NoSymLinks|QDir::NoDotAndDotDot);

    foreach(QFileInfo thema_file, thema_files) {
        Thema_C* thema = LoadThema(thema_file.absoluteFilePath());
        if(thema) {
            emit ThemaLoaded(thema);
        }
    }
    emit finishedLoading();
}

/*!
 \brief

 \param file_path
 \return Thema_C
*/
Thema_C *ThemaLoader_C::LoadThema(QString file_path, bool defered)
{
    Thema_C* thema = new Thema_C();
    if(thema->Read(file_path,defered)) {
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

/*!
 \brief

*/
void ThemaLoader_C::StartLoading()
{
    LOG_INFO(QString("Thema loader :: Starting loading"));
    QThreadPool::globalInstance()->start(this);
}