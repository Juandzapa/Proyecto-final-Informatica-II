#include "obstaculo.h"

Obstaculo::Obstaculo(const QPixmap &imagen, TipoObstaculo tipo, float x, float linea_suelo, float velocidad) : ObjetoJuego(imagen),
      tipo(tipo), vel_x(velocidad), golpeado(false), superado(false)
{
    setPos(x, linea_suelo - pixmap().height());
    setZValue(1);
}

void Obstaculo::actualizar(float dt) {
    setX(x() - vel_x * dt);
}

float Obstaculo::get_empuje() const {
    switch (tipo) {
    case FAROL:
        return 160.0f;
    case CARTEL:
        return 120.0f;
    case SABANA:
        return 200.0f;
    }
    return 120.0f;
}

bool Obstaculo::fue_golpeado() const {
    return golpeado;
}

void Obstaculo::marcar_golpeado() {
    golpeado = true;
}

bool Obstaculo::fue_superado() const {
    return superado;
}

void Obstaculo::marcar_superado() {
    superado = true;
}
