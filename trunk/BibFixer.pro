#-------------------------------------------------
#
# Project created by QtCreator 2011-12-20T17:46:18
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = BibFixer
TEMPLATE = app

win32 {
	RC_FILE = Resource.rc
}
mac {
	ICON = Images/Fix.icns
}

SOURCES += main.cpp\
		MainWindow.cpp \
	DlgSettings.cpp \
	DlgAbbreviation.cpp \
	Convertor.cpp \
	TextEdit.cpp \
	Parser.cpp \
	Reference.cpp \
    ../ImageColorBoolModel/ImageColorBoolProxy.cpp \
    ../ImageColorBoolModel/ImageColorBoolDelegate.cpp

HEADERS  += MainWindow.h \
	DlgSettings.h \
	MySetting.h \
	DlgAbbreviation.h \
	Convertor.h \
	TextEdit.h \
	Parser.h \
	Reference.h \
    ../ImageColorBoolModel/ImageColorBoolProxy.h \
    ../ImageColorBoolModel/ImageColorBoolDelegate.h

FORMS    += MainWindow.ui \
	DlgSettings.ui \
	DlgAbbreviation.ui

RESOURCES += \
	Resource.qrc


