#ifndef QMAKEBLOCK
#define QMAKEBLOCK

#include <QString>

#include <memory>
#include <vector>
#include <map>

#include "qmakevariable.h"

struct QMakeBlock {
	std::shared_ptr<QMakeBlock> parent;
	std::vector<std::shared_ptr<QMakeBlock> > subBlocks;

	// Should be friend for QMakeReader
	std::vector<QString> logicalLine;
};

struct QMakeOptionalBlock : public QMakeBlock {
	QMakeOptionalBlock(bool enabled) : QMakeBlock() {
		this->enabled = enabled;
	}
	bool enabled = true;
};

#endif