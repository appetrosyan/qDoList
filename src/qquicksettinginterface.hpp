#pragma once

#include <QtCore/QObject>

#include <QSettings>

class QQuickSettingInterface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY darkModeChanged)
	Q_PROPERTY(bool autoSync READ autoSync WRITE setAutoSync NOTIFY autoSyncChanged)
public:
	explicit QQuickSettingInterface(QObject *parent = nullptr);
	bool m_darkMode;
	bool m_autoSync;
	QSettings settings;

	bool darkMode() const;
	void setDarkMode(bool darkMode);

	bool autoSync() const;
	void setAutoSync(bool autoSync);
signals:
	void darkModeChanged();
	void autoSyncChanged();
public slots:
	void writeRecentFilesToDisk(QStringList files);
};

