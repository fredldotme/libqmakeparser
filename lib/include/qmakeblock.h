#ifndef QMAKEBLOCK
#define QMAKEBLOCK

#include <QString>

#include <memory>
#include <vector>

struct QMakeBlock {
	std::shared_ptr<QMakeBlock> parent;
	std::vector<std::shared_ptr<QMakeBlock> > subBlocks;

	std::vector<QString> logicalLine;
};

#endif