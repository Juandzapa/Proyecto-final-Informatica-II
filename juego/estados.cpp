#include "estados.h"

Estados::Estados() : tiempo_asustado(0.0f), tiempo_cansado(0.0f), agachadas(0), tiempo_agachadas(0.0f)
{}

void Estados::asustar(){
    tiempo_asustado = 3.0f;
}

void Estados::registrar_agachada(){
    if (agachadas == 0 || tiempo_agachadas > 2.0f) {
        agachadas = 0;
        tiempo_agachadas = 0.0f;
    }
    agachadas++;

    if (agachadas > 3) {
        tiempo_cansado = 5.0f;
        agachadas = 0;
    }
}

void Estados::actualizar(float dt){
    if (tiempo_asustado > 0.0f) {
        tiempo_asustado -= dt;
        if (tiempo_asustado < 0.0f) {
            tiempo_asustado = 0.0f;
        }
    }

    if (tiempo_cansado > 0.0f) {
        tiempo_cansado -= dt;
        if (tiempo_cansado < 0.0f) {
            tiempo_cansado = 0.0f;
        }
    }

    tiempo_agachadas += dt;
}

float Estados::factor_salto() const{
    if (esta_asustado()) {
        return 1.095f;
    }
    return 1.0f;
}

float Estados::factor_gravedad() const{
    if (esta_cansado()) {
        return 1.3f;
    }
    return 1.0f;
}

float Estados::factor_friccion() const{
    if (esta_asustado()) {
        return 1.5f;
    }
    return 1.0f;
}

bool Estados::esta_asustado() const{
    return tiempo_asustado > 0.0f;
}

bool Estados::esta_cansado() const{
    return tiempo_cansado > 0.0f;
}
