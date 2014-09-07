#include <QCoreApplication>
#include "modemrestarter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ModemRestarter restarter(NULL);

    return a.exec();
}
