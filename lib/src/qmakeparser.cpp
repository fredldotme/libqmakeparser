#include <qmakeparser.h>

#include <QFile>

using namespace std;

QMakeParser::QMakeParser(QObject* parent) : QObject(parent)
{
	QObject::connect(this, &QMakeParser::filePathChanged,
		this, &QMakeParser::parseProject, Qt::DirectConnection);
}

void QMakeParser::setProjectFile(const QString& filePath)
{
	if (this->m_filePath == filePath)
		return;

	this->m_filePath = filePath;
	Q_EMIT filePathChanged();
}

QString QMakeParser::projectFile()
{
    return this->m_filePath;
}

void QMakeParser::feedValues(const QStringList& values)
{
	for (const QString& value : values) {
		this->m_reader.feedValues(value);
	}
}

void QMakeParser::parseProject()
{
	if (this->m_filePath.isEmpty()) {
		Q_EMIT parseError(QMakeParseError::ERROR_FILE_NOT_PROVIDED);
		return;
	}

	if (!QFile::exists(this->m_filePath)) {
		Q_EMIT parseError(QMakeParseError::ERROR_FILE_NOT_FOUND);
		return;
	}

	const bool loadSuccess = this->m_reader.loadFile(this->m_filePath);
	if (!loadSuccess) {
		Q_EMIT parseError(QMakeParseError::ERROR_FILE_LOAD_FAILED);
		return;
	}
}

map<QString, QMakeVariable> QMakeParser::getVariables()
{
	return this->m_reader.getVariables();
}
