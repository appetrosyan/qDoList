#pragma once
#include <QtCore/QAbstractItemModel>
#include <QtQml/QQmlListProperty>

//class Task;

class List : public QAbstractListModel {
	Q_OBJECT
	Q_PROPERTY(int size READ size NOTIFY sizeChanged)
	Q_PROPERTY(QQmlListProperty<QObject> content READ content)
	Q_PROPERTY(QObject * parent READ parent WRITE setParent)
	Q_CLASSINFO("DefaultProperty", "content")
public:
	typedef QList<QObject*>::iterator iterator;
	typedef QList<QObject*>::const_iterator const_iterator;
	iterator begin() { return _data.begin();}
	const_iterator cbegin() {return _data.cbegin();}
	iterator end() { return _data.end();}
	const_iterator cend() {return _data.cend();}
	QObject* back() {return _data.back();}
	List(QObject *parent = nullptr) : QAbstractListModel(parent) { }
	List &operator=(const List& nu);
	int rowCount(const QModelIndex &p=QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;
	int size() const { return _data.size(); }
	QQmlListProperty<QObject> content();
	QObject * operator[](int i);
	int indexOf(QObject* element) const;
public slots:
	void insert(QObject * o, int i);
	void clear();
	QObject *take(int i);
	QObject *get(int i);
	void move(int from, int to);
	bool moveRows(const QModelIndex& sourceParent, int sourceRow,int sourceLast,  const QModelIndex& destinationParent, int destinationChild ) override;
	void add(QObject * o);
	void internalChange(QObject * o);
	bool isEmpty();
signals:
	void sizeChanged();
protected:
	QList<QObject *> _data;
	bool inBounds(int index){
		return (index > -1) && (index < _data.size());
	}
};
