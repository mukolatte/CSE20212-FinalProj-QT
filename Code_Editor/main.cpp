#include "codeeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    codeeditor w;
    w.show();

    return a.exec();
}
