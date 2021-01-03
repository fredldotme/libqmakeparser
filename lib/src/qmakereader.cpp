#include <qmakeparser.h>
#include <qmakecursor.h>

#include <QFile>
#include <QSysInfo>

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

	// First pass: check for brackets and curly brackets
	QMakeCursor bracketCheckCursor(nullptr, content);
	bracketCheckCursor.handleCharacter = std::bind(&QMakeReader::bracketChecker, this,
		std::placeholders::_1);
	const bool bracketCheckSuccess = bracketCheckCursor.process();
	if (!bracketCheckSuccess)
		return false;

	// Pass two: parse the contents
	QMakeCursor cursor(nullptr, content);
	cursor.handleCharacter = std::bind(&QMakeReader::handleCharacter, this, std::placeholders::_1);
	const bool parseSuccess = cursor.process();
	if (!parseSuccess)
		return false;
	return true;
}

bool QMakeReader::checkBracketCount()
{
	if (this->m_openBrackets != this->m_closedBrackets) {
		cerr << "Bracket mismatch" << endl;
		return false;
	}
	if (this->m_openCurlyBrackets != this->m_closedCurlyBrackets) {
		cerr << "Curly bracket mismatch" << endl;
		return false;
	}
	return true;
}

bool QMakeReader::bracketChecker(QMakeCursorPos* pos)
{
	QChar qChar = QChar(pos->currentChar.unicode());

	if (pos->isEOF()) {
		if (!checkBracketCount())
			return false;
	}
	else if (qChar == QChar('(')) {
		++this->m_openBrackets;
	}
	else if (qChar == QChar(')')) {
		++this->m_closedBrackets;
	}
	else if (qChar == QChar('{')) {
		++this->m_openCurlyBrackets;
	}
	else if (qChar == QChar('}')) {
		++this->m_closedCurlyBrackets;
	}

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

bool QMakeReader::hasOsSpecifier(const QString& value)
{
	return value.contains("unix") || value.contains("linux") ||
		value.contains("macx") || value.contains("windows") ||
		value.contains("android") || value.contains("ios");
}

bool QMakeReader::isRequestedOs(const QString& value)
{
	if (!value.contains(':'))
		return (QSysInfo::productType() == value || QSysInfo::kernelType() == value);

	const QStringList values = value.split(':');
	for (const QString& assignedValue : values) {
		if (QSysInfo::productType() == assignedValue || QSysInfo::kernelType() == assignedValue)
			return true;
	}

	return false;
}

void QMakeReader::processLogicalLine()
{
	processWordBuffer();

	bool isEmpty = true;
	if (this->m_currentBlock->logicalLine.size() > 0) {
		for (const QString& word : this->m_currentBlock->logicalLine) {
			if (!word.trimmed().isEmpty()) {
				isEmpty = false;
				break;
			}
		}
	}

	if (isEmpty) {
		this->m_currentBlock->logicalLine.clear();
		return;
	}

	QMakeLineType lineType = QMakeLineType::LINETYPE_NONE;

	// cout << "Block address:" << this->m_currentBlock.get() << endl;
	// cout << "Logical line begin:" << endl;
	for (int i = 0; i < this->m_currentBlock->logicalLine.size(); i++) {
		const QString& word = this->m_currentBlock->logicalLine[i];

		if (i == 0) {
			// We only care about the first word in case it's an OS-specific block
			if (hasOsSpecifier(word)) {
				if (!isRequestedOs(word)) {
					break;
				} else {
					continue;
				}
			}
		}
		else if (i == 1) {
			if (word == QString("=")) {
				lineType = QMakeLineType::LINETYPE_VARIABLE_ASSIGNMENT;
				// Clear existing values from the variable in question
				const QString& key = this->m_currentBlock->logicalLine[0];
				this->m_variables[key].values.clear();
			}
			else if (word == QString("+=")) {
				lineType = QMakeLineType::LINETYPE_VARIABLE_ADDITION;
			}
			else if (word == QString("-=")) {
				lineType = QMakeLineType::LINETYPE_VARIABLE_SUBTRACTION;
			}
		}
		else if (lineType != QMakeLineType::LINETYPE_NONE) {
			const QString& key = this->m_currentBlock->logicalLine[0];
			QMakeVariable& var = this->m_variables[key];

			switch (lineType) {
			case QMakeLineType::LINETYPE_VARIABLE_ADDITION:
			case QMakeLineType::LINETYPE_VARIABLE_ASSIGNMENT:
				var.values.push_back(this->m_currentBlock->logicalLine[i]);
				break;
			case QMakeLineType::LINETYPE_VARIABLE_SUBTRACTION:
				for (auto it = var.values.begin(); it != var.values.end();) {
					if (*it == this->m_currentBlock->logicalLine[i]) {
						it = var.values.erase(it);
					} else {
						++it;
					}
				}
				break;
			}
		}
		// cout << word.toStdString() << endl;
	}
	// cout << "Logical line end" << endl;

	this->m_currentBlock->logicalLine.clear();
}

// Read the processed wordBuffer and find out whether
// the current read is a contains(<LISTNAME>, <VALUE>) {}
bool QMakeReader::isListContains()
{
	if (this->m_currentBlock->logicalLine.size() <= 0) {
		return false;
	}

	const QString& logicalLineStart = this->m_currentBlock->logicalLine[0];
	if (logicalLineStart.startsWith(QStringLiteral("contains"))) {
		return true;
	}

	return false;
}

bool QMakeReader::hasListValue()
{
	if (this->m_currentBlock->logicalLine.size() <= 2) {
		return false;
	}

	const QString& listName = this->m_currentBlock->logicalLine[1];
	const QString& valName = this->m_currentBlock->logicalLine[2];

	if (this->m_variables.find(listName) != this->m_variables.end()) {
		const QMakeVariable& variable = this->m_variables[listName];
		for (auto value : variable.values) {
			if (value == valName)
				return true;
		}
	}

	return false;
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
		// Process word buffer now so that we can properly
		// enable or disable optional blocks
		processLogicalLine();

		std::shared_ptr<QMakeBlock> newBlock;
		if (isListContains())
			newBlock = std::make_shared<QMakeOptionalBlock>(hasListValue());
		else
			newBlock = std::make_shared<QMakeBlock>();

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
	else if (qChar == QChar('(') || qChar == QChar(')')) {
		// Silently skip brackets, but process the word
		processWordBuffer();
	}
	else if (qChar == QChar(',')) {
		// Silently skip commas
	}
	else if (qChar == QChar('\\')) {
		this->m_continueLine = true;
	}
	else if (qChar == QChar(' ')) {
		processWordBuffer();
	}
	else if (qChar == QChar('\n') || pos->isEOF()) {
		if (pos->isEOF() && this->m_currentBlock != this->m_rootBlock) {
			cout << "Failed to close root block at " << pos->x << "x" << pos->y << endl;
			return false;
		}
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
	else if (this->m_mode == QMakeCursorMode::MODE_BLIND) {
		// Silently skip in blind mode (reading comment)
	}
	else {
		cout << "Failed to read at pos " << pos->x << "x" << pos->y << endl;
		return false;
	}

	return true;
}