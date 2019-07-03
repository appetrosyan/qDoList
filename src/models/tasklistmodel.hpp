#pragma once
#include <QtCore/QObject>

#include "listmodel.hpp"
class Task;

class TaskListModel : public List
{
	Q_OBJECT
	Q_PROPERTY(int completeTasks READ incompleteTasks NOTIFY changesMade)
	Q_PROPERTY(int totalTasks READ totalTasks  NOTIFY changesMade)
public:
	TaskListModel(QObject* parent);
	bool changesToSync();
	void makeChanges();
	void connectSubtaskChanges(Task *o);
	int totalTasks(){
		return _data.count();
	}
	int incompleteTasks(){
		int ret=0;
		for (auto x: _data){
			if(x->property("done").toBool()){
				++ret;
			}
		}
		return ret;
	}
	void unmakeChanges();
signals:
	void changesMade();
public slots:
	void append(Task* o);
	void createNewTask(QString taskName);
	QObject* removeTask(Task* t);
private:
	bool m_internalChange=false;
};
