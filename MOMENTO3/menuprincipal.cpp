#include "menuprincipal.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPixmap>
#include <QHBoxLayout>

MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(
        "MenuPrincipal { background-color: #2e3326; }"
        "QLabel { color: #e8e0cc; }"
        "QPushButton { background-color: #5a5f3c; color: #f0e8d0; border: 2px solid #8c2f2a;"
        "              border-radius: 6px; padding: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6e7449; }"
        "QComboBox { background-color: #e8e0cc; padding: 4px; }");
    setAttribute(Qt::WA_StyledBackground, true);

    QHBoxLayout *fila = new QHBoxLayout(this);
    QLabel *imagen = new QLabel(this);
    QPixmap papa(":/sprites/menu_papa.png");
    if (!papa.isNull()) {
        imagen->setPixmap(papa.scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    fila->addWidget(imagen);

    QVBoxLayout *columna = new QVBoxLayout();
    columna->setAlignment(Qt::AlignCenter);
    fila->addLayout(columna);

    QLabel *titulo = new QLabel("Huyendo del reclutamiento\nde Papá Cerdito", this);
    QFont fuente_titulo = titulo->font();
    fuente_titulo.setPointSize(20);
    fuente_titulo.setBold(true);
    titulo->setFont(fuente_titulo);
    titulo->setStyleSheet("color: #c8463a;");
    titulo->setAlignment(Qt::AlignCenter);

    selector_dificultad = new QComboBox(this);
    selector_dificultad->addItem("Fácil");
    selector_dificultad->addItem("Normal");
    selector_dificultad->addItem("Difícil");
    selector_dificultad->setCurrentIndex(DIFICIL);

    boton_nivel1 = new QPushButton("Nivel 1: La huida", this);
    boton_nivel2 = new QPushButton("Nivel 2: El ataque", this);
    boton_nivel1->setMinimumWidth(260);
    boton_nivel2->setMinimumWidth(260);

    QLabel *controles = new QLabel("W: saltar     S: deslizarse     A / D: moverse (nivel 2)", this);
    controles->setAlignment(Qt::AlignCenter);

    columna->addWidget(titulo);
    columna->addSpacing(20);
    columna->addWidget(boton_nivel1);
    columna->addSpacing(10);
    columna->addWidget(new QLabel("Dificultad del nivel 2:", this));
    columna->addWidget(selector_dificultad);
    columna->addWidget(boton_nivel2);
    columna->addSpacing(20);
    columna->addWidget(controles);

    connect(boton_nivel1, &QPushButton::clicked, this, &MenuPrincipal::boton_nivel1_presionado);
    connect(boton_nivel2, &QPushButton::clicked, this, &MenuPrincipal::boton_nivel2_presionado);
}

NivelDificultad MenuPrincipal::dificultad_escogida() const
{

    return static_cast<NivelDificultad>(selector_dificultad->currentIndex());
}

void MenuPrincipal::boton_nivel1_presionado()
{

    emit jugar(1, NORMAL);
}

void MenuPrincipal::boton_nivel2_presionado()
{
    emit jugar(2, dificultad_escogida());
}
