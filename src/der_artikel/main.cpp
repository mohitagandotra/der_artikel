#include <QApplication>
#include <QQmlContext>
#include <QtQml>

#include "qtquick2applicationviewer.h"
#include "manager.h"
#include "data/result.h"

#ifdef ENABLE_THEMA_BUILDER
#include "thema_builder.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterUncreatableType<Manager_C>("com.vystosi.qmlcomponents", 1, 0, "Manager","reason");
    qmlRegisterUncreatableType<Result_C>("com.vystosi.qmlcomponents", 1, 0, "Result","reason");


#ifdef ENABLE_THEMA_BUILDER
    ThemaBuilder_C thema_builder;
    thema_builder.show();
#endif

    Manager_C manager;

    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("manager", &manager);
    viewer.rootContext()->setContextProperty("currentResult", manager.GetCurrentResult());
    viewer.setMainQmlFile(QStringLiteral("qml/der_artikel/main.qml"));

    QQuickItem* root_item = viewer.rootObject();
    manager.SetRootItem(root_item);

    viewer.showExpanded();

    return app.exec();
}
