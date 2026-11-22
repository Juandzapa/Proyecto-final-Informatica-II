#ifndef OBJETOJUEGO_H
#define OBJETOJUEGO_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

const int ANCHO_ESCENA = 800;
const int ALTO_ESCENA = 450;

class ObjetoJuego : public QGraphicsPixmapItem
{
public:
    ObjetoJuego(const QPixmap &imagen);
    virtual ~ObjetoJuego();

    virtual void actualizar(float dt) = 0;
    bool fuera_de_pantalla() const;
};

#endif 
