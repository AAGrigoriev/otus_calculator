#include "calulatorwidget.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalculatorWidget w;
    w.show();
    return a.exec();
}
