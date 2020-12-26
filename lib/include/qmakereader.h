#ifndef QMAKEREADER
#define QMAKEREADER

#include <QObject>

class QMakeReader : public QObject {
	Q_OBJECT

public:
	QMakeReader(QObject* parent = nullptr);
	bool loadFile(const QString& filePath);

};

#endif