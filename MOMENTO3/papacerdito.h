#ifndef PAPACERDITO_H
#define PAPACERDITO_H

#include <QVector>
#include "objetojuego.h"
#include "personaje.h"
#include "proyectil.h"

const unsigned short TAM_HISTORIAL = 5;

// Agente inteligente del juego.
// Percepcion: mira si el jugador esta en el aire o en el suelo.
// Razonamiento: decide a que carril (alto o bajo) se mueve y dispara.
// Accion: se mueve con MAS y dispara proyectiles de lodo.
// Aprendizaje: recuerda las ultimas 5 jugadas; si el jugador salta mucho,
// empieza a disparar arriba aunque el jugador este en el suelo.

class PapaCerdito : public ObjetoJuego
{
public:
    PapaCerdito(const QPixmap &imagen, float x, float carril_bajo, float carril_alto,
                float amplitud, float omega, float limite_saltos, float intervalo_disparo);

    void percibir(const Personaje &jugador);
    void razonar();
    void actuar(float dt);
    void aprender();
    Proyectil *disparar(const QVector<QPixmap> &cuadros_bala, float velocidad);
    void acelerar_disparos();
    void set_cuadros(const QVector<QPixmap> &apuntar, const QVector<QPixmap> &disparo, const QVector<QPixmap> &recarga, float segundos_por_cuadro);
    void set_boca_arma(float fraccion_x, float fraccion_y);
    void actualizar(float dt) override;

private:
    void actualizar_imagen(float dt);

    // movimiento: y(t) = y_centro + A*sin(w*t + fase)
    float carril_bajo;
    float carril_alto;
    float y_centro;
    float y_objetivo;
    float velocidad_carril;
    float amplitud;
    float omega;
    float fase;
    float t;
    float desfase_boca; // para que la boca del arma quede justo en el carril

    // memoria del agente (cola circular)
    bool historial[TAM_HISTORIAL];
    unsigned short indice_historial;
    unsigned short saltos_en_historial;
    unsigned short registros;
    float limite_saltos;

    bool jugador_en_aire;
    bool apuntar_arriba;

    float tiempo_disparo;
    float intervalo_disparo;

    // animacion del helicoptero (solo nivel 2)
    bool animado;
    // en cada estado se alternan dos cuadros para que giren las aspas
    QVector<QPixmap> cuadros_apuntar;
    QVector<QPixmap> cuadros_disparo;
    QVector<QPixmap> cuadros_recarga;
    unsigned short cuadro_actual;
    float tiempo_cuadro;
    float tiempo_desde_disparo;
    const QPixmap *imagen_puesta;

    // donde queda la boca del arma dentro de la imagen (0 a 1)
    float boca_x;
    float boca_y;

    float tiempo_por_cuadro; // cada cuanto cambia de cuadro
};

#endif // PAPACERDITO_H
