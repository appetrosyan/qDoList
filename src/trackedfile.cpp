#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>


#include "trackedfile.hpp"
#include "models/tasklistmodel.hpp"
#include "task.hpp"

TrackedFile::TrackedFile(const QString& fileName, QObject *parent) noexcept: QObject(parent), m_url(fileName), m_file(new QFile(m_url.path()))
{
	m_taskList = new TaskListModel(this);
	connect(m_taskList, &TaskListModel::changesMade, this, [=]() {setModified(m_taskList->changesToSync()); });
}


bool TrackedFile::open(const QIODevice::OpenMode& flags)
{
	return m_file->open(flags);
}


void TrackedFile::write(const QJsonDocument& json)
{
	m_file->write(m_isBinary?json.toBinaryData():json.toJson());
}


void TrackedFile::close()
{
	m_file->flush();
	m_file->close();
}


QString TrackedFile::fileName(){return m_url.fileName();}


void TrackedFile::setModified(bool newModified)
{
	if(m_modified!=newModified){
		m_modified=newModified;
		emit modifiedChanged();
	}
}

bool TrackedFile::isBinary(){
	return m_isBinary;
}

void TrackedFile::setBinary(bool newBinary){
	if(m_isBinary!=newBinary){
		m_isBinary=newBinary;
		emit binaryChanged();
		setModified(true);
	}
}

QString TrackedFile::fullPath(){return m_url.path();}


bool TrackedFile::modified(){return m_modified;}


bool TrackedFile::openIfExists(const QIODevice::OpenMode& flags)
{
	if(m_file->exists()){
		if(m_file->open(flags)){
			return true;
		}else {
			qWarning("File couldn't be opened");
			qDebug() << m_file->error();
			qDebug() << m_url.path();
			return false;
		}
	}else {
		qWarning("File not found");
		return false;
	}
}


QByteArray TrackedFile::readAll(){	return m_file->readAll();}


TrackedFile& TrackedFile::setTaskList(TaskListModel* newTaskList)
{
	m_taskList = newTaskList;
	return *this;
}


TaskListModel& TrackedFile::taskList()	{return *m_taskList;}


void TrackedFile::requestAttention()	{emit wantAttention(this);}


void TrackedFile::saveToFile(TaskListModel* newTaskList)
{
	if(!newTaskList) newTaskList = m_taskList;
	if(!open(QIODevice::WriteOnly)){
		qDebug() << m_url.path();
		qWarning("Couldn't open save file.");
	}else {
		QJsonArray arr;
		for(int i=0; i<newTaskList->rowCount();++i){
			arr.append((qobject_cast<Task*>(newTaskList->get(i)))->toJson());
		}
		write(QJsonDocument(arr));
		close();
		m_modified=false;
		emit modifiedChanged();
	}
}


void TrackedFile::loadFromFile()
{
	if(openIfExists(QIODevice::ReadOnly)){
		QByteArray saveData = readAll();
		QJsonDocument loadDoc;
		if(m_isBinary)
			loadDoc= QJsonDocument::fromBinaryData(saveData);
		if(loadDoc.isNull()){
			loadDoc= QJsonDocument::fromJson(saveData);
			setBinary(false);
		}
		close();
		if(loadDoc.isEmpty()){
			qDebug() << "Ignoring empty document";
			return;
		}
		m_taskList->clear();
		for (auto o: loadDoc.array()){
			auto x = new Task();
			m_taskList->append(&x->updateFromJson(o.toObject()));
		}
		m_modified=false;
		emit modifiedChanged();
	}
}
