#ifndef QMAKEBLOCK
#define QMAKEBLOCK

#include <QString>

#include <memory>
#include <vector>

struct QMakeBlock {
	std::shared_ptr<QMakeBlock> parent;
	std::vector<std::shared_ptr<QMakeBlock> > subBlocks;

	std::map<QString, QMakeVariable> variables;

	// Should be friend for QMakeReader
	std::vector<QString> logicalLine;
};

#endif