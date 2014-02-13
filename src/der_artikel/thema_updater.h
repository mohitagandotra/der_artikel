#ifndef THEMA_UPDATER_H
#define THEMA_UPDATER_H

#include <QObject>
#include <QUrl>
#include <QVector>
#include <QHash>

#include "file_downloader.h"

class Thema_C;
class Settings_C;

class ThemaUpdater_C : public QObject
{
    Q_OBJECT
    Q_ENUMS(UpdateResponse_TP)
public:

    enum UpdateResponse_TP {
        UPDATE_AVAILABLE=1,
        UPDATE_NOT_AVAILABLE,
        UPDATE_ERROR
    };

    enum FileType_TP {
        IS_INDEX = 1,
        IS_THEMA,
        NONE
    };

    enum FileOperation_TP {
        ADD = 1,
        REPLACE,
        REMOVE,
    };

    struct FileOperationData_TP {
        public:
            QString _local_file_path;
            QUrl _remote_file_url;
            FileOperation_TP _operation;
            int _old_experience;
    };

public:
    explicit ThemaUpdater_C(Settings_C& settings, QObject *parent = 0);

public:
    Q_INVOKABLE void checkUpdate();

private slots:
    void onFileDownloadFinished();
    void onFileDownloadAborted();
    void onNewThemaLoaded(Thema_C *new_thema);
    void onBuildLocalDataFinished();

signals:
    void updateResponse(UpdateResponse_TP response_code);

private:
    bool ParseIndexFile(QByteArray file_data, QHash<QString, QDateTime>& parsed_data);
    bool executeOperations();
    void buildLocalData();
    void reset();

private:
    Settings_C& _settings;
    FileDownloader_C _file_downloader;
    FileType_TP _downloading_file_type;
    QHash<QString, QDateTime> _remote_file_data;
    QVector<FileOperationData_TP> _file_operations;
};

#endif // THEMA_UPDATER_H
