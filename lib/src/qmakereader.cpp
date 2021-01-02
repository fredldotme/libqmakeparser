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
	if (!this->m_wordBuffer.isEmpty()) {
		this->m_lastWord = this->m_wordBuffer;
		this->m_currentBlock->logicalLine.push_back(this->m_lastWord);	
	}
	this->m_wordBuffer.clear();
}

void QMakeReader::processLogicalLine()
{
	processWordBuffer();

	bool isEmpty = true;
	for (const QString& word : this->m_currentBlock->logicalLine) {
		if (!word.trimmed().isEmpty()) {
			isEmpty = false;
			break;
		}
	}

	if (isEmpty) {
		this->m_currentBlock->logicalLine.clear();
		return;
	}

	QMakeKeyword keyword = QMakeKeyword::KEYWORD_NONE;

	cout << "Block address:" << this->m_currentBlock.get() << endl;
	cout << "Logical line begin:" << endl;
	for (int i = 0; i < this->m_currentBlock->logicalLine.size(); i++) {
		const QString& word = this->m_currentBlock->logicalLine[i];

		if (i == 0) {
			if (word == QString("TEMPLATE")) {
				keyword = QMakeKeyword::KEYWORD_TEMPLATE;
			} else if (word == QString("TARGET")) {
				keyword = QMakeKeyword::KEYWORD_TARGET;
			} else if (word == QString("SOURCES")) {
				keyword = QMakeKeyword::KEYWORD_SOURCES;
			} else if (word == QString("HEADERS")) {
				keyword = QMakeKeyword::KEYWORD_HEADERS;
			} else if (word == QString("INCLUDEPATH")) {
				keyword = QMakeKeyword::KEYWORD_INCLUDEPATH;
			} else if (word == QString("CONFIG")) {
				keyword = QMakeKeyword::KEYWORD_CONFIG;
			} else if (word == QString("SUBDIRS")) {
				keyword = QMakeKeyword::KEYWORD_SUBDIRS;
			}
		}
		cout << word.toStdString() << endl;
	}
	cout << "Logical line end" << endl;

	this->m_currentBlock->logicalLine.clear();
}

bool QMakeReader::handleCharacter(QMakeCursorPos* pos)
{
	// cout << (char)character;

	QChar qChar = QChar(pos->currentChar.unicode());

	// cout << "x=" << pos->x << " y=" << pos->y << " char=" << qChar.toLatin1() << endl;

	if (pos->x == 0 && pos->y == 0) {
		this->m_rootBlock = std::make_shared<QMakeBlock>();
		this->m_currentBlock = this->m_rootBlock;
	}

	if (qChar == QChar('{')) {
		std::shared_ptr<QMakeBlock> newBlock = std::make_shared<QMakeBlock>();
		newBlock->parent = this->m_currentBlock;
		this->m_currentBlock->subBlocks.push_back(newBlock);
		this->m_currentBlock = this->m_currentBlock->subBlocks.back();
	}
	else if (qChar == QChar('}')) {
		processLogicalLine();
		this->m_currentBlock = this->m_currentBlock->parent;
	}
	else if (qChar == QChar('\t')) {
		// Silently skip tabs
	}
	else if (qChar == QChar('\\')) {
		this->m_continueLine = true;
	}
	else if (qChar == QChar(' ')) {
		processWordBuffer();
	}
	else if (qChar == QChar('\n') || pos->isEOF()) {
		if (this->m_mode != QMakeCursorMode::MODE_BLIND)
			this->m_wordBuffer += qChar;

		if (!this->m_continueLine)
			processLogicalLine();

		this->m_continueLine = false;
		this->m_mode = QMakeCursorMode::MODE_NORMAL;
	}
	else if (qChar == QChar('#')) {
		this->m_mode = QMakeCursorMode::MODE_BLIND;
	}
	else if (this->m_mode != QMakeCursorMode::MODE_BLIND) {
		this->m_wordBuffer += qChar;
	}

	return true;
}