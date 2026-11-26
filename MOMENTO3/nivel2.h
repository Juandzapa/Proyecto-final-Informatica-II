#ifndef NIVEL2_H
#define NIVEL2_H

#include <QList>
#include <QVector>
#include "nivel.h"
#include "proyectil.h"



class Nivel2 : public Nivel
{
    Q_OBJECT

public:
    Nivel2(NivelDificultad dif, QObject *parent = nullptr);

protected:
    void actualizar_elementos(float dt) override;
    void revisar_colisiones() override;

private:
    void avisar_al_agente();

    QList<Proyectil *> proyectiles;
    QVector<QPixmap> cuadros_proyectil;
};

#endif // NIVEL2_H
