#include <QApplication>
#include "juego.h"

int main(int argc, char *argv[])
{
    QApplication aplicacion(argc, argv);

    Juego juego;
    juego.show();

    return aplicacion.exec();
}
