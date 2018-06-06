#include "guimain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUIMain w;
    w.show();

    return a.exec();
}
