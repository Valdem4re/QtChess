#include <QCoreApplication>
#include "qtchessserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtChessServer server;
    return a.exec();
}
