#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QVector>
#include "objetojuego.h"
#include "estados.h"

class Personaje : public ObjetoJuego
{
public:
    Personaje(const QVector<QPixmap> &correr, const QVector<QPixmap> &salto, const QVector<QPixmap> &deslizar, const QPixmap &img_golpe, float linea_suelo, float x_base);

    void saltar();
    void agacharse(bool estado);
    void recibir_golpe(float v0, float k);
    void asustar();
    void perder_vida();
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
    QPixmap imagen_golpe;
    unsigned short cuadro_actual;
    float tiempo_cuadro;
    float tiempo_golpe;
    float tiempo_agachado;
    const QPixmap *imagen_puesta;

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
};

#endif
