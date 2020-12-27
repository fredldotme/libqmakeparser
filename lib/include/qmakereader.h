#ifndef QMAKEREADER
#define QMAKEREADER

#include <QObject>

class QMakeReader : public QObject {
	Q_OBJECT

public:
	QMakeReader(QObject* parent = nullptr);
	bool loadFile(const QString& filePath);

private:
	bool handleCharacter(ushort character);
	void processWordBuffer();

	QString m_wordBuffer;
	QString m_lastWord;
};

#endif