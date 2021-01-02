#ifndef QMAKETYPES
#define QMAKETYPES

enum QMakeProjectType {
	TYPE_UNKNOWN = 0,
	TYPE_APP,
	TYPE_LIB,
	TYPE_SUBDIRS,
	TYPE_AUX
};

enum QMakeParseError {
	ERROR_UNKNOWN = 0,
	ERROR_FILE_NOT_PROVIDED,
	ERROR_FILE_NOT_FOUND,
	ERROR_FILE_LOAD_FAILED
};

enum QMakeCursorMode {
	MODE_NORMAL = 0,
	MODE_BLIND
};

/*enum QMakeKeyword {
	KEYWORD_NONE = 0,
	KEYWORD_TEMPLATE,
	KEYWORD_TARGET,
	KEYWORD_SOURCES,
	KEYWORD_HEADERS,
	KEYWORD_INCLUDEPATH,
	KEYWORD_CONFIG,
	KEYWORD_SUBDIRS
};*/

enum QMakeLineType {
	LINETYPE_NONE = 0,
	LINETYPE_VARIABLE_ASSIGNMENT,
	LINETYPE_VARIABLE_ADDITION,
	LINETYPE_VARIABLE_SUBTRACTION
};

enum QMakeVariableType {
	VARIABLETYPE_VALUE,
	VARIABLETYPE_LIST
};

#endif