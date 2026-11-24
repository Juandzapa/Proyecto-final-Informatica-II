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
    float k_friccion;           // que tan rapido se frena el arrastre (1/s)
    float limite_saltos;        // porcentaje de saltos para que Papa Cerdito apunte arriba
    float intervalo_disparo;    // segundos entre disparos (nivel 2)
    float velocidad_proyectil;  // px/s
    float intervalo_obstaculos; // segundos entre obstaculos (nivel 1)
};

#endif // DIFICULTAD_H
