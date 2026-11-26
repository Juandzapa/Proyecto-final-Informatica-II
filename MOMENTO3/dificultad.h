#ifndef DIFICULTAD_H
#define DIFICULTAD_H

enum NivelDificultad { FACIL, NORMAL, DIFICIL };

class Dificultad
{
public:
    Dificultad(NivelDificultad nivel);

    float get_k_friccion() const;
    float get_limite_saltos() const;
    float get_intervalo_disparo() const;
    float get_velocidad_proyectil() const;
    float get_intervalo_obstaculos() const;

private:
    float k_friccion;
    float limite_saltos;
    float intervalo_disparo;
    float velocidad_proyectil;
    float intervalo_obstaculos;
};

#endif // DIFICULTAD_H
