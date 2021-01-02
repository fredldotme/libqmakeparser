#ifndef QMAKEVARIABLE
#define QMAKEVARIABLE

#include <QString>

#include "qmakevariable.h"

struct QMakeVariableVal {
	
};

struct QMakeVariableListVal : public QMakeVariableVal {
	
};

struct QMakeVariable {
	QMakeVariableType type;
	QMakeVariableVal value;
};

#endif