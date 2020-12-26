TARGET = qmakeparser

TEMPLATE = app
QT = core

INCLUDEPATH += \
	$$PWD/../lib/include

LIBS += \
	-L$$OUT_PWD/../lib -lqmakeparser

SOURCES += \
	$$PWD/src/main.cpp
