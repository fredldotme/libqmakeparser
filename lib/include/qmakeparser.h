#ifndef QMAKEPARSER
#define QMAKEPARSER

#include <QObject>
#include <QString>

#include <vector>
#include <map>
#include <memory>

#include "qmaketypes.h"
#include "qmakereader.h"
#include "qmakeblock.h"

class QMakeParser : public QObject {
	Q_OBJECT

public:
	QMakeParser(QObject* parent = nullptr);

	void setProjectFile(const QString& filePath);
	void feedValues(const QStringList& values);
	std::map<QString, QMakeVariable> getVariables();

private:
	void parseProject();

	QMakeReader m_reader;

	QString m_filePath;
	std::vector<QMakeParser*> m_subdirs;

Q_SIGNALS:
	void filePathChanged();
	void parseError(QMakeParseError error);
	void buildInstructionGenerated(QString contents);

};

#endif