#include "papacerdito.h"
#include <cmath>

PapaCerdito::PapaCerdito(const QPixmap &imagen, float x, float carril_bajo, float carril_alto,
                         float amplitud, float omega, float limite_saltos, float intervalo_disparo)
    : ObjetoJuego(imagen),
      carril_bajo(carril_bajo),
      carril_alto(carril_alto),
      y_centro(carril_bajo),
      y_objetivo(carril_bajo),
      velocidad_carril(120.0f),
      amplitud(amplitud),
      omega(omega),
      fase(0.0f),
      t(0.0f),
      desfase_boca(0.0f),
      indice_historial(0),
      saltos_en_historial(0),
      registros(0),
      limite_saltos(limite_saltos),
      jugador_en_aire(false),
      apuntar_arriba(false),
      tiempo_disparo(0.0f),
      intervalo_disparo(intervalo_disparo),
      animado(false),
      cuadro_actual(0),
      tiempo_cuadro(0.0f),
      tiempo_desde_disparo(1.0f),
      imagen_puesta(nullptr),
      boca_x(1.0f),
      boca_y(0.5f),
      tiempo_por_cuadro(0.06f)
{
    for (unsigned short i = 0; i < TAM_HISTORIAL; i++) {
        historial[i] = false;
    }
    setPos(x, y_centro - pixmap().height() / 2.0f);
    setZValue(1);
}

void PapaCerdito::percibir(const Personaje &jugador)
{
    jugador_en_aire = !jugador.esta_en_suelo();
}

void PapaCerdito::aprender()
{
    // si el historial esta lleno se va a sobrescribir la jugada mas vieja,
    // entonces primero se descuenta si esa era un salto
    if (registros == TAM_HISTORIAL) {
        if (historial[indice_historial]) {
            saltos_en_historial--;
        }
    }
    else {
        registros++;
    }

    historial[indice_historial] = jugador_en_aire;
    if (jugador_en_aire) {
        saltos_en_historial++;
    }

    indice_historial = (indice_historial + 1) % TAM_HISTORIAL;
}

void PapaCerdito::razonar()
{
    // solo usa lo aprendido cuando ya tiene las 5 jugadas
    apuntar_arriba = false;
    if (registros == TAM_HISTORIAL) {
        float proporcion = static_cast<float>(saltos_en_historial) / TAM_HISTORIAL;
        if (proporcion >= limite_saltos) {
            apuntar_arriba = true;
        }
    }

    if (jugador_en_aire || apuntar_arriba) {
        y_objetivo = carril_alto;
    }
    else {
        y_objetivo = carril_bajo;
    }
}

void PapaCerdito::actuar(float dt)
{
    t += dt;

    // se acerca al carril que escogio
    if (y_centro < y_objetivo) {
        y_centro += velocidad_carril * dt;
        if (y_centro > y_objetivo) {
            y_centro = y_objetivo;
        }
    }
    else if (y_centro > y_objetivo) {
        y_centro -= velocidad_carril * dt;
        if (y_centro < y_objetivo) {
            y_centro = y_objetivo;
        }
    }

    // movimiento armonico simple alrededor del carril
    float y = y_centro + amplitud * std::sin(omega * t + fase);
    setY(y - pixmap().height() / 2.0f - desfase_boca);

    tiempo_disparo += dt;
}

Proyectil *PapaCerdito::disparar(const QVector<QPixmap> &cuadros_bala, float velocidad)
{
    if (tiempo_disparo < intervalo_disparo) {
        return nullptr;
    }

    // espera a que el MAS pase por el centro del carril escogido (sin = 0),
    // asi la bala sale derecho a la altura correcta
    float desplazamiento = amplitud * std::sin(omega * t + fase);
    if (y_centro != y_objetivo || std::fabs(desplazamiento) > 4.0f) {
        return nullptr;
    }
    tiempo_disparo = 0.0f;
    tiempo_desde_disparo = 0.0f;

    // la bala sale derecho desde la boca del arma, que esta sobre el carril escogido
    float x_inicio = x() + boca_x * pixmap().width();
    float y_inicio = y() + boca_y * pixmap().height() - cuadros_bala.at(0).height() / 2.0f;
    return new Proyectil(cuadros_bala, x_inicio, y_inicio, velocidad);
}

void PapaCerdito::acelerar_disparos()
{
    intervalo_disparo = intervalo_disparo / 2.0f;
}

void PapaCerdito::actualizar(float dt)
{
    actuar(dt);
    actualizar_imagen(dt);
}

void PapaCerdito::set_cuadros(const QVector<QPixmap> &apuntar, const QVector<QPixmap> &disparo, const QVector<QPixmap> &recarga, float segundos_por_cuadro)
{
    cuadros_apuntar = apuntar;
    cuadros_disparo = disparo;
    cuadros_recarga = recarga;
    tiempo_por_cuadro = segundos_por_cuadro;
    animado = true;
}

void PapaCerdito::set_boca_arma(float fraccion_x, float fraccion_y)
{
    boca_x = fraccion_x;
    boca_y = fraccion_y;
    // se corre el dibujo para que la boca (y no el centro) quede en y_centro
    desfase_boca = (boca_y - 0.5f) * pixmap().height();
}

void PapaCerdito::actualizar_imagen(float dt)
{
    if (!animado) {
        return;
    }

    tiempo_desde_disparo += dt;

    // segun lo que este haciendo se escoge la lista de cuadros
    const QVector<QPixmap> *cuadros = &cuadros_apuntar;
    if (tiempo_desde_disparo < 0.25f) {
        cuadros = &cuadros_disparo;
    }
    else if (tiempo_desde_disparo < 0.75f) {
        cuadros = &cuadros_recarga;
    }

    // en todos los estados se alternan los cuadros para que giren las aspas
    tiempo_cuadro += dt;
    if (tiempo_cuadro >= tiempo_por_cuadro) {
        tiempo_cuadro = 0.0f;
        cuadro_actual = static_cast<unsigned short>((cuadro_actual + 1) % cuadros->size());
    }
    if (cuadro_actual >= cuadros->size()) {
        cuadro_actual = 0;
    }

    const QPixmap *nueva = &cuadros->at(cuadro_actual);
    if (nueva != imagen_puesta) {
        setPixmap(*nueva);
        imagen_puesta = nueva;
    }
}

