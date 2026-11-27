#include "nivel2.h"
#include <QVector>

Nivel2::Nivel2(NivelDificultad dif, QObject *parent)
    : Nivel(dif, 40.0f, ":/sprites/fondo2.png", parent)
{

    vista_fija = true;
    jugador->permitir_movimiento(170.0f, ANCHO_ESCENA - 50.0f);
    intervalo_generacion = dificultad.get_intervalo_obstaculos() * 2.2f;
    espera = intervalo_generacion;

    tipos_obstaculo.append(PINCHOS);
    tipos_obstaculo.append(BOMBA);
    tipos_obstaculo.append(METEORO);
    tipos_obstaculo.append(SABANA);

    cuadros_proyectil.append(imagen_o_provisional(":/sprites/proyectil_1.png", 40, 13, QColor(200, 160, 60)));
    cuadros_proyectil.append(imagen_o_provisional(":/sprites/proyectil_2.png", 40, 13, QColor(200, 160, 60)));


    QColor verde(72, 82, 44);
    QVector<QPixmap> apuntar;
    QVector<QPixmap> disparo;
    QVector<QPixmap> recarga;
    for (int i = 1; i <= 2; i++) {
        apuntar.append(imagen_o_provisional(QString(":/sprites/helicoptero_apuntar_%1.png").arg(i), 170, 130, verde));
        disparo.append(imagen_o_provisional(QString(":/sprites/helicoptero_disparo_%1.png").arg(i), 170, 130, verde));
        recarga.append(imagen_o_provisional(QString(":/sprites/helicoptero_recarga_%1.png").arg(i), 170, 130, verde));
    }

    papa = new PapaCerdito(apuntar.at(0), -20.0f, carril_bajo, carril_alto, 40.0f, 4.0f,
                           dificultad.get_limite_saltos(), dificultad.get_intervalo_disparo());
    papa->set_cuadros(apuntar, disparo, recarga, 0.06f);

    papa->set_boca_arma(0.82f, 0.60f);
    addItem(papa);
}

void Nivel2::actualizar_elementos(float dt)
{
    actualizar_obstaculos(dt);

    for (int i = 0; i < proyectiles.size(); i++) {
        proyectiles[i]->actualizar(dt);
    }

    Proyectil *nuevo = papa->disparar(cuadros_proyectil, dificultad.get_velocidad_proyectil());
    if (nuevo != nullptr) {
        addItem(nuevo);
        proyectiles.append(nuevo);
        emit hubo_disparo();
    }
}

void Nivel2::avisar_al_agente()
{
    papa->percibir(*jugador);
    papa->aprender();
    papa->razonar();
}

void Nivel2::revisar_colisiones()
{

    unsigned short superados = revisar_obstaculos();
    for (unsigned short i = 0; i < superados; i++) {
        avisar_al_agente();
    }

    for (int i = proyectiles.size() - 1; i >= 0; i--) {
        Proyectil *proyectil = proyectiles[i];
        bool borrar = false;

        if (jugador->collidesWithItem(proyectil)) {
            jugador->asustar();
            jugador->perder_vida();
            jugador->recibir_golpe(90.0f, dificultad.get_k_friccion());
            emit hubo_golpe();
            avisar_al_agente();
            borrar = true;
        }
        else if (!proyectil->fue_evaluado() && proyectil->x() > jugador->x() + jugador->pixmap().width()) {

            proyectil->marcar_evaluado();
            avisar_al_agente();
        }

        if (proyectil->fuera_de_pantalla()) {
            borrar = true;
        }

        if (borrar) {
            removeItem(proyectil);
            delete proyectil;
            proyectiles.removeAt(i);
        }
    }
}
