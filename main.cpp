#include "ForbiddenIslandUI.h"
//#include "ForbiddenIsland.h"
#include "Game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ForbiddenIslandUI w;
    w.show();
    return a.exec();
    //Game();
    //return 0;
}



