#include "objetojuego.h"

ObjetoJuego::ObjetoJuego(const QPixmap &imagen) : QGraphicsPixmapItem(imagen)
{
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

ObjetoJuego::~ObjetoJuego()
{}

bool ObjetoJuego::fuera_de_pantalla() const
{
    bool por_izquierda = x() + pixmap().width() < 0;
    bool por_derecha = x() > ANCHO_ESCENA;
    bool por_abajo = y() > ALTO_ESCENA;
    return por_izquierda || por_derecha || por_abajo;
}
