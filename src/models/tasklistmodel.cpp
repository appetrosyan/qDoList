#include <QDebug>
#include <QRegularExpression>

#include "../task.hpp"
#include "tasklistmodel.hpp"
#include "../naturallanguageparser.h"
#include "../tasklang.h"

auto static constexpr changedSignals = {&Task::dueChanged, &Task::addedChanged, &Task::scheduledChanged, &Task::commentChanged, &Task::textChanged, &Task::childrenChanged, &Task::doneChanged};


TaskListModel::TaskListModel(QObject* parent) : List(parent){}

bool TaskListModel::changesToSync(){
	return m_internalChange;
}

void TaskListModel::makeChanges(){
	m_internalChange = true;
	emit changesMade();
}


QObject * TaskListModel::removeTask(Task *t)
{
	auto idx= _data.indexOf(qobject_cast<QObject*>(t));
	if(idx==-1){
		qWarning() << "Task not found" << t-> text();
		for(auto& d: _data){
			qDebug() << d->property("name");
		}
	}
	return take(idx);
}


void TaskListModel::connectSubtaskChanges(Task *o)
{
	makeChanges();
	for(auto signal: changedSignals){
		connect(o, signal, this, &TaskListModel::makeChanges);
	}
}

void TaskListModel::unmakeChanges(){
	m_internalChange = false;
}


void TaskListModel::append(Task *o)
{
	o->setSuperModel(this);
	connectSubtaskChanges(o);
	add(qobject_cast<QObject*>(o));
}


void TaskListModel::createNewTask(QString taskName)
{
	if(taskName.isNull() || taskName.isEmpty()){return;	}
	auto c = TaskLang::command.match(taskName);
	if(c.hasMatch()){
		taskName=c.captured(2).simplified();
		Task* newTask = new Task(taskName);
		newTask = TaskLang::captureDueDate(taskName, newTask);
		newTask = TaskLang::captureScheduled(taskName, newTask);
		newTask = TaskLang::captureSubtasks(taskName, newTask);
		newTask = TaskLang::captureComment(taskName, newTask);
		if(c.captured(1).isEmpty()){
			append(newTask);
		}
		else {
			if(c.captured(1).contains("-")){
//				qDebug() << "TODO implement subtaskgin";
				UNFUCK(_data.back())->addSubTask(new Task(c.captured(2)));
				emit changesMade();
			}
			if(c.captured(1).contains("+")){
				QStringList names;
				for(auto x: _data){
					auto y = UNFUCK(x);
					names<<x->property("name").toString();
					if(y->text().startsWith(c.captured(2))){
						y->toggle();
					}
				}
			}
		}
	}
}
