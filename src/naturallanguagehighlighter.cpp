#include "naturallanguagehighlighter.h"
#include "tasklang.h"

/**
 * @brief Constructor for the Natural Language parser
 * 
 * @param parent p_parent:...
 */
NaturalLanguageHighlighter::NaturalLanguageHighlighter(QTextDocument* parent) : QSyntaxHighlighter (parent)
{
	dueFormat.setFontWeight(QFont::Bold);
	dueFormat.setForeground(Qt::red);
	commentFormat.setFontItalic(true);
	scheduledFormat.setForeground(Qt::cyan);
	subTaskFormat.setForeground(Qt::gray);
	commandFormat.setFontWeight(QFont::Bold);
}

/**
 * @brief the workHorse of the class it basically highlights everything that needs to be highlighted. 
 * 
 * @param text p_text: The actual text snippet to highlight
 * @param re p_re: If this regExp is matched the part will be highlighted
 * @param fmt p_fmt: This is the QTextCharFormat to be applied uppon matching. 
 */
void NaturalLanguageHighlighter::formatText(const QString& text, const QRegularExpression& re, const QTextCharFormat& fmt){
	QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
	while(matchIterator.hasNext()){
		auto match = matchIterator.next();
		setFormat(match.capturedStart(), match.capturedLength(), fmt);
	}
}

void NaturalLanguageHighlighter::formatCommand(const QString& text, const QRegularExpression& re, const QTextCharFormat& fmt){
	QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
	while(matchIterator.hasNext()){
		auto match = matchIterator.next();
		if(!match.captured(1).isEmpty()){
			setFormat(match.capturedStart(), match.capturedLength(), fmt);
		}
	}
}

void NaturalLanguageHighlighter::formatCaptureGroup(const QString & text, const QRegularExpression& re, const QTextCharFormat& fmt, int group){
	QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
	while(matchIterator.hasNext()){
		auto match = matchIterator.next();
		if(!match.captured(group).isEmpty()){
			setFormat(match.capturedStart(group), match.capturedLength(group), fmt);
		}
	}
}


void NaturalLanguageHighlighter::highlightBlock(const QString& text)
{

	formatText(text, TaskLang::due, dueFormat);
	formatText(text, TaskLang::comment, commentFormat);
	formatText(text, TaskLang::scheduled, scheduledFormat);
	formatText(text, TaskLang::subTasks, subTaskFormat);
	formatCommand(text, TaskLang::command, commandFormat);
}

