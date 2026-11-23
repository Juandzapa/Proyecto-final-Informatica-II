#include "nivel1.h"
#include <QVector>

Nivel1::Nivel1(QObject *parent) : Nivel(NORMAL, 35.0f, ":/sprites/fondo1.png", parent)
{
    intervalo_generacion = dificultad.get_intervalo_obstaculos();
    espera = intervalo_generacion;

    QVector<QPixmap> caminar;
    for (int i = 1; i <= 3; i++) {
        caminar.append(imagen_o_provisional(QString(":/sprites/papa_caminar_%1.png").arg(i), 43, 70, QColor(200, 90, 120)));
    }
    papa = new PapaCerdito(caminar.at(0), 20.0f, linea_suelo - 35.0f, linea_suelo - 35.0f, 6.0f, 10.0f, dificultad.get_limite_saltos(), 1000.0f);
    papa->set_cuadros(caminar, caminar, caminar, 0.15f);
    addItem(papa);
}

void Nivel1::actualizar_elementos(float dt)
{
    actualizar_obstaculos(dt);
}

void Nivel1::revisar_colisiones()
{
    revisar_obstaculos();
}
