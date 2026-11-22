#include "proyectil.h"

Proyectil::Proyectil(const QVector<QPixmap> &cuadros, float x, float y, float velocidad) : ObjetoJuego(cuadros.at(0)),
      cuadros(cuadros), cuadro_actual(0), tiempo_cuadro(0.0f), vel_x(velocidad), evaluado(false)
{
    setPos(x, y);
    setZValue(2);
}

void Proyectil::actualizar(float dt){
    setX(x() + vel_x * dt);

    tiempo_cuadro += dt;
    if (tiempo_cuadro >= 0.05f) {
        tiempo_cuadro = 0.0f;
        cuadro_actual = static_cast<unsigned short>((cuadro_actual + 1) % cuadros.size());
        setPixmap(cuadros.at(cuadro_actual));
    }
}

bool Proyectil::fue_evaluado() const{
    return evaluado;
}

void Proyectil::marcar_evaluado(){
    evaluado = true;
}
