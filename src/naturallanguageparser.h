#pragma once
#include <QtCore/QDateTime>
#include <QRegularExpression>
class Task;


namespace TaskLang{
	QDateTime smartDate(const QString& date);
	Task* captureDueDate(QString& taskName, Task* newTask);
	Task* captureScheduled(QString& taskName, Task* newTask);
	Task* captureSubtasks(QString& taskName, Task* task);
	Task* captureComment(QString& taskName, Task* task);
}
