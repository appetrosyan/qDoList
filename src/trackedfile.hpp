#pragma once

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QUrl>

class TaskListModel;
class QJsonDocument;

class TrackedFile : public QObject
{
	Q_OBJECT
private:
	QUrl m_url;
	QFile* m_file{};
	TaskListModel* m_taskList{};
	bool m_modified=false;
	Q_PROPERTY(QString fileName READ fileName NOTIFY locationChanged)
	Q_PROPERTY(QString fullPath READ fullPath NOTIFY locationChanged)
	Q_PROPERTY(bool isModified READ modified NOTIFY modifiedChanged)
public:
	explicit TrackedFile(const QString& fileName, QObject *parent = nullptr) noexcept;
	TrackedFile()=default;
//	TrackedFile(TrackedFile&& other) noexcept;
//	TrackedFile& operator=(TrackedFile&& other) noexcept;
	bool open(const QFile::OpenMode& flags);
	void write(const QJsonDocument& json);
	void close();
	QString fileName();
	QString fullPath();
	void setModified(bool newModified);
	bool modified();
	bool openIfExists(const QFile::OpenMode& flags);
	QByteArray readAll();
	TrackedFile& setTaskList(TaskListModel* newTaskList);
	TaskListModel& taskList();
signals:
	void locationChanged();
	void wantAttention(TrackedFile* ot);
	void modifiedChanged();
public slots:
	void requestAttention();
	void saveToFile(TaskListModel* taskList=nullptr);
	void loadFromFile();
};

