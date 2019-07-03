#pragma once

#include <QObject>
#include <QSyntaxHighlighter>


class NaturalLanguageHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	NaturalLanguageHighlighter(QTextDocument* parent=nullptr);
	void formatText(const QString& text, const QRegularExpression& re, const QTextCharFormat& fmt);

	void formatCommand(const QString& text, const QRegularExpression& re, const QTextCharFormat& fmt = QTextCharFormat());
	void formatCaptureGroup(const QString& text, const QRegularExpression& re, const QTextCharFormat& fmt, int group);
protected:
	void highlightBlock(const QString& text) override;
	QTextCharFormat dueFormat;
	QTextCharFormat commentFormat;
	QTextCharFormat scheduledFormat;
	QTextCharFormat subTaskFormat;
	QTextCharFormat commandFormat;
};

