#include "panel.h"
#include <QHBoxLayout>

Panel::Panel(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(36);

    QHBoxLayout *fila = new QHBoxLayout(this);
    fila->setContentsMargins(10, 4, 10, 4);

    etiqueta_vidas = new QLabel("Vidas: 3", this);
    etiqueta_tiempo = new QLabel("Tiempo: 0 s", this);

    barra_progreso = new QProgressBar(this);
    barra_progreso->setRange(0, 100);
    barra_progreso->setValue(0);
    barra_progreso->setFormat("Escape: %p%");

    fila->addWidget(etiqueta_vidas);
    fila->addSpacing(20);
    fila->addWidget(etiqueta_tiempo);
    fila->addSpacing(20);
    fila->addWidget(barra_progreso);
}

void Panel::mostrar_vidas(bool mostrar)
{
    etiqueta_vidas->setVisible(mostrar);
}

void Panel::actualizar(unsigned short vidas, unsigned short porcentaje, unsigned short segundos)
{
    etiqueta_vidas->setText(QString("Vidas: %1").arg(vidas));
    etiqueta_tiempo->setText(QString("Tiempo: %1 s").arg(segundos));
    barra_progreso->setValue(porcentaje);
}
