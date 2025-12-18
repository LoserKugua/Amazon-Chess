#include <QApplication>
#include "GUI/ChessWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessWindow w;
    w.show();
    return a.exec();
}