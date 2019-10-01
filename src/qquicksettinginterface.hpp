#pragma once

#include <QtCore/QObject>

#include <QSettings>

class QQuickSettingInterface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY darkModeChanged)
	Q_PROPERTY(bool autoSync READ autoSync WRITE setAutoSync NOTIFY autoSyncChanged)
	Q_PROPERTY(bool allowEval READ allowEval WRITE setAllowEval NOTIFY allowEvalChanged)
public:
	explicit QQuickSettingInterface(QObject *parent = nullptr);
	bool m_darkMode;
	bool m_autoSync;
	bool m_allowEval;
	QSettings settings;

	bool allowEval() const;
	void setAllowEval(bool allowEval);

	bool darkMode() const;
	void setDarkMode(bool darkMode);

	bool autoSync() const;
	void setAutoSync(bool autoSync);
signals:
	void darkModeChanged();
	void autoSyncChanged();
	void allowEvalChanged();
public slots:
	void writeRecentFilesToDisk(QStringList files);
};

