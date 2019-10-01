#include "qquicksettinginterface.hpp"

#include <QSettings>

#define genericSet(x) if(m_##x != (x)){ \
	m_##x = (x); \
	emit x##Changed();\
	}

QQuickSettingInterface::QQuickSettingInterface(QObject *parent) : QObject(parent)
{
	m_darkMode =settings.value("ui/darkMode", false).toBool();
	connect(this, &QQuickSettingInterface::darkModeChanged, this, [=](){settings.setValue("ui/darkMode", m_darkMode);});

	m_autoSync =settings.value("behaviour/autoSync", false).toBool();
	connect(this, &QQuickSettingInterface::darkModeChanged, this, [=](){settings.setValue("behaviour/autoSync", m_autoSync);});

	m_allowEval=settings.value("behaviour/allowEval", false).toBool();
	connect(this, &QQuickSettingInterface::allowEvalChanged, this, [=](){settings.setValue("behaviour/allowEval", m_allowEval);});
}

bool QQuickSettingInterface::autoSync() const
{
	return m_autoSync;
}

void QQuickSettingInterface::setAllowEval(bool allowEval){
	genericSet(allowEval);
}

void QQuickSettingInterface::setAutoSync(bool autoSync)
{
	genericSet(autoSync);
}

void QQuickSettingInterface::writeRecentFilesToDisk(QStringList files)
{
	settings.beginWriteArray("LastOpenedFiles");
	for(int i=0; i< files.count(); ++i){
		settings.setArrayIndex(i);
		settings.setValue("fileName", files.at(i));
	}
	settings.endArray();
}

bool QQuickSettingInterface::allowEval() const
{
	return m_allowEval;
}

bool QQuickSettingInterface::darkMode() const
{
	return m_darkMode;
}

void QQuickSettingInterface::setDarkMode(bool darkMode)
{
	genericSet(darkMode);
}


