#include <qmakeparser.h>
#include <qmakecursor.h>

#include <QFile>

#include <iostream>

QMakeReader::QMakeReader(QObject* parent) : QObject(parent)
{
	
}

bool QMakeReader::loadFile(const QString& filePath)
{
	QFile file(filePath);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		return false;
	}

	const QString content = QString::fromUtf8(file.readAll());
	QMakeCursor cursor(nullptr, content);
	cursor.handleCharacter = [=](ushort character) {
		std::cout << (char)character;
	};

	cursor.process();
	return true;
}