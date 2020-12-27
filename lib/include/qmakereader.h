#ifndef QMAKEREADER
#define QMAKEREADER

#include <QObject>

#include <vector>

class QMakeReader : public QObject {
	Q_OBJECT

public:
	QMakeReader(QObject* parent = nullptr);
	bool loadFile(const QString& filePath);

private:
	bool handleCharacter(ushort character);
	void processWordBuffer();
	void processLogicalLine();

	QString m_wordBuffer;
	QString m_lastWord;
	std::vector<QString> m_logicalLine;
};

#endif