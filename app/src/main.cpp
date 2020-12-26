#include <qmakeparser.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		cerr << "No qmake file provided" << endl;
		return 1;
	}

	QMakeParser parser(nullptr);
	parser.setProjectFile(QString::fromUtf8(argv[1], strlen(argv[1])));

	return 0;
}