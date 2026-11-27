#include "obstaculo.h"

Obstaculo::Obstaculo(const QPixmap &imagen, TipoObstaculo tipo, float x, float linea_suelo, float velocidad) : ObjetoJuego(imagen),
      tipo(tipo), vel_x(velocidad), golpeado(false), superado(false), cayendo(false), temporal(false), y_inicial(0.0f),
      y_suelo(linea_suelo - pixmap().height()), t_caida(0.0f), gravedad(0.0f), tiempo_restante(0.0f), cuadro_actual(0), tiempo_cuadro(0.0f)
{
    setPos(x, linea_suelo - pixmap().height());
    setZValue(1);
}

void Obstaculo::actualizar(float dt) {
    if (cayendo) {
        t_caida += dt;
        float y = y_inicial + 0.5f * gravedad * t_caida * t_caida;
        if (y >= y_suelo) {
            y = y_suelo;
            cayendo = false;
        }
        setY(y);
    }
    else if (temporal) {
        tiempo_restante -= dt;
    }
    else {
        setX(x() - vel_x * dt);
    }

    if (cuadros.size() > 1) {
        tiempo_cuadro += dt;
        if (tiempo_cuadro >= 0.06f) {
            tiempo_cuadro = 0.0f;
            cuadro_actual = static_cast<unsigned short>((cuadro_actual + 1) % cuadros.size());
            setPixmap(cuadros.at(cuadro_actual));
        }
    }
}

void Obstaculo::soltar_desde_arriba(float gravedad_caida, float segundos_en_suelo) {
    y_inicial = -pixmap().height();
    setY(y_inicial);
    t_caida = 0.0f;
    gravedad = gravedad_caida;
    cayendo = true;
    temporal = true;
    tiempo_restante = segundos_en_suelo;
}

bool Obstaculo::termino() const {
    return temporal && !cayendo && tiempo_restante <= 0.0f;
}

void Obstaculo::set_cuadros(const QVector<QPixmap> &animacion) {
    cuadros = animacion;
}

void Obstaculo::poner_en_altura(float y) {
    setY(y);
    y_suelo = y;
}

float Obstaculo::get_empuje() const {
    switch (tipo) {
    case FAROL:
        return 160.0f;
    case CARTEL:
        return 120.0f;
    case SABANA:
        return 200.0f;
    case DRON:
        return 140.0f;
    case PINCHOS:
        return 180.0f;
    case BOMBA:
        return 220.0f;
    case METEORO:
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
