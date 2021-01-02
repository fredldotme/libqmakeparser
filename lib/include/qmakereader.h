#ifndef QMAKEREADER
#define QMAKEREADER

#include <QObject>

#include <memory>
#include <vector>

#include "qmakecursor.h"
#include "qmakeblock.h"
#include "qmaketypes.h"

class QMakeReader : public QObject {
	Q_OBJECT

public:
	QMakeReader(QObject* parent = nullptr);
	bool loadFile(const QString& filePath);

private:
	bool handleCharacter(QMakeCursorPos*);
	void processWordBuffer();
	void processLogicalLine();

	QMakeCursorMode m_mode = QMakeCursorMode::MODE_NORMAL;
	bool m_continueLine = false;
	QString m_wordBuffer;
	QString m_lastWord;
	std::shared_ptr<QMakeBlock> m_rootBlock;
	std::shared_ptr<QMakeBlock> m_currentBlock;
};

#endif