#pragma once
#include <QApplication>
#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>

//#include "headers/trackedfile.hpp"



class QSettings;
class QJsonDocument;
class QQmlContext;
class QQuickWindow;
class QGuiApplication;
class QQmlApplicationEngine;
class TrackedFile;
class TaskListModel;
class FileListModel;
class Task;
class QSystemTrayIcon;
class QMenu;
class QQuickSettingInterface;

class QMLSignalHandler : public QObject{
	Q_OBJECT
	void lastFocusedGuard();

public:
	explicit QMLSignalHandler(QGuiApplication* app, QObject* parent=nullptr);
	QSettings* m_settings;
	QQuickWindow * window;
	QMap<QString, Task*> m_globalMap;
	QQmlApplicationEngine engine;
	TaskListModel* taskList;
	FileListModel* fileList;
	QQuickSettingInterface* m_settingsInterface;
public slots:
	void populateModel();
	void handleMessage(QString msg);
	void setActiveTrackedFile(QString fname);
	void removeAt(int x);
	void saveModelToFile(QString fname);
	void syncAllFiles();
	void loadModelFromFile(QString fname);
	void receiveTask(Task* t);
	void printAllTasksToConsole();
	void resetContext();
	void moveCurrentlyFocusedTaskUp();
	void moveCurrentlyFocusedTaskDown();
	void demoteCurrentlyFocusedTask();
	void promoteCurrentlyFocusedTask();
};
