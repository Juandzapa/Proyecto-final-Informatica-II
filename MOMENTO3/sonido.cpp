#include "sonido.h"
#include <QFile>
#include <QUrl>
#include <stdexcept>

Sonido::Sonido(QObject *parent)
    : QObject(parent),
      musica(nullptr),
      salida(nullptr)
{

    if (!QFile::exists(":/sonidos/musica.wav")) {
        throw std::runtime_error("Falta el archivo musica.wav");
    }
    if (!QFile::exists(":/sonidos/golpe.wav") || !QFile::exists(":/sonidos/disparo.wav")) {
        throw std::runtime_error("Faltan los efectos de sonido");
    }

    musica = new QMediaPlayer(this);
    salida = new QAudioOutput(this);
    musica->setAudioOutput(salida);
    musica->setSource(QUrl("qrc:/sonidos/musica.wav"));
    musica->setLoops(QMediaPlayer::Infinite);
    salida->setVolume(0.4f);

    efectos[GOLPE].setSource(QUrl("qrc:/sonidos/golpe.wav"));
    efectos[DISPARO].setSource(QUrl("qrc:/sonidos/disparo.wav"));
    efectos[GOLPE].setVolume(0.8f);
    efectos[DISPARO].setVolume(0.5f);
}

void Sonido::iniciar_musica()
{
    musica->setPosition(0);
    musica->play();
}

void Sonido::detener_musica()
{
    musica->stop();
}

void Sonido::reproducir_golpe()
{
    efectos[GOLPE].play();
}

void Sonido::reproducir_disparo()
{
    efectos[DISPARO].play();
}
