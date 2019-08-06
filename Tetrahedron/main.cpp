#include "Tetrahedron.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tetrahedron w;
    w.show();

    return a.exec();
}
