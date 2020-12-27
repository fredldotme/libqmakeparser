#include <qmakeparser.h>
#include <qmakecursor.h>

#include <QFile>

#include <iostream>

using namespace std;

QMakeReader::QMakeReader(QObject* parent) : QObject(parent)
{

}

bool QMakeReader::loadFile(const QString& filePath)
{
	QFile file(filePath);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		return false;
	}

	const QString content = QString::fromUtf8(file.readAll());

	QMakeCursor cursor(nullptr, content);

	cursor.handleCharacter = std::bind(&QMakeReader::handleCharacter, this, std::placeholders::_1);

	cursor.process();
	return true;
}

void QMakeReader::processWordBuffer()
{
	this->m_wordBuffer = this->m_wordBuffer.trimmed();
	// cout << this->m_wordBuffer.toStdString() << endl;
	this->m_lastWord = this->m_wordBuffer;
	this->m_logicalLine.push_back(this->m_lastWord);
	this->m_wordBuffer.clear();
}

void QMakeReader::processLogicalLine()
{
	processWordBuffer();
	for (const QString& word : this->m_logicalLine) {
		cout << word.toStdString() << endl;
	}
}

bool QMakeReader::handleCharacter(ushort character)
{
	// cout << (char)character;

	QChar qChar = QChar(character);

	if (qChar.isLetter() || qChar.isNumber()) {
		this->m_wordBuffer += qChar;
	} else if (qChar == QChar('\t')) {
		// Silently skip tabs
	} else if (qChar == QChar('\\') || qChar == QChar(' ')) {
		processWordBuffer();
	} else if (qChar == QChar('\n')) {
		processLogicalLine();
	}

	return true;
}