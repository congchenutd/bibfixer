#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

    BibFixer::MainWindow wnd;
    wnd.showMaximized();
    if(argc > 1)
        wnd.open(argv[1]);

	return app.exec();
}
