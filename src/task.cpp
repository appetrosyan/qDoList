#include "task.hpp"

#include <QDebug>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


QVector<Task*> Task::globalRegister;
Task* Task::lastFocusedTask=nullptr;

Task::Task(QString text, bool done, QDateTime added, QDateTime scheduled, QDateTime due, QObject* parent) :
	QObject(parent),
	m_added(std::move(added)),
	m_scheduled(std::move(scheduled)),
	m_due(std::move(due)),
	m_text(std::move(text)),
	m_done(done)
{
	if(qobject_cast<Task*> (parent)){
		m_superTask = dynamic_cast<Task*> (parent);
		setSuperModel(m_superTask->subModel());
	}
	globalRegister.append(this);
}

Task::Task(const QJsonObject& json, QObject *parent) : m_done(false)
{
	this->setParent(parent);
	this->updateFromJson(json);
}

Task::~Task()
{
	globalRegister.remove(globalRegister.indexOf(this));
	if(lastFocusedTask == this){
		lastFocusedTask=nullptr;
	}
}

bool Task::isEverySubtaskDone() const
{
	bool everySubtaskIsDone = true;
	if(m_submodel->_data.isEmpty())
		return everySubtaskIsDone;
	for(auto& t: m_submodel->_data){
		auto c = dynamic_cast<Task*>(t);
		everySubtaskIsDone =
				everySubtaskIsDone &&
				c->m_done &&
				c->isEverySubtaskDone() ;
	}
	return everySubtaskIsDone;
}

Task& Task::setSuperModel(TaskListModel* superModel)
{
	if(superModel == m_superModel) {
		;
	}else {
		m_superModel = superModel;
	}
	return *this;
}


bool Task::toggle() {
	if(this->isEverySubtaskDone()){
		if(m_done){
			m_done = false;
			for(auto& t: m_submodel->_data){
				auto c = dynamic_cast<Task*>(t);
				c->toggle();
			}
		} else {
			m_done = true;
		}
		emit doneChanged();
		emit overDueChanged();
		return true;
	}
	return false;
}

void Task::goAway()
/*
 * if You're having issues with this function, you're probably doing raw insert or add on the models. Don't forget to update the supermodels.
 * All puns intended.
 * */
{
	if(m_superTask){
		m_superTask->m_submodel->removeTask(this);
		disconnect(this, &Task::doneChanged, m_superTask, &Task::childToggled);
		emit m_superTask->childrenChanged();
		emit childrenChanged();
	}else {
		delete m_superModel->removeTask(this);
		emit m_superModel->changesMade();
	}
}

void Task::demote()
{
	auto index = m_superModel->indexOf(this);
	if(index!=0){
		m_superModel->take(index);
		disconnect(this, &Task::doneChanged, m_superTask, &Task::childToggled);
		auto newParent = qobject_cast<Task*>((*m_superModel)[index-1]);
		newParent->addSubTask(this);
	}
}

void Task::promote()
{
	if(m_superTask){
		m_superModel->removeTask(this);
		disconnect(this, &Task::doneChanged, m_superTask, &Task::childToggled);
		m_superTask->m_superModel->insert(this, m_superTask->m_superModel->indexOf(m_superTask));
		emit m_superTask->childrenChanged();
		setSuperModel(m_superTask->m_superModel);
		m_superTask=m_superTask->m_superTask;
		if(m_superTask){
			connect(this, &Task::doneChanged, m_superTask, &Task::childToggled);
		}
	}
}

void Task::moveUp(int dx)
{
	int x = m_superModel->indexOf(this);
	m_superModel->move(x, x-dx);
}

void Task::moveDown(int dx)
{
	int x = m_superModel->indexOf(this);
	m_superModel->move(x, x+dx);
}

Task& Task::requestFocus(){
	if(lastFocusedTask!=this){
		Task* old=lastFocusedTask;
		lastFocusedTask=this;
		if(old){
			emit old->lastFocusedChanged();
		}
		emit lastFocusedChanged();
	}
	return *this;
}

bool Task::done() const { return m_done;}

Task &Task::setDone(bool done)
{
	if(done!=m_done){
		m_done = done;
		emit doneChanged();
	}
	return *this;
}

bool Task::hasChildren() const { return !m_submodel->_data.isEmpty(); }

QDateTime Task::added() const { return m_added;}

QDateTime Task::scheduled() const { return m_scheduled;}

Task &Task::setScheduled(const QDateTime &scheduled)
{
	if(m_scheduled != scheduled){
		m_scheduled = scheduled;
		if(m_scheduled>m_due){
			m_due = m_scheduled;
			emit dueChanged();
			emit overDueChanged();
		}
		emit scheduledChanged();
		emit overDueChanged();
	}
	return *this;
}

QDateTime Task::due() const{ return m_due; }


Task &Task::setDue(const QDateTime &due)
{

	if(m_due != due){
		m_due = due;
		if(m_due<m_scheduled){
			m_scheduled = m_due;
			emit scheduledChanged();
			emit overDueChanged();
		}
		emit dueChanged();
		emit overDueChanged();
	}
	return *this;
}

Task &Task::extendDeadline(const std::function<QDateTime (QDateTime)> &fn)
{
	m_due = fn(m_due);
	return *this;
}

Task &Task::setText(const QString &text)
{
	if(m_text!= text){
		m_text = text;
		emit textChanged();
	}
	return *this;
}



Task &Task::addSubTask(Task *newTask)
{
	newTask->setParent(this);
	newTask->m_superTask = this;
	m_submodel->append(newTask);
	connect(newTask, &Task::doneChanged, this, &Task::childToggled);
	emit childrenChanged();
	return *this;
}

Task *Task::subtask(int row) const
{
	return qobject_cast<Task*>((*m_submodel)[row]);
}

int Task::subtaskCount() const { return m_submodel->rowCount();}

int Task::doneSubtaskCount() const
{
	int doneTasks=0;
	for(int i=0; i<m_submodel->rowCount(); ++i){
		doneTasks+=m_submodel->get(i)->property("done").toBool();
	}
	return doneTasks;
}


TaskListModel *Task::subModel() { return m_submodel;}

bool Task::isScheduledToBegin() const
{
	return !(scheduled() < QDateTime::currentDateTime());
}

bool Q_INVOKABLE Task::isOverDue() const
{
	return (!m_done) && !(due() > QDateTime::currentDateTime());
}

QJsonObject Task::toJson() const
{
	QJsonObject retVal;
	retVal["text"] = m_text;
	retVal["done"] = m_done;
	retVal["comment"] = m_comment;
	auto f = [] (QDateTime t) {return t.toString(fmt);};
	retVal["added"] = f(m_added);
	retVal["scheduled"] = f(m_scheduled);
	retVal["due"] = f(m_due);
	QJsonArray arr;
	for(int i=0; i<m_submodel->rowCount();++i){
		auto t = qobject_cast<Task*>(m_submodel->get(i));
		arr.append(t->toJson());
	}
	retVal["subtasks"] = arr;
	return retVal;
}

Task &Task::updateFromJson(QJsonObject json)
{
	m_text = json.contains("text") && json["text"].isString() ? json["text"].toString():"";
	m_comment = json.contains("comment") && json["comment"].isString() ? json["comment"].toString():"";
	m_done = json.contains("done") && json["done"].isBool() && json["done"].toBool();
	auto f = [json] (std::string v) {
		QString verb = QString::fromStdString(v);
		return json.contains(verb) && json[verb].isString()?
					QDateTime::fromString(json[verb].toString(), fmt):
					QDateTime::currentDateTime();
	};
	m_added = f("added");
	m_scheduled = f("scheduled");
	m_due = f("due");
	if(json.contains("subtasks") && json["subtasks"].isArray()){
		m_submodel->clear();
		for(auto a: json["subtasks"].toArray()){
			auto t = new Task();
			t->setParent(this);
			t->updateFromJson(a.toObject());
			this->addSubTask(t);
		}
	}
	return *this;
}

QString Task::comment() const {	return m_comment;}

// You might reasonably ask, why do I have a duplicate property for essentially the exact
// same thing as a due date. Why can't I just process the raw data in QML, and do the prettification there?
// Two reasons:
//  * JavaScript would do the same thing in a more brittle and slower fashion.
// It has one Job and it does it poorly. It's so "efficient" that a small website
// with mainly text takes up your entire RAM. For reference I can run a real time
// simulation of the Universe and not run out of it.
//  * QQC2 Sections is poorly designed. I would expect that having JavaScript and
// being able to pass functions around, you could have at least JitTed a comparator.
// instead you pass the variable ... name ... as a string. If at least one of those
// wasn't true, there wouldn't have been an issue. I'm not even passing it executable code,
// just the property name. And funnily enough, I don't want for my tasks to be separate if
// they are only milliseconds apart.

// Why couldn't have QML been an extension of Phython? With TOML syntax? Or better yet, JSON-only, no JavaScript, no implicit conversions?
// that way the interface would have been truly purely declarative, and you wouldn't have a Turing complete language be a glorified paperweight.

bool isWithinWeek(const QDateTime& a){
	return a < QDateTime::currentDateTime().addDays(7) && a> QDateTime::currentDateTime().addDays(-7);
}

short weekcmp(const QDateTime& a, const QDateTime& b=QDateTime::currentDateTime()){
	if(a.date().weekNumber()>b.date().weekNumber()){
		return 1;
	} else if(a.date().weekNumber()< b.date().weekNumber()){
		return -1;
	}
	return 0;
}

QString Task::prettyDueDate() const
{
	if(!m_due.isValid()){
		return "unscheduled";
	}
	if(isWithinWeek(m_due)){
		if(weekcmp(m_due)==0){
			return m_due.date().day() == QDate::currentDate().day()?tr("today"):tr("this %1").arg(m_due.toString("dddd"));
		}else  if(weekcmp(m_due)>0){
			return tr("next %1").arg(m_due.toString("dddd"));
		} else {
			return tr("last %1").arg(m_due.toString("dddd"));
		}
	} else {
		return m_due.toString(Qt::LocalDate);
	}
}

Task &Task::setComment(const QString& msg)
{
	if(m_comment !=msg){
		m_comment = msg;
		emit commentChanged();
	}
	return *this;
}

