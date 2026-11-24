#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QVector>
#include <QGraphicsColorizeEffect>
#include "objetojuego.h"
#include "estados.h"

class Personaje : public ObjetoJuego
{
public:
    Personaje(const QVector<QPixmap> &correr, const QVector<QPixmap> &salto, const QVector<QPixmap> &deslizar, const QVector<QPixmap> &agachar, const QVector<QPixmap> &quieto, const QPixmap &img_golpe, float linea_suelo, float x_base);

    void saltar();
    void agacharse(bool estado);
    void recibir_golpe(float v0, float k);
    void asustar();
    void perder_vida();
    void permitir_movimiento(float minimo, float maximo);
    void mover(short sentido);
    void actualizar(float dt) override;
    bool esta_en_suelo() const;
    unsigned short get_vidas() const;

private:
    void actualizar_salto(float dt);
    void actualizar_arrastre(float dt);
    void actualizar_imagen(float dt);

    QVector<QPixmap> cuadros_correr;
    QVector<QPixmap> cuadros_salto;
    QVector<QPixmap> cuadros_deslizar;
    QVector<QPixmap> cuadros_agachado;
    QVector<QPixmap> cuadros_quieto;
    unsigned short cuadro_quieto;
    float tiempo_quieto;
    QPixmap imagen_golpe;
    unsigned short cuadro_actual;
    float tiempo_cuadro;
    float tiempo_golpe;
    float tiempo_agachado;
    const QPixmap *imagen_puesta;
    bool mirando_izquierda;
    bool izquierda_puesta;

    unsigned short vidas;
    Estados estados;

    float linea_suelo;
    float altura;
    float t_salto;
    float v0_salto;
    float gravedad;
    float v0_actual;
    float g_actual;
    bool en_suelo;
    bool agachado;

    bool arrastrando;
    float x_inicio_arrastre;
    float v0_arrastre;
    float k_arrastre;
    float t_arrastre;
    float x_base;
    float velocidad_regreso;

    bool movimiento_libre;
    short direccion;
    float velocidad_lateral;
    float x_minima;
    float x_maxima;

    QGraphicsColorizeEffect *efecto_cansado;
    bool mostrando_cansado;
};

#endif
