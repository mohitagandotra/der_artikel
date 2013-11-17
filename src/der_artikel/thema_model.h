#ifndef THEMA_MODEL_H
#define THEMA_MODEL_H

#include <QAbstractListModel>
#include <QVector>

class Thema_C;

class ThemaModel_C : public QAbstractListModel
{
    Q_OBJECT

    enum ROLES {
        DISPLAY_NAME=Qt::UserRole+1,
        TR_NAME,
        ICON,
        SELECTED,
        PROGRESS
    };

public:
    explicit ThemaModel_C(QVector<Thema_C*> thema_list, QObject *parent = 0);

// QAbstarctItemModel Implementation.
public:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    QHash<int,QByteArray> roleNames() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public:
    void AddThema(Thema_C* new_thema);

private:
    QVector<Thema_C*>& _thema_list;

};

#endif // THEMA_MODEL_H
