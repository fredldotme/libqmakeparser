#ifndef QMAKEPARSER
#define QMAKEPARSER

#include <QObject>
#include <QString>

#include <vector>

#include "qmaketypes.h"
#include "qmakereader.h"

class QMakeParser : public QObject {
	Q_OBJECT

public:
	QMakeParser(QObject* parent = nullptr);

	void setProjectFile(QString filePath);
	
private:
	void parseProject();

	QMakeReader m_reader;

	QString m_filePath;
	std::vector<QMakeParser*> m_subdirs;
	std::vector<QString> m_pris;
	std::vector<QString> m_configs;
	std::vector<QString> m_includePaths;
	std::vector<QString> m_sources;
	std::vector<QString> m_headers;
	std::vector<QString> m_defines;

Q_SIGNALS:
	void filePathChanged();
	void parseError(QMakeParseError error);
	void buildInstructionGenerated(QString contents);

};

#endif