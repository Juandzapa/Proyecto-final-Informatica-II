#include "juego.h"
#include "nivel1.h"
#include "nivel2.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <stdexcept>

Juego::Juego(QWidget *parent)
    : QMainWindow(parent),
      sonido(nullptr),
      nivel_actual(nullptr),
      intervalo_ms(16)
{
    setWindowTitle("Huyendo del reclutamiento de Papá Cerdito");

    pantallas = new QStackedWidget(this);
    menu = new MenuPrincipal(pantallas);


    pantalla_juego = new QWidget(pantallas);
    QVBoxLayout *columna = new QVBoxLayout(pantalla_juego);
    columna->setContentsMargins(0, 0, 0, 0);
    columna->setSpacing(0);

    panel = new Panel(pantalla_juego);

    vista = new QGraphicsView(pantalla_juego);
    vista->setFixedSize(ANCHO_ESCENA, ALTO_ESCENA);
    vista->setFrameShape(QFrame::NoFrame);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);

    columna->addWidget(panel);
    columna->addWidget(vista);

    pantallas->addWidget(menu);
    pantallas->addWidget(pantalla_juego);
    setCentralWidget(pantallas);
    setFixedSize(ANCHO_ESCENA, ALTO_ESCENA + panel->height());
    setFocusPolicy(Qt::StrongFocus);

    try {
        sonido = new Sonido(this);
    }
    catch (const std::runtime_error &error) {
        sonido = nullptr;
        QMessageBox::warning(this, "Sonido", QString("%1. Se jugará sin sonido.").arg(error.what()));
    }

    reloj = new QTimer(this);
    connect(reloj, &QTimer::timeout, this, &Juego::actualizar_juego);
    connect(menu, &MenuPrincipal::jugar, this, &Juego::iniciar_nivel);
}

void Juego::iniciar_nivel(unsigned short numero, NivelDificultad dif)
{
    if (numero == 1) {
        nivel_actual = new Nivel1(this);
    }
    else {
        nivel_actual = new Nivel2(dif, this);
    }

    connect(nivel_actual, &Nivel::nivel_ganado, this, &Juego::ganar_nivel);
    connect(nivel_actual, &Nivel::nivel_perdido, this, &Juego::perder_nivel);
    connect(nivel_actual, &Nivel::cambiaron_datos, panel, &Panel::actualizar);

    if (sonido != nullptr) {
        connect(nivel_actual, &Nivel::hubo_golpe, sonido, &Sonido::reproducir_golpe);
        connect(nivel_actual, &Nivel::hubo_disparo, sonido, &Sonido::reproducir_disparo);
        sonido->iniciar_musica();
    }


    panel->mostrar_vidas(numero == 2);

    vista->setScene(nivel_actual);
    pantallas->setCurrentWidget(pantalla_juego);
    setFocus();
    reloj->start(intervalo_ms);
}

void Juego::actualizar_juego()
{
    if (nivel_actual != nullptr) {
        float dt = intervalo_ms / 1000.0f;
        nivel_actual->actualizar(dt);
    }
}

void Juego::ganar_nivel()
{
    terminar_nivel(true);
}

void Juego::perder_nivel()
{
    terminar_nivel(false);
}

void Juego::terminar_nivel(bool gano)
{
    if (nivel_actual == nullptr) {
        return;
    }


    reloj->stop();
    if (sonido != nullptr) {
        sonido->detener_musica();
    }
    vista->setScene(nullptr);


    nivel_actual->deleteLater();
    nivel_actual = nullptr;

    pantallas->setCurrentWidget(menu);

    if (gano) {
        QMessageBox::information(this, "Nivel terminado", "¡Escapaste de Papá Cerdito!");
    }
    else {
        QMessageBox::information(this, "Nivel terminado", "Papá Cerdito te atrapó.");
    }
}

void Juego::keyPressEvent(QKeyEvent *evento)
{
    if (nivel_actual == nullptr || evento->isAutoRepeat()) {
        QMainWindow::keyPressEvent(evento);
        return;
    }
    nivel_actual->procesar_tecla(evento->key(), true);
}

void Juego::keyReleaseEvent(QKeyEvent *evento)
{
    if (nivel_actual == nullptr || evento->isAutoRepeat()) {
        QMainWindow::keyReleaseEvent(evento);
        return;
    }
    nivel_actual->procesar_tecla(evento->key(), false);
}
