#pragma once

#include <QtCore/QObject>
#include "listmodel.hpp"

class TrackedFile;

class FileListModel : public List
{
	Q_OBJECT
	Q_PROPERTY(TrackedFile* activeTrackedFile READ activeTrackedFile NOTIFY activeTrackedFileChanged)
private:
	TrackedFile* m_activeTrackedFile = nullptr;
	QMap<QString, TrackedFile*> m_files;
public:
	FileListModel(QObject* parent);
	TrackedFile* activeTrackedFile();
public slots:
	void addCurrentlyActiveFile(const QString& fname);
	void trackFile(TrackedFile* file);

	QStringList fileNames();
	int activeTrackedFileIndex();
signals:
	void activeTrackedFileChanged();
};
