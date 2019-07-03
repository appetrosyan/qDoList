#pragma once

#include <QObject>
#include <QDateTime>

#include "models/tasklistmodel.hpp"

class List;


#define UNFUCK(X) qobject_cast<Task*>(X)
const auto fmt = Qt::DateFormat::RFC2822Date;
QString toStdTime(const QDateTime& date);


class Task : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString name READ text WRITE setText NOTIFY textChanged)
	Q_PROPERTY(QDateTime added READ added NOTIFY addedChanged)
	Q_PROPERTY(QDateTime scheduled READ scheduled WRITE setScheduled NOTIFY scheduledChanged)
	Q_PROPERTY(QDateTime due READ due WRITE setDue NOTIFY dueChanged)
	Q_PROPERTY(bool done READ done WRITE setDone NOTIFY doneChanged)
	Q_PROPERTY(bool overDue READ isOverDue NOTIFY overDueChanged)
	Q_PROPERTY(bool hasChildren READ hasChildren NOTIFY childrenChanged)
	Q_PROPERTY(TaskListModel* subModel READ subModel NOTIFY childrenChanged)
	Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
	Q_PROPERTY(int subtaskCount READ subtaskCount NOTIFY childrenChanged)
	Q_PROPERTY(int doneSubtaskCount READ doneSubtaskCount NOTIFY childToggled)
	Q_PROPERTY(QString prettyDueDate READ prettyDueDate NOTIFY dueChanged)
public:
	explicit Task(QString text="",
				  bool done = false,
				  QDateTime added = QDateTime::currentDateTime(),
				  QDateTime scheduled=QDateTime::currentDateTime(),
				  QDateTime due=QDateTime::currentDateTime(),
				  QObject *parent = nullptr);
//	explicit Task(const Task&& nu);
	Task(const Task& )=delete ;
	Task& operator=(const Task&) = delete;
	~Task() override;

	static QVector<Task*> globalRegister;

	bool isEverySubtaskDone() const;
	Task& setSuperModel(List* superModel);
	bool done() const;
	Task& setDone(bool done);
	bool hasChildren() const;

	QDateTime added() const;
	QDateTime scheduled() const;
	QDateTime due() const;
	Task& setScheduled(const QDateTime &scheduled);
	Task& setDue(const QDateTime &due);
	Task& extendDeadline(const std::function<QDateTime (QDateTime)>& fn);
	QString text() const {return m_text;}
	Task& setText(const QString &text);
	Task& fromNaturalLanguage(const QString& task);
	Task& addSubTask(Task* newTask);
	Task* subtask(int row) const;
	int subtaskCount() const;
	int doneSubtaskCount() const;
	TaskListModel* subModel();
	bool isScheduledToBegin() const;
	QJsonObject toJson() const;
	Task& updateFromJson(QJsonObject json);
	QString comment() const;
	QString prettyDueDate() const;
	Task& setComment(const QString& msg);
	Task& setSuperModel(TaskListModel* superModel);
	void forwardSignal();


signals:
	void textChanged();
	void addedChanged();
	void doneChanged();
	void overDueChanged();
	void scheduledChanged();
	void dueChanged();
	void childrenChanged();
	void commentChanged();
	void childToggled();

public slots:
	Q_INVOKABLE bool  isOverDue() const;
	Q_INVOKABLE Task& toggle();
	Q_INVOKABLE void goAway();
	Q_INVOKABLE void demote();
	Q_INVOKABLE void promote();
	Q_INVOKABLE void moveUp(int dx=1);
	Q_INVOKABLE void moveDown(int dx=1);
private:
	QDateTime m_added;
	QDateTime m_scheduled;
	QDateTime m_due;
	QString m_text;
	QString m_comment;
	TaskListModel* m_submodel=new TaskListModel(this);
	Task* m_superTask=nullptr;
	TaskListModel* m_superModel=nullptr;
	explicit Task(const QJsonObject& json, QObject *parent = nullptr);

	bool m_done{false};
};


//void tasksFromArray(QJsonArray arr, TaskListModel* tasks, QObject * parent=nullptr);


