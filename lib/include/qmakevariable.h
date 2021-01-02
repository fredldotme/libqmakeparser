#ifndef QMAKEVARIABLE
#define QMAKEVARIABLE

#include <QString>

#include "qmaketypes.h"

struct QMakeVariable {
	std::vector<QString> values;
};

#endif