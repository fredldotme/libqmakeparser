#ifndef QMAKEREADER
#define QMAKEREADER

#include <QObject>

#include <memory>
#include <vector>

#include "qmakecursor.h"
#include "qmakeblock.h"
#include "qmaketypes.h"
#include "qmakevariable.h"

class QMakeReader : public QObject {
	Q_OBJECT

public:
	QMakeReader(QObject* parent = nullptr);
	bool loadFile(const QString& filePath);
	void feedValues(const QString& value);
	std::map<QString, QMakeVariable> getVariables() {
		return this->m_variables;
	}

private:
	bool checkBracketCount();
	bool bracketChecker(QMakeCursorPos*);
	bool handleCharacter(QMakeCursorPos*);
	void processWordBuffer();
	void processLogicalLine();
	void processLogicalLine(const std::vector<QString>& line);
	bool isListContains();
	bool hasListValue();
	bool hasOsSpecifier(const QString& value);
	bool isRequestedOs(const QString& value);

	QMakeCursorMode m_mode = QMakeCursorMode::MODE_NORMAL;
	bool m_continueLine = false;
	QString m_wordBuffer;
	QString m_lastWord;
	std::shared_ptr<QMakeBlock> m_rootBlock;
	std::shared_ptr<QMakeBlock> m_currentBlock;

	quint64 m_openBrackets = 0;
	quint64 m_closedBrackets = 0;
	quint64 m_openCurlyBrackets = 0;
	quint64 m_closedCurlyBrackets = 0;

	std::map<QString, QMakeVariable> m_variables;
};

#endif