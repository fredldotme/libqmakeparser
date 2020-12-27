#ifndef QMAKECURSOR
#define QMAKECURSOR

#include <QObject>
#include <QDebug>
#include <functional>

struct QMakeCursorPos {
	Q_GADGET

public:
	QMakeCursorPos(const QString& content)
	{
		this->m_content = content;
	};

	QMakeCursorPos& operator++()
	{
		// Handle EOF
		if ((this->m_readPos + 1) > this->m_content.length())
			return *this;

		const QChar nextChar = this->m_content.at(++this->m_readPos);
		if (nextChar == QChar('\n')) {
			++this->y;
			this->x = 0;
		} else {
			++this->x;
		}

		this->currentChar = nextChar;
		return *this;
	}

	QMakeCursorPos operator++(int)
	{
		QMakeCursorPos newPos(*this);
		++(*this);
		return newPos;
	}

	bool operator==(const QMakeCursorPos& o)
	{
		return this->x == o.x &&
				this->y == o.y &&
				this->m_content == o.m_content &&
				this->m_readPos == o.m_readPos &&
				this->currentChar == o.currentChar;
	}

	bool operator!=(const QMakeCursorPos& o)
	{
		return !(*this == o);
	}

	qint64 x = -1;
	qint64 y = -1;
	QChar currentChar = QChar('\0');

	bool isValidChar()
	{
		return x >= 0 && y >= 0;
	}

	bool isEOF()
	{
		return this->m_readPos >= this->m_content.length();
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

	std::function<bool(QMakeCursorPos*)> handleCharacter;
	void process();

private:
	bool proceed();

	QMakeCursorPos* m_currentPos = nullptr;

Q_SIGNALS:
	void proceeded(QMakeCursorPos);
	void readError(QMakeCursorPos);
};

#endif
