#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>

// Barra de arriba con las vidas, el tiempo y el progreso del nivel

class Panel : public QWidget
{
    Q_OBJECT

public:
    Panel(QWidget *parent = nullptr);

    void mostrar_vidas(bool mostrar);

public slots:
    void actualizar(unsigned short vidas, unsigned short porcentaje, unsigned short segundos);

private:
    QLabel *etiqueta_vidas;
    QLabel *etiqueta_tiempo;
    QProgressBar *barra_progreso;
};

#endif // PANEL_H
