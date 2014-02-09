#include "thema_model.h"
#include <QPixmap>

/*!
 \brief

 \param parent
*/
ThemaModel_C::ThemaModel_C(QObject *parent):
    QAbstractListModel(parent)
{

}

/*!
 \brief

*/
ThemaModel_C::~ThemaModel_C()
{
    foreach(Thema_C* thema, _thema_list) {
        delete thema;
    }
}

/*!
 \brief

 \param index
 \param role
 \return QVariant
*/
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

            case AUTHOR:
                data = thema->Author();
                break;

            case SELECTED:
                data = thema->Selected();
                break;

            case EXPERIENCE:
                data = thema->ExperiencePoints();
                break;

            case WORD_COUNT:
                data = thema->GetWordCount();
                break;

            case THEMA_STATE:
                data = thema->state();
                break;

            case THEMA_OBJECT:
                data = QVariant::fromValue<QObject*>(thema);
                break;
            default:
                break;
            }

        }
    }


    return data;
}

/*!
 \brief

 \param parent
 \return int
*/
int ThemaModel_C::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _thema_list.count();
}

/*!
 \brief

 \return QHash<int, QByteArray>
*/
QHash<int, QByteArray> ThemaModel_C::roleNames() const
{
    QHash<int,QByteArray> roleNames;
    roleNames[DISPLAY_NAME] = "display_name";
    roleNames[TR_NAME] = "tr_name";
    roleNames[AUTHOR] = "author";
    roleNames[SELECTED] = "selected";
    roleNames[EXPERIENCE] = "experience";
    roleNames[WORD_COUNT] = "word_count";
    roleNames[THEMA_STATE] = "thema_state";
    roleNames[THEMA_OBJECT] = "thema_object";
    return roleNames;
}

/*!
 \brief

 \param index
 \return Qt::ItemFlags
*/
Qt::ItemFlags ThemaModel_C::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

/*!
 \brief

 \param index
 \param value
 \param role
 \return bool
*/
bool ThemaModel_C::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = false;
    if(index.isValid()) {
        Thema_C* thema = _thema_list.at(index.row());

        if(thema) {
            QVector<int> changedRoles;
            bool selected = value.toBool();
            switch(role) {

            case SELECTED:
                thema->setSelected(selected);
                if(selected) {
                    _selected_thema_list.append(thema);
                } else {
                    _selected_thema_list.removeAll(thema);
                }
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

/*!
 \brief

 \param new_thema
*/
void ThemaModel_C::AddThema(Thema_C *new_thema)
{
    Q_ASSERT(new_thema);
    beginInsertRows(QModelIndex(),_thema_list.count(),_thema_list.count());
    _thema_list.append(new_thema);
    connect(new_thema, SIGNAL(selectionChanged(Thema_C::SelectionType_TP)),this,SLOT(OnThemaItemSelectionChanged(Thema_C::SelectionType_TP)));
    endInsertRows();
}

/*!
 \brief

 \return Thema_C
*/
Thema_C* ThemaModel_C::GetSelectedThema()
{
    Thema_C* thema = 0;
    if(_selected_thema_list.count() > 0 ) {
        thema = _selected_thema_list.at(0);
    }
    return thema;
}

/*!
 \brief

*/
void ThemaModel_C::ClearSelection()
{
    foreach (Thema_C* thema, _selected_thema_list) {
       thema->setSelected(false);
    }
    _selected_thema_list.clear();
}

/*!
 \brief

 \return ThemaModel_C::SelectionState_TP
*/
ThemaModel_C::SelectionState_TP ThemaModel_C::SelectionState() const
{
    SelectionState_TP selection_state;

    switch (_selected_thema_list.count()) {
        case 0:
           selection_state = NO_SELECTION;
            break;
        case 1:
           selection_state = SINGLE_SELECTION;
            break;
        default:
            selection_state = MULTIPLE_SELECTION;
            break;
    }

    return selection_state;
}

/*!
 \brief

 \param type
*/
void ThemaModel_C::OnThemaItemSelectionChanged(Thema_C::SelectionType_TP type)
{
    Thema_C* thema = qobject_cast<Thema_C*>(sender());
    Q_ASSERT(thema);
    if(type == Thema_C::SINGLE_SELECTION) {
        ClearSelection();
    }
    if(thema->Selected()) {
        _selected_thema_list.append(thema);
    } else {
        _selected_thema_list.removeAll(thema);
    }
    emit themaSelectionChanged();
    QModelIndex thema_index = index(_thema_list.indexOf(thema));
    QVector<int> roles;
    roles<<SELECTED;
    emit dataChanged(thema_index,thema_index,roles);
}
