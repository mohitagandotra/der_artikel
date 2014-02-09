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
}

/*!
 \brief

 \param file_path
 \return Thema_C
*/
Thema_C *ThemaLoader_C::LoadThema(QString file_path)
{
    Thema_C* thema = 0;
    QFile thema_file(file_path);

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
                            thema = new Thema_C();
                            if(!thema->Read(domNode.toElement())) {
                                delete thema;
                                thema = 0;
                                qDebug()<<"Invalid Thema.";
                            } else {
                                thema->SetFilePath(file_path);
                            }
                            if(_thema_parent) {
                                thema->moveToThread(_thema_parent->thread());
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

    return thema;
}

/*!
 \brief

*/
void ThemaLoader_C::StartLoading()
{
    QThreadPool::globalInstance()->start(this);
}
