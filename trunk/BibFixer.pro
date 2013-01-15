QT += core sql network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BibFixer
TEMPLATE = app

INCLUDEPATH +=	../EnglishName \
				../MySetting \
				../ImageColorBoolModel

include(qtsingleapplication/src/qtsingleapplication.pri)

win32 {
	RC_FILE = Resource.rc
}
mac {
	ICON = Images/Fix.icns
}

SOURCES +=\
		MainWindow.cpp \
	DlgSettings.cpp \
	Convertor.cpp \
	TextEdit.cpp \
	Parser.cpp \
	Reference.cpp \
    ../ImageColorBoolModel/ImageColorBoolProxy.cpp \
    ../ImageColorBoolModel/ImageColorBoolDelegate.cpp \
    WidgetAbbreviation.cpp \
    WidgetValidFields.cpp \
    Main.cpp \
    Commands.cpp \
    ../EnglishName/EnglishName.cpp \
    WidgetKeyGen.cpp \
    KeyGenerator.cpp

HEADERS  += MainWindow.h \
	DlgSettings.h \
	Convertor.h \
	TextEdit.h \
	Parser.h \
	Reference.h \
    ../ImageColorBoolModel/ImageColorBoolProxy.h \
    ../ImageColorBoolModel/ImageColorBoolDelegate.h \
    WidgetAbbreviation.h \
    WidgetValidFields.h \
    Commands.h \
    ../EnglishName/EnglishName.h \
    ../MySetting/MySetting.h \
    WidgetKeyGen.h \
    KeyGenerator.h

FORMS    += MainWindow.ui \
	DlgSettings.ui \
    WidgetAbbreviation.ui \
    WidgetValidFields.ui \
    WidgetKeyGen.ui

RESOURCES += \
	Resource.qrc


