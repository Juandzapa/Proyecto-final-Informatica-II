#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QVector>
#include "objetojuego.h"

enum TipoObstaculo { FAROL, CARTEL, SABANA, DRON, PINCHOS, BOMBA, METEORO };

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
    void soltar_desde_arriba(float gravedad_caida, float segundos_en_suelo);
    bool termino() const;
    void set_cuadros(const QVector<QPixmap> &animacion);
    void poner_en_altura(float y);

private:
    TipoObstaculo tipo;
    float vel_x;
    bool golpeado;
    bool superado;

    bool cayendo;
    bool temporal;
    float y_inicial;
    float y_suelo;
    float t_caida;
    float gravedad;
    float tiempo_restante;

    QVector<QPixmap> cuadros;
    unsigned short cuadro_actual;
    float tiempo_cuadro;
};

#endif
