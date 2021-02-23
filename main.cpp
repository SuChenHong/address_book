#include "logdlg.h"
#include "basicWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logdlg w;
    w.show();
    return a.exec();
}
