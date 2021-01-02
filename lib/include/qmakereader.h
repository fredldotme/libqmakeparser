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
	std::map<QString, QMakeVariable> getVariables() {
		return this->m_variables;
	}

private:
	bool handleCharacter(QMakeCursorPos*);
	void processWordBuffer();
	void processLogicalLine();
	bool isListContains();
	bool hasListValue();

	QMakeCursorMode m_mode = QMakeCursorMode::MODE_NORMAL;
	bool m_continueLine = false;
	QString m_wordBuffer;
	QString m_lastWord;
	std::shared_ptr<QMakeBlock> m_rootBlock;
	std::shared_ptr<QMakeBlock> m_currentBlock;

	std::map<QString, QMakeVariable> m_variables;
};

#endif