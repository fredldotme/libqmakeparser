#include <qmakeparser.h>

#include <QFile>

using namespace std;

QMakeParser::QMakeParser(QObject* parent) : QObject(parent)
{
	QObject::connect(this, &QMakeParser::filePathChanged,
		this, &QMakeParser::parseProject, Qt::DirectConnection);
}

void QMakeParser::setProjectFile(QString filePath)
{
	if (this->m_filePath == filePath)
		return;

	this->m_filePath = filePath;
	Q_EMIT filePathChanged();
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

	const bool loadSuccess = m_reader.loadFile(this->m_filePath);
	if (!loadSuccess) {
		Q_EMIT parseError(QMakeParseError::ERROR_FILE_LOAD_FAILED);
		return;
	}
}

map<QString, QMakeVariable> QMakeParser::getVariables()
{
	return this->m_reader.getVariables();
}