#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "objetojuego.h"

enum TipoObstaculo { FAROL, CARTEL, SABANA };

class Obstaculo : public ObjetoJuego
{
public:
    Obstaculo(const QPixmap &imagen, TipoObstaculo tipo, float x, float linea_suelo, float velocidad);

    void actualizar(float dt) override;
    float get_empuje() const;
    bool fue_golpeado() const;
    void marcar_golpeado();
    bool fue_superado() const;
    void marcar_superado();

private:
    TipoObstaculo tipo;
    float vel_x;
    bool golpeado;
    bool superado; 
};

#endif
