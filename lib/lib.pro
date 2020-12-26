TEMPLATE = lib
TARGET = qmakeparser

QT = core

INCLUDEPATH += \
	$$PWD/include

SOURCES += \
	$$PWD/src/qmakeparser.cpp \
	$$PWD/src/qmakereader.cpp \
	$$PWD/src/qmakecursor.cpp

HEADERS += \
	$$PWD/include/qmakeparser.h \
	$$PWD/include/qmakereader.h \
	$$PWD/include/qmakecursor.h \
	$$PWD/include/qmaketypes.h