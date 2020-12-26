#ifndef QMAKECURSOR
#define QMAKECURSOR

#include <QObject>
#include <functional>

struct QMakeCursorPos {
	Q_GADGET

public:
	QMakeCursorPos(const QString& content)
	{
		this->m_content = content;
	};

	QMakeCursorPos operator++(int i)
	{
		QMakeCursorPos newPos(this->m_content);
		newPos.m_readPos = this->m_readPos;

		// Handle EOF
		if ((newPos.m_readPos + i) > newPos.m_content.length())
			return newPos;

		const QChar nextChar = this->m_content.at(++newPos.m_readPos);
		if (nextChar == QChar('\n')) {
			newPos.y += i;
			newPos.x = 0;
		} else {
			newPos.x += i;
		}

		newPos.currentChar = nextChar;
		newPos.m_readPos += i;
		return newPos;
	}

	bool operator!=(const QMakeCursorPos& o)
	{
		return this->x == o.x &&
				this->y == o.y &&
				this->m_content == o.m_content &&
				this->m_readPos == o.m_readPos;
	}

	qint64 x = -1;
	qint64 y = -1;
	QChar currentChar = QChar('\0');

	bool isValidChar()
	{
		return x >= 0 && y >= 0;
	}

private:
	QString m_content = QByteArrayLiteral("");
	qint64 m_readPos = -1;
};

class QMakeCursor : public QObject {
	Q_OBJECT

public:
	QMakeCursor(QObject* parent = nullptr, const QString& content = QStringLiteral(""));
	~QMakeCursor();

	std::function<void(ushort)> handleCharacter;
	void process();

private:
	bool proceed();

	QMakeCursorPos* m_currentPos = nullptr;

Q_SIGNALS:
	void proceeded(QMakeCursorPos);
	void readError(QMakeCursorPos);
};

#endif