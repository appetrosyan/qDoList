//#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>

#include "task.hpp"
#include "naturallanguageparser.h"
#include "tasklang.h"


Task*  TaskLang::captureDueDate(QString& taskName, Task* newTask)
{
	if(Q_UNLIKELY(!newTask)) {
		qWarning("Null Pointer passed in");
		return nullptr;
	}
	auto d = TaskLang::due.match(taskName);
	if(d.hasMatch()){
		newTask->setText(taskName.remove(TaskLang::due));
		newTask->setDue(TaskLang::smartDate(d.captured("date")));
	}
	return newTask;
}


QDateTime TaskLang::smartDate(const QString& date)
{
	qDebug("%s", QString(date).toStdString().c_str());
	QDate out;
	if(date.contains(QObject::tr("tomorrow"),Qt::CaseInsensitive) || date.contains("завтра")){
		qDebug("%s", (date + "recognised as tomorrow").toStdString().c_str());
		out =  QDate::currentDate().addDays(1);
	}
	if(date.contains(QObject::tr("next"), Qt::CaseInsensitive)){
		if(date.contains(QObject::tr("week"), Qt::CaseInsensitive)){
			out = QDate::currentDate().addDays(7);
		} else if (date.contains(QObject::tr("day"), Qt::CaseInsensitive)){
			out = QDate::currentDate().addDays(1);
		}
	}
	// Try to capture a digital date first
	QTime time;
	static QRegularExpression re(TaskLang::digiDate);
	auto match = re.match(date);
	if(!match.captured("ddate").isEmpty()){
		auto datePart = match.captured("ddate");
		auto sep = match.captured("sep");
		if(match.captured("extra").isEmpty()){
			for(const auto& fmt: {"d"+sep+"M", "M"+sep+"d"}){
				out= QDate::fromString(datePart, fmt);
				if(out.isValid()){
					out.setDate(QDate::currentDate().year(), out.month(), out.day());
					break;
				}
			}
		}else {
			for(const auto& fmt: {"d"+sep+"M"+sep+"yy", "d"+sep+"M"+sep+"yyyy", "M"+sep+"d"+sep+"yy", "M"+sep+"d"+sep+"yyyy"}){
				out = QDate::fromString(datePart, fmt);
				if(out.isValid()){
					break;
				}
			}
		}
	}
	if(!match.captured("hhmm").isEmpty()){
		for(auto fmt: {"h:mm", "h:mmAP", "h:mmap"}){
			time = QTime::fromString(match.captured("hhmm"), fmt);
			if(time.isValid()){
				break;
			}
		}
	} else if(!match.captured("phhmm").isEmpty()){
		for(auto fmt: {"h:mm", "h:mmAP", "h:mmap"}){
			time = QTime::fromString(match.captured("phhmm"), fmt);
			if(time.isValid()){
				break;
			}
		}
	} else if(!match.captured("phh").isEmpty()){
		auto ap = match.captured("pap").isEmpty()?"pm":match.captured("pap");
		time = QTime::fromString(match.captured("phh")+ap, "hap");
	} else if(!match.captured("hh").isEmpty()){
		auto ap = match.captured("ap").isEmpty()?"pm":match.captured("ap");
		time = QTime::fromString(match.captured("hh")+ap, "hap");
	}
	return out.isValid()?QDateTime(out,time.isValid()?time:QTime::currentTime()):time.isValid()?QDateTime(QDate::currentDate(), time):QDateTime();
}




Task* TaskLang::captureScheduled(QString& taskName, Task* newTask)
{
	if(Q_UNLIKELY(!newTask)) {
		qWarning("Null pointer passed into captureScheduled");
		return nullptr;
	}
	static auto d = TaskLang::scheduled.match(taskName);
	if(d.hasMatch()){
		newTask->setText(taskName.remove(TaskLang::scheduled));
		newTask->setScheduled(TaskLang::smartDate(d.captured("date")));
	}
	return newTask;
}


Task* TaskLang::captureSubtasks(QString& taskName, Task* task)
{
	if(Q_UNLIKELY(!task)) {
		qWarning(" nullptr in captureSubtasks");
		return nullptr;
	}
	auto s = TaskLang::subTasks.match(taskName);
	if(s.hasMatch()){
		task-> setText(taskName.remove(s.captured(1)).simplified());
		auto subs = s.captured(2).split(",");
		for(const auto& q: subs){
			if(q.simplified().isEmpty()){
				qDebug("caught empty string");
			}else {
				task->addSubTask(new Task(q.simplified(), false, QDateTime::currentDateTime(), task->scheduled(), task->due()));
			}
		}
	}
	return task;
}


Task* TaskLang::captureComment(QString& taskName, Task* task)
{
	if(Q_UNLIKELY(!task)) {
		qWarning("nullptr in captureComment");
		return nullptr;
	}
	auto c = TaskLang::comment.match(taskName);
	if(c.hasMatch()){
		task->setText(taskName.remove(TaskLang::comment).simplified());
		task->setComment(c.captured(1));
	}
	return task;
}
