#include <qmakecursor.h>

QMakeCursor::QMakeCursor(QObject* parent, const QString& content) :
	QObject(parent), m_currentPos(new QMakeCursorPos(content))
{
}

QMakeCursor::~QMakeCursor()
{
	if (this->m_currentPos) {
		delete this->m_currentPos;
		this->m_currentPos = nullptr;
	}
}

void QMakeCursor::process()
{
	while (proceed())
	{
		this->handleCharacter(this->m_currentPos->currentChar.unicode());
	}
	return;
}

bool QMakeCursor::proceed()
{
	QMakeCursorPos prevPos = (*this->m_currentPos);
	const bool hasRead = prevPos != ++(*this->m_currentPos);

	if (hasRead) {
		Q_EMIT proceeded(*this->m_currentPos);
	}

	return hasRead;
}
