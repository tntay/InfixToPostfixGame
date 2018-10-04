#include "view.h"
#include "model.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    Model m;
    View w(m);
    w.show();

    return a.exec();
}
