#include "thema_model.h"
#include "data/thema.h"
#include <QPixmap>

ThemaModel_C::ThemaModel_C(QObject *parent):
    QAbstractListModel(parent)
{

}

ThemaModel_C::~ThemaModel_C()
{
    foreach(Thema_C* thema, _thema_list) {
        delete thema;
    }
}

QVariant ThemaModel_C::data(const QModelIndex &index, int role) const
{
    QVariant data;

    if(index.isValid() && index.row() < _thema_list.count()) {
        Thema_C* thema = _thema_list.at(index.row());
        if(thema) {

            switch(role) {

            case DISPLAY_NAME:
                data = thema->GetText();
                break;

            case TR_NAME:
                data = thema->GetTrText();
                break;

            case ICON:
                data = thema->GetIconUrl();
                break;

            case SELECTED:
                data = thema->Selected();
                break;

            case PROGRESS:
                data = 0.5;
                break;

            case WORD_COUNT:
                data = thema->GetWordCount();
                break;
            default:
                break;
            }

        }
    }


    return data;
}

int ThemaModel_C::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _thema_list.count();
}

QHash<int, QByteArray> ThemaModel_C::roleNames() const
{
    QHash<int,QByteArray> roleNames;
    roleNames[DISPLAY_NAME] = "display_name";
    roleNames[TR_NAME] = "tr_name";
    roleNames[ICON] = "icon";
    roleNames[SELECTED] = "selected";
    roleNames[PROGRESS] = "progress";
    roleNames[WORD_COUNT] = "word_count";
    return roleNames;
}

Qt::ItemFlags ThemaModel_C::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool ThemaModel_C::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = false;
    if(index.isValid()) {
        Thema_C* thema = _thema_list.at(index.row());

        if(thema) {
            QVector<int> changedRoles;
            switch(role) {

            case SELECTED:
                thema->SetSelected(value.toBool());
                success = true;
                changedRoles<<SELECTED;
                emit dataChanged(index,index,changedRoles);
                break;

            default:
                break;
            }
        }
    }

    return success;
}

void ThemaModel_C::AddThema(Thema_C *new_thema)
{
    Q_ASSERT(new_thema);
    beginInsertRows(QModelIndex(),_thema_list.count(),_thema_list.count());
    _thema_list.append(new_thema);
    endInsertRows();
}
