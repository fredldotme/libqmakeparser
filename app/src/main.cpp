#include <qmakeparser.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2) {
		cerr << "No qmake file provided" << endl;
		return 1;
	}

	QMakeParser parser(nullptr);
	parser.setProjectFile(QString::fromUtf8(argv[1], strlen(argv[1])));

	if (argc > 2) {
		QStringList args;
		for (int i = 2; i < argc; i++) {
			const QString arg = QString::fromUtf8(argv[i], strlen(argv[i]));
			args.append(arg);
		}
		parser.feedValues(args);
	}

	map<QString, QMakeVariable> variables = parser.getVariables();
	for (auto keyValPair : variables) {
		cout << "KEY: " << keyValPair.first.toStdString() << endl;
		for (int i = 0; i < keyValPair.second.values.size(); i++) {
			cout << "\tValue: " << keyValPair.second.values[i].toStdString() << endl;
		}
	}

	return 0;
}
