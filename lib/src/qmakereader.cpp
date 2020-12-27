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
	cout << this->m_wordBuffer.toStdString() << endl;
	this->m_lastWord = this->m_wordBuffer;
	this->m_wordBuffer.clear();
}

bool QMakeReader::handleCharacter(ushort character)
{
	cout << (char)character;

	QChar qChar = QChar(character);
	if (qChar.isLetter() || qChar.isNumber()) {
		m_wordBuffer += qChar;
	} else if (qChar == QChar(' ') || qChar == QChar('\t')) {
		// Silently skip tabs and spaces
	} else {
		processWordBuffer();
	}

	return true;
}