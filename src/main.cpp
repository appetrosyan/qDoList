#include <QGuiApplication>
#include <QQuickWindow>

#include "qmlsignalhandler.hpp"
#include "task.hpp"
#include "models/listmodel.hpp"
#include "qquicksettinginterface.hpp"
#include "trackedfile.hpp"

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QQuickWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QQuickStyle>
#include <QLoggingCategory>
#include <iostream>



int main(int argc, char ** argv)
{
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);
	QIcon qdo(":qDo.svg");
	QApplication::setWindowIcon(qdo);
	QApplication::setOrganizationName("ac.uk.cam.ap886");
	QApplication::setOrganizationDomain("cam.ac.uk");
	QApplication::setApplicationName("qDolist");

	QLoggingCategory::setFilterRules("*.debug=true\nqt.*.debug=false");
	QIcon::setThemeName("breeze");

	if (QFontDatabase::addApplicationFont(":/fontello.ttf") == -1)
		qWarning("Failed to load fontello.ttf");

	// Starting the system tray.
	QSystemTrayIcon tray(&app);
	tray.setIcon(QPixmap(":/qDo.svg"));
	tray.show();
	tray.setToolTip(QObject::tr("qDolist: A to-do list manager for grown-ups"));

	// Actions
	QMenu contextMenu;

	QAction* showAction = new QAction(QObject::tr("&Show"));
	QAction* hideAction = new QAction(QObject::tr("&Minimize to tray"));
	QAction* quitAction = new QAction(QObject::tr("&Quit"));
	QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

	contextMenu.addAction(hideAction);
	contextMenu.addAction(showAction);
	contextMenu.addSeparator();
	contextMenu.addAction(quitAction);

	tray.setContextMenu(&contextMenu);

	qmlRegisterType<Task>("ac.uk.cam.ap886", 1, 0, "Task");
	qmlRegisterType<List>("core", 1, 0, "List");
	qmlRegisterType<QQuickSettingInterface>("ac.uk.cam.ap886", 1,0, "SettingsInterface");
	qmlRegisterType<TrackedFile>("ac.uk.cam.ap886", 1, 0, "TrackedFile");
#ifdef Q_OS_MAC
	QQuickStyle::setStyle("Material");
#endif
	QMLSignalHandler handler(&app);
	handler.setSysTrayIcon(&tray);
	QObject::connect(hideAction, &QAction::triggered, handler.window, &QQuickWindow::hide);
	QObject::connect(showAction, &QAction::triggered, handler.window, &QQuickWindow::show);

	return QApplication::exec();
}




