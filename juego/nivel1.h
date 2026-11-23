#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"

class Nivel1 : public Nivel
{
    Q_OBJECT

public:
    Nivel1(QObject *parent = nullptr);

protected:
    void actualizar_elementos(float dt) override;
    void revisar_colisiones() override;
};

#endif
