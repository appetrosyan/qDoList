#include "filelistmodel.hpp"
#include "../trackedfile.hpp"

FileListModel::FileListModel(QObject* parent) : List(parent)
{

}

void FileListModel::addCurrentlyActiveFile(const QString& fname)
{
	if(!m_files.contains(fname)){
		m_files.insert(fname, new TrackedFile(fname));
		this->add(m_files[fname]);
	}
	m_activeTrackedFile = m_files[fname];
	connect(m_activeTrackedFile, &TrackedFile::wantAttention, this, &FileListModel::trackFile);
}

TrackedFile* FileListModel::activeTrackedFile()
{
	return m_activeTrackedFile;
}

QStringList FileListModel::fileNames()
{
	QStringList qlst;
	for(QString str: m_files.keys()){
		if(!str.isEmpty() && str!="" && m_files[str]->m_isWritable){
			qlst.append(str);
		}
	}
	return qlst;
}

int FileListModel::activeTrackedFileIndex()
{
	return _data.indexOf(m_activeTrackedFile);
}

void FileListModel::trackFile(TrackedFile* file)
{
	m_activeTrackedFile = file;
	emit activeTrackedFileChanged();
}

