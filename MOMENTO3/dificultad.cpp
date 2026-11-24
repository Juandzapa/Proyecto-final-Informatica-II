#include "dificultad.h"

Dificultad::Dificultad(NivelDificultad nivel)
{
    switch (nivel) {
    case FACIL:
        k_friccion = 3.0f;
        limite_saltos = 0.8f;
        intervalo_disparo = 2.6f;
        velocidad_proyectil = 240.0f;
        intervalo_obstaculos = 2.2f;
        break;
    case NORMAL:
        // el nivel 1 siempre usa estos valores
        k_friccion = 1.8f;
        limite_saltos = 0.8f;
        intervalo_disparo = 2.0f;
        velocidad_proyectil = 290.0f;
        intervalo_obstaculos = 1.5f;
        break;
    case DIFICIL:
        // el arrastre dura mas y Papa Cerdito aprende con menos saltos
        k_friccion = 1.4f;
        limite_saltos = 0.6f;
        intervalo_disparo = 1.5f;
        velocidad_proyectil = 350.0f;
        intervalo_obstaculos = 1.3f;
        break;
    }
}

float Dificultad::get_k_friccion() const
{
    return k_friccion;
}

float Dificultad::get_limite_saltos() const
{
    return limite_saltos;
}

float Dificultad::get_intervalo_disparo() const
{
    return intervalo_disparo;
}

float Dificultad::get_velocidad_proyectil() const
{
    return velocidad_proyectil;
}

float Dificultad::get_intervalo_obstaculos() const
{
    return intervalo_obstaculos;
}
