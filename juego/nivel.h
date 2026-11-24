#ifndef NIVEL_H
#define NIVEL_H
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QString>
#include <QColor>
#include "dificultad.h"
#include "personaje.h"
#include "papacerdito.h"
#include "obstaculo.h"
#include <QList>
#include <QVector>

class Nivel : public QGraphicsScene
{
    Q_OBJECT

public:
    Nivel(NivelDificultad dif, float duracion, const QString &ruta_fondo, QObject *parent = nullptr);

    void actualizar(float dt);
    void procesar_tecla(int tecla, bool presionada);

signals:
    void nivel_ganado();
    void nivel_perdido();
    void cambiaron_datos(unsigned short vidas, unsigned short porcentaje, unsigned short segundos);
    void hubo_golpe();
    void hubo_disparo();

protected:
    virtual void actualizar_elementos(float dt) = 0;
    virtual void revisar_colisiones() = 0;

    QPixmap cargar_imagen(const QString &ruta);
    QPixmap imagen_o_provisional(const QString &ruta, int ancho, int alto, const QColor &color);

    void actualizar_obstaculos(float dt);
    unsigned short revisar_obstaculos();

    Personaje *jugador;
    PapaCerdito *papa;
    Dificultad dificultad;
    float linea_suelo;
    float carril_bajo;
    float carril_alto;
    float velocidad_fondo;
    float duracion;
    float tiempo_transcurrido;
    float tiempo_generacion;
    float intervalo_generacion;
    float espera;
    bool vista_fija;
    QList<Obstaculo *> obstaculos;
    QPixmap imagenes_obstaculo[4];
    QVector<QPixmap> cuadros_dron;

private:
    void crear_fondo(const QString &ruta);
    void crear_jugador();
    void crear_obstaculo();
    void mover_fondo(float dt);
    void revisar_frenesi();
    void revisar_fin();
    void avisar_cambios();

    QGraphicsPixmapItem *fondo[2];
    QGraphicsRectItem *sombra;
    bool frenesi_activo;
    bool terminado;

    unsigned short ultimas_vidas;
    unsigned short ultimo_porcentaje;
    unsigned short ultimos_segundos;
};

#endif
