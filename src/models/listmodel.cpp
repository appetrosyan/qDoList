//#include <QDebug>

#include "listmodel.hpp"

void List::insert(QObject *o, int i)
{
	beginInsertRows(QModelIndex(), i, i);
	_data.insert(i, o);
	o->setParent(this);
	sizeChanged();
	endInsertRows();
}

void List::clear()
{
	if(_data.isEmpty()) return;
	beginRemoveRows(QModelIndex(), 0, _data.size());
	_data.clear();
	emit sizeChanged();
	endRemoveRows();
}


QObject *List::take(int i)
/*
 * Removes the element from the model, returning a pointer to the QObject removed.
 * */
{
	if ((i > -1) && (i < _data.size())) {
		beginRemoveRows(QModelIndex(), i, i);
		QObject * o =_data.takeAt(i);
		o->setParent(nullptr);
		emit sizeChanged();
		endRemoveRows();
		return o;
	} else {
		qWarning("%s", tr("ERROR: take() failed - index %1 out of bounds!").arg(i).toStdString().c_str());
		return nullptr;
	}
}



QObject *List::get(int i)
{
	return ((i > -1) && (i < _data.size()))? _data[i]:nullptr;
}

void List::move(int from, int to)
{
	if (inBounds(from) && inBounds(to)) {
		if(beginMoveRows(QModelIndex(), from, from ,QModelIndex(), to)){
			_data.move(from, to);
		endMoveRows();
		}
		else {

			// Thanks for such a WONDERFUL API. The View model abstraction
			// is used so often, is so poorly documented and has so few
			// convenience wrappers in the base system, that I just.... WHY???
			// WHY SILENTLY FAIL? WHY NOT HAVE A WARNING OR A STATIC ASSERT.
			// THIS kind of design gives OOP a bad Reputation. WTF?
			if(beginMoveRows(QModelIndex(), to, to, QModelIndex(), from)){
				_data.move(from, to);
				endMoveRows();
			} else {
				qWarning("%s", tr("Something actually went wrong").toStdString().c_str());
			}
		}
	} else {
		if(inBounds(from)){
			qWarning("%s", tr("Error: move failed. — to %1 out of bounds").arg(to).toStdString().c_str());
		}else {
			qWarning("%s", tr("ERROR: move failed. — to %1 out of bounds").arg(from).toStdString().c_str());
		}

	}
}

bool List::moveRows(const QModelIndex& sourceParent, int sourceFirst, int sourceLast, const QModelIndex& destinationParent, int destinationChild)
{
	if(sourceParent != destinationParent){
		return false;
	}
	if(inBounds(sourceFirst) && inBounds(destinationChild)){
		if(beginMoveRows(sourceParent, sourceFirst, sourceLast, destinationParent, destinationChild))
			_data.move(sourceFirst, sourceLast);
		endMoveRows();
		return true;
	}

	return false;

}

void List::internalChange(QObject *o)
{
	// added to force sort/filter reevaluation
	int i = _data.indexOf(o);
	if (i == -1) {
		qWarning("%s", tr("internal change failed, obj not found").toStdString().c_str());
		return;
	}
	dataChanged(index(i), index(i));

}

bool List::isEmpty(){
	return _data.isEmpty();
}

List &List::operator=(const List &nu)
{
	this->_data = nu._data;
	return *this;
}

int List::rowCount(const QModelIndex &p) const
{
	Q_UNUSED(p)
	return _data.size();
}

QVariant List::data(const QModelIndex &index, int role) const
{
	Q_UNUSED(role)
	return QVariant::fromValue(_data[index.row()]);
}

QHash<int, QByteArray> List::roleNames() const
{
	static QHash<int, QByteArray> * pHash;
	if (!pHash) {
		pHash = new QHash<int, QByteArray>;
		(*pHash)[Qt::UserRole + 1] = "object";
	}
	return *pHash;
}

QQmlListProperty<QObject> List::content()
{
	return {this, _data};
}

QObject *List::operator[](int i)
{
	return _data[i];
}

int List::indexOf(QObject* element) const
{
	return _data.indexOf(element);
}

void List::add(QObject *o)
{
	int i = _data.size();
	beginInsertRows(QModelIndex(), i, i);
	_data.append(o);
	o->setParent(this);
	sizeChanged();
	endInsertRows();
}
