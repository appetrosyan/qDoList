#include <QDateTime>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlContext>
#include <QJsonDocument>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QQuickTextDocument>
#include <QQmlFileSelector>

#include "task.hpp"
#include "qmlsignalhandler.hpp"
#include "models/tasklistmodel.hpp"
#include "models/filelistmodel.hpp"
#include "qquicksettinginterface.hpp"
#include "trackedfile.hpp"
#include "naturallanguagehighlighter.h"

#undef QT_NO_DEBUG_OUTPUT

template <class T>
T childObject(QQmlApplicationEngine& engine,
			  const QString& objectName,
			  const QString& propertyName)
{
	QList<QObject*> rootObjects = engine.rootObjects();
	foreach (QObject* object, rootObjects)
	{
		auto* child = object->findChild<QObject*>(objectName);
		if (child != nullptr)
		{
			std::string s = propertyName.toStdString();
			auto* object = child->property(s.c_str()).value<QObject*>();
			Q_ASSERT(object != nullptr);
			T prop = dynamic_cast<T>(object);
			Q_ASSERT(prop != nullptr);
			return prop;
		}
	}
	return static_cast<T>(nullptr);
}

void QMLSignalHandler::populateModel()
{
	QStringList fileNames;
	auto size = m_settings->beginReadArray("LastOpenedFiles");
	if(size==0){
		qDebug() << "No session files found. Creating a new empty session";
	}
	for(int i=0; i< size; ++i){
		m_settings->setArrayIndex(i);
		qDebug() << m_settings->value("fileName").toString();
		loadModelFromFile(m_settings->value("fileName").toString());
	}
	m_settings->endArray();
}

QMLSignalHandler::QMLSignalHandler(QGuiApplication* app,
								   QObject* parent) : QObject (parent)
{
	m_settingsInterface=new QQuickSettingInterface(this);
	m_settings=new QSettings();

	taskList = new TaskListModel{static_cast<QObject*>(this)};
	fileList = new FileListModel{static_cast<QObject*>(this)};
	qmlRegisterType<QSettings>("QSettings", 1,0, "QSettings");
	engine.rootContext()->setContextProperty("myFileList", fileList);
	engine.rootContext()->setContextProperty("myModel", taskList);
	engine.rootContext()->setContextProperty("settings", m_settingsInterface);
	selector = new QQmlFileSelector(&engine);
	qInfo() << "Hello world ";
#ifdef Q_OS_LINUX
	const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
#endif
#ifdef Q_OS_MAC
	const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
#endif
	auto f = [url](QObject *obj, const QUrl &objUrl)
	{
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	};
	connect(&engine, &QQmlApplicationEngine::objectCreated, app,f, Qt::QueuedConnection);
	engine.load(url);
	window = qobject_cast<QQuickWindow *>(engine.rootObjects().value(0));
	populateModel();
//	connect(window, SIGNAL(deleteAt(int)), this, SLOT(removeAt(int)));
	connect(window, SIGNAL(writeToFile(QString)), this, SLOT(saveModelToFile(QString)));
	connect(window, SIGNAL(loadFromFile(QString)), this, SLOT(loadModelFromFile(QString)));
	connect(window, SIGNAL(saveAllFiles()), this, SLOT(syncAllFiles()));
	connect(window, SIGNAL(requestTaskList()), this, SLOT(printAllTasksToConsole()));
	connect(window, SIGNAL(moveFocusedTaskUp()), this, SLOT(moveCurrentlyFocusedTaskUp()));
	connect(window, SIGNAL(moveFocusedTaskDown()), this, SLOT(moveCurrentlyFocusedTaskDown()));
	connect(window, SIGNAL(demoteFocusedTask()), this, SLOT(demoteCurrentlyFocusedTask()));
	connect(window, SIGNAL(promoteFocusedTask()), this, SLOT(promoteCurrentlyFocusedTask()));
	connect(window, SIGNAL(toggleFocusedTask()), this, SLOT(toggleFocusedTask()));
	auto* doc = childObject<QQuickTextDocument*>(engine, "textEditor", "textDocument");
	auto* parser = new NaturalLanguageHighlighter(doc->textDocument());
	Q_UNUSED(parser);

}


void QMLSignalHandler::resetContext()
{
	engine.rootContext()->setContextProperty("myModel", &fileList->activeTrackedFile()->taskList());
}

void QMLSignalHandler::lastFocusedGuard(){
	if(Task::lastFocusedTask==nullptr){
		if(!taskList->isEmpty())
			qobject_cast<Task*> (taskList->back())->requestFocus();
	}
}

#define LASTFOCUSED(Y) if(Task::lastFocusedTask!=nullptr){Task::lastFocusedTask->Y();}

void QMLSignalHandler::moveCurrentlyFocusedTaskUp()
{
	lastFocusedGuard();
	LASTFOCUSED(moveUp)
}

void QMLSignalHandler::moveCurrentlyFocusedTaskDown()
{
	lastFocusedGuard();
	LASTFOCUSED(moveDown)
}

void QMLSignalHandler::demoteCurrentlyFocusedTask()
{
	lastFocusedGuard();
	LASTFOCUSED(demote)
}

void QMLSignalHandler::promoteCurrentlyFocusedTask()
{
	lastFocusedGuard();
	LASTFOCUSED(promote)
}

void QMLSignalHandler::toggleFocusedTask()
{
	lastFocusedGuard();
	LASTFOCUSED(toggle)
}

#undef LASTFOCUSED

void QMLSignalHandler::handleMessage(QString msg)
{
	if(msg.isEmpty())
		return;
	fileList->activeTrackedFile()->setModified(true);
	fileList->activeTrackedFile()->taskList().append(new Task(msg));
}

void QMLSignalHandler::setActiveTrackedFile(QString fname)
{

	fileList->addCurrentlyActiveFile(fname);
	taskList=&fileList->activeTrackedFile()->taskList();
	resetContext();
	connect(fileList, &FileListModel::activeTrackedFileChanged, this, [=](){ resetContext();});
}

void QMLSignalHandler::removeAt(int x)
{
	qDebug()<<"remove "<<x;
}

void QMLSignalHandler::saveModelToFile(QString fname)
{
	if(fileList->isEmpty()){
		fileList->addCurrentlyActiveFile(fname);
		fileList->activeTrackedFile()->setTaskList(taskList);
		fileList->activeTrackedFile()->saveToFile();
		resetContext();
		connect(fileList, &FileListModel::activeTrackedFileChanged, this, [=](){ resetContext();});
	}else {
		setActiveTrackedFile(fname);
		fileList->activeTrackedFile()->saveToFile();
	}
}

void QMLSignalHandler::syncAllFiles()
{
	if(!fileList->isEmpty()){
		for(auto x: *fileList){
			auto y = qobject_cast<TrackedFile*>(x);
			if(y->modified()){
				y->saveToFile();
			}
		}
		m_settingsInterface->writeRecentFilesToDisk(fileList->fileNames());
	}
}



void QMLSignalHandler::loadModelFromFile(QString fname)
{
	setActiveTrackedFile(fname);
	fileList->activeTrackedFile()->loadFromFile();
}

void QMLSignalHandler::receiveTask(Task* t)
{
	Q_UNUSED(t)
	qDebug() << "Success! Received";
}

void QMLSignalHandler::printAllTasksToConsole()
{
	qDebug() << Task::globalRegister.count();
}
