#pragma once

#include <QtCore/QRegularExpression>
#include <QStringLiteral>

namespace TaskLang{
	static const QString nameDate("(\\d{1,2}[\\/\\.\\-\\h]+)?(?|Jan(uary|\\.)?|Feb(ruary|\\.)?|Mar(ch|\\.)?|Apr(ril|\\.)?|May|Jun(e|\\.)?|Jul(y|\\.)?|Aug(ust|\\.)?|Sep(tember|\\.)?|Oct(ober|\\.)?|Dec(ember|\\.)?|\\d{1,2})([\\h\\/\\.\\-]*(\\d{1,4}))?");
	static const QString digiDate("(?<![\\d\\.\\/-])(?'pretime'(?'hhmm'\\d{1,2}:\\d{2}(?'hhmmap'[ap]m)?)|(?'hh'[01]?\\d(?'ap'[ap]m)?(?!\\d*[\\.\\/-])))?\\h*(?'ddate'\\d{1,4}(?'sep'[\\.\\/-])+\\d{1,2}(?'extra'\\k'sep'\\d{1,4})?)?\\h*(?'ptime'(?'phhmm'\\d{1,2}:\\d{2}(?'phhmmap'[ap]m)?)|(?'phh'[01]?\\d(?'pap'[ap]m)?(?!\\d*[\\.\\/-])))?");
	static const QString date("(?|(?'date'today|сегодня|завтра|послезавтра|(?:на)*\\h*следующей\\h*недел[еи]|tomorrow|next\\h*(?:week|day|night)|завтра|послезавтра|"+digiDate+"|"+nameDate+")))");
	static const QRegularExpression due("(?:(?:(?:d\\h*=)|(?:by)|(?:до))\\h*"+date, QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression scheduled("(?:(?:(?:s\\h*=)|(?:since)|(?:начиная\\h*со*))\\h*"+QString(date),QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression subTasks("\\D*\\h*((?|:(\\D{1,2}.*)|{\\h*([^} ]+)\\h*}))\\h*$");
	static const QRegularExpression comment("\\(((?:\\h*\\w+\\h*)+)\\)");
	static const QRegularExpression command("\\h*([-+])*\\h*(.*)");
}
