#ifndef SONIDO_H
#define SONIDO_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

class Sonido : public QObject
{
    Q_OBJECT

public:
    Sonido(QObject *parent = nullptr);

    void iniciar_musica();
    void detener_musica();

public slots:
    void reproducir_golpe();
    void reproducir_disparo();

private:
    enum Efecto { GOLPE, DISPARO, CANTIDAD_EFECTOS };

    QMediaPlayer *musica;
    QAudioOutput *salida;
    QSoundEffect efectos[CANTIDAD_EFECTOS]; // se accede directo con el enum
};

#endif // SONIDO_H
