TEMPLATE = lib
TARGET = qmakeparser
CONFIG += unversioned_libname unversioned_soname

QT = core

INCLUDEPATH += \
	$$PWD/include

SOURCES += \
	$$PWD/src/qmakeparser.cpp \
	$$PWD/src/qmakereader.cpp \
	$$PWD/src/qmakecursor.cpp \
	$$PWD/src/qmakeblock.cpp

HEADERS += \
	$$PWD/include/qmakeparser.h \
	$$PWD/include/qmakereader.h \
	$$PWD/include/qmakecursor.h \
	$$PWD/include/qmakevariable.h \
	$$PWD/include/qmaketypes.h