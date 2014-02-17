#ifndef THEMA_REPLACE_OPERATION_H
#define THEMA_REPLACE_OPERATION_H

#include <QObject>
#include <QEventLoop>
#include <QUrl>
#include "thema_file_operation.h"
#include "file_downloader.h"

class ThemaReplaceOperation_C : public QObject, public ThemaFileOperation_I
{
    Q_OBJECT
public:
    ThemaReplaceOperation_C(QString local_file_path, QUrl remote_file_url,  int old_experience);

public:
    virtual bool execute();

private slots:
    void onFileDownloadFinished();
    void onFileDownloadAborted();

private:
    FileDownloader_C _file_downloader;
    QString _local_file_path;
    QUrl _remote_file_url;
    int _old_experience;
    QEventLoop _event_loop;
};

#endif // THEMA_REPLACE_OPERATION_H
