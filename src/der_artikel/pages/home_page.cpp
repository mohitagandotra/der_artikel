#include "home_page.h"


HomePage_C::HomePage_C(Manager_C &page_manager, QQmlContext &root_context, QObject *parent) :
    Page_C(Manager_C::HOME_PAGE, page_manager, parent),
    _root_context(root_context)
{

}


