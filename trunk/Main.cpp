#include "MainWindow.h"
#include <QtSingleApplication>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    if(app.sendMessage(argc > 1 ? argv[1] : QString()))
        return 0;

    BibFixer::MainWindow wnd;
    app.setActivationWindow(&wnd);
    wnd.resize(1000, 600);
    wnd.show();
    if(argc > 1)
        wnd.openBibFile(argv[1]);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &wnd, SLOT(openBibFile(const QString&)));

	return app.exec();
}
