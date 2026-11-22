#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QVector>
#include "objetojuego.h"

class Proyectil : public ObjetoJuego
{
public:
    Proyectil(const QVector<QPixmap> &cuadros, float x, float y, float velocidad);

    void actualizar(float dt) override;
    bool fue_evaluado() const;
    void marcar_evaluado();

private:
    QVector<QPixmap> cuadros; 
    unsigned short cuadro_actual;
    float tiempo_cuadro;
    float vel_x;
    bool evaluado;
};

#endif // PROYECTIL_H
