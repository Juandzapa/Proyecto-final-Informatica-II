#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include "menuprincipal.h"
#include "panel.h"
#include "sonido.h"
#include "nivel.h"

class Juego : public QMainWindow
{
    Q_OBJECT

public:
    Juego(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;

private slots:
    void iniciar_nivel(unsigned short numero, NivelDificultad dif);
    void actualizar_juego();
    void ganar_nivel();
    void perder_nivel();

private:
    void terminar_nivel(bool gano);

    QStackedWidget *pantallas;
    MenuPrincipal *menu;
    QWidget *pantalla_juego;
    Panel *panel;
    QGraphicsView *vista;
    Sonido *sonido;
    Nivel *nivel_actual;
    QTimer *reloj;
    int intervalo_ms;
};

#endif // JUEGO_H
