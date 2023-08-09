#include "SolitaireUI.h"
#include "Solitaire.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SolitaireUI w;
    w.show();
    return a.exec();
}
