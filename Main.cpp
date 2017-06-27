#include "MainWindow.h"
#include <QDir>
#include <QApplication>

// workaround for a bug on Mavericks
// Finder returns / as the working path of an app bundle
// but if the app is run from terminal, the path is correct
// This method calcluates the path of the bundle from the application's path
QString getCurrentPath()
{
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    return dir.absolutePath();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

#ifdef Q_OS_OSX
    QDir::setCurrent(getCurrentPath());
#endif

    BibFixer::MainWindow wnd;
    app.setActiveWindow(&wnd);   // make sure this app is on the top of the desktop
    wnd.resize(1000, 600);
    wnd.show();
    if(argc > 1)
        wnd.openBibFile(argv[1]);

	return app.exec();
}
