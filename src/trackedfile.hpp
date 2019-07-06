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
	bool m_isBinary=true;
	Q_PROPERTY(QString fileName READ fileName NOTIFY locationChanged)
	Q_PROPERTY(QString fullPath READ fullPath NOTIFY locationChanged)
	Q_PROPERTY(bool isModified READ modified NOTIFY modifiedChanged)
	Q_PROPERTY(bool isBinary READ isBinary WRITE setBinary NOTIFY binaryChanged)
public:
	explicit TrackedFile(const QString& fileName, QObject *parent = nullptr) noexcept;
	TrackedFile()=default;
	bool open(const QFile::OpenMode& flags);
	void write(const QJsonDocument& json);
	void close();
	QString fileName();
	QString fullPath();
	void setModified(bool newModified);
	bool isBinary();
	void setBinary(bool newBinary);
	bool modified();
	bool openIfExists(const QFile::OpenMode& flags);
	QByteArray readAll();
	TrackedFile& setTaskList(TaskListModel* newTaskList);
	TaskListModel& taskList();
signals:
	void locationChanged();
	void wantAttention(TrackedFile* ot);
	void modifiedChanged();
	void binaryChanged();
public slots:
	void requestAttention();
	void saveToFile(TaskListModel* taskList=nullptr);
	void loadFromFile();
};

