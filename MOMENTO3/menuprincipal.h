#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include "dificultad.h"

class MenuPrincipal : public QWidget
{
    Q_OBJECT

public:
    MenuPrincipal(QWidget *parent = nullptr);

signals:
    void jugar(unsigned short nivel, NivelDificultad dif);

private slots:
    void boton_nivel1_presionado();
    void boton_nivel2_presionado();

private:
    NivelDificultad dificultad_escogida() const;

    QPushButton *boton_nivel1;
    QPushButton *boton_nivel2;
    QComboBox *selector_dificultad;
};

#endif // MENUPRINCIPAL_H
