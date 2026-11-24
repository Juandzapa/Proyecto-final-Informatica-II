#include "personaje.h"
#include <QTransform>
#include <cmath>

Personaje::Personaje(const QVector<QPixmap> &correr, const QVector<QPixmap> &salto, const QVector<QPixmap> &deslizar, const QVector<QPixmap> &agachar, const QVector<QPixmap> &quieto, const QPixmap &img_golpe, float linea_suelo, float x_base)
    : ObjetoJuego(correr.at(0)),
      cuadros_correr(correr),
      cuadros_salto(salto),
      cuadros_deslizar(deslizar),
      cuadros_agachado(agachar),
      cuadros_quieto(quieto),
      cuadro_quieto(0),
      tiempo_quieto(0.0f),
      imagen_golpe(img_golpe),
      cuadro_actual(0),
      tiempo_cuadro(0.0f),
      tiempo_golpe(0.0f),
      tiempo_agachado(0.0f),
      imagen_puesta(nullptr),
      mirando_izquierda(false),
      izquierda_puesta(false),
      vidas(3),
      linea_suelo(linea_suelo),
      altura(0.0f),
      t_salto(0.0f),
      v0_salto(650.0f),
      gravedad(1800.0f),
      v0_actual(650.0f),
      g_actual(1800.0f),
      en_suelo(true),
      agachado(false),
      arrastrando(false),
      x_inicio_arrastre(x_base),
      v0_arrastre(0.0f),
      k_arrastre(1.0f),
      t_arrastre(0.0f),
      x_base(x_base),
      velocidad_regreso(50.0f),
      movimiento_libre(false),
      direccion(0),
      velocidad_lateral(180.0f),
      x_minima(0.0f),
      x_maxima(ANCHO_ESCENA),
      efecto_cansado(nullptr),
      mostrando_cansado(false)
{
    setPos(x_base, linea_suelo - pixmap().height());
    setZValue(1);

    efecto_cansado = new QGraphicsColorizeEffect();
    efecto_cansado->setColor(QColor(70, 110, 255));
    efecto_cansado->setStrength(0.6);
    efecto_cansado->setEnabled(false);
    setGraphicsEffect(efecto_cansado);
}

void Personaje::saltar(){
    if (en_suelo && !agachado) {
        en_suelo = false;
        t_salto = 0.0f;
        v0_actual = v0_salto * estados.factor_salto();
        g_actual = gravedad * estados.factor_gravedad();
    }
}

void Personaje::agacharse(bool estado){
    if (!en_suelo) {
        return;
    }

    if (estado && !agachado) {
        estados.registrar_agachada();
        tiempo_agachado = 0.0f;
    }
    agachado = estado;
}

void Personaje::recibir_golpe(float v0, float k){
    arrastrando = true;
    t_arrastre = 0.0f;
    x_inicio_arrastre = x();
    v0_arrastre = v0;
    k_arrastre = k * estados.factor_friccion();
    tiempo_golpe = 0.4f;
}

void Personaje::asustar(){
    estados.asustar();
}

void Personaje::perder_vida(){
    if (vidas > 0) {
        vidas--;
    }
}

void Personaje::permitir_movimiento(float minimo, float maximo){
    movimiento_libre = true;
    x_minima = minimo;
    x_maxima = maximo;
}

void Personaje::mover(short sentido){
    direccion = sentido;
    if (sentido < 0) {
        mirando_izquierda = true;
    }
    else if (sentido > 0) {
        mirando_izquierda = false;
    }
}

void Personaje::actualizar(float dt){
    estados.actualizar(dt);
    actualizar_salto(dt);
    actualizar_arrastre(dt);
    actualizar_imagen(dt);
}

void Personaje::actualizar_salto(float dt){
    if (en_suelo) {
        return;
    }

    t_salto += dt;
    altura = v0_actual * t_salto - 0.5f * g_actual * t_salto * t_salto;

    if (altura <= 0.0f) {
        altura = 0.0f;
        en_suelo = true;
        t_salto = 0.0f;
    }
}

void Personaje::actualizar_arrastre(float dt){
    if (arrastrando) {
        t_arrastre += dt;
        float frenado = std::exp(-k_arrastre * t_arrastre);
        setX(x_inicio_arrastre - (v0_arrastre / k_arrastre) * (1.0f - frenado));

        if (v0_arrastre * frenado < 5.0f) {
            arrastrando = false;
            if (movimiento_libre) {
                x_base = x();
            }
        }
    }
    else if (movimiento_libre && direccion != 0 && !agachado) {
        float nueva_x = x() + direccion * velocidad_lateral * dt;
        if (nueva_x < x_minima) {
            nueva_x = x_minima;
        }
        if (nueva_x > x_maxima) {
            nueva_x = x_maxima;
        }
        setX(nueva_x);
        x_base = nueva_x;
    }
    else if (x() < x_base) {
        float nueva_x = x() + velocidad_regreso * dt;
        if (nueva_x > x_base) {
            nueva_x = x_base;
        }
        setX(nueva_x);
    }
}

void Personaje::actualizar_imagen(float dt){
    const QPixmap *nueva = nullptr;

    if (tiempo_golpe > 0.0f) {
        tiempo_golpe -= dt;
        nueva = &imagen_golpe;
    }
    else if (!en_suelo) {
        float duracion_salto = 2.0f * v0_actual / g_actual;
        float avance = t_salto / duracion_salto;
        int indice = static_cast<int>(avance * cuadros_salto.size());
        if (indice >= cuadros_salto.size()) {
            indice = cuadros_salto.size() - 1;
        }
        nueva = &cuadros_salto.at(indice);
    }
    else if (agachado) {
        const QVector<QPixmap> *cuadros = &cuadros_deslizar;
        if (movimiento_libre) {
            cuadros = &cuadros_agachado;
        }
        tiempo_agachado += dt;
        int indice = static_cast<int>(tiempo_agachado / 0.08f);
        if (indice >= cuadros->size()) {
            indice = cuadros->size() - 1;
        }
        nueva = &cuadros->at(indice);
    }
    else if (movimiento_libre && direccion == 0) {
        tiempo_quieto += dt;
        if (tiempo_quieto >= 0.5f) {
            tiempo_quieto = 0.0f;
            cuadro_quieto = static_cast<unsigned short>((cuadro_quieto + 1) % cuadros_quieto.size());
        }
        nueva = &cuadros_quieto.at(cuadro_quieto);
    }
    else {
        tiempo_cuadro += dt;
        if (tiempo_cuadro >= 0.1f) {
            tiempo_cuadro = 0.0f;
            cuadro_actual = static_cast<unsigned short>((cuadro_actual + 1) % cuadros_correr.size());
        }
        nueva = &cuadros_correr.at(cuadro_actual);
    }

    if (nueva != imagen_puesta || mirando_izquierda != izquierda_puesta) {
        setPixmap(*nueva);
        imagen_puesta = nueva;
        izquierda_puesta = mirando_izquierda;

        if (mirando_izquierda) {
            setTransform(QTransform().translate(pixmap().width(), 0).scale(-1, 1));
        }
        else {
            setTransform(QTransform());
        }
    }

    if (estados.esta_asustado()) {
        setOpacity(0.6);
    }
    else {
        setOpacity(1.0);
    }

    if (estados.esta_cansado() != mostrando_cansado) {
        mostrando_cansado = estados.esta_cansado();
        efecto_cansado->setEnabled(mostrando_cansado);
    }

    setY(linea_suelo - pixmap().height() - altura);
}

bool Personaje::esta_en_suelo() const{
    return en_suelo;
}

unsigned short Personaje::get_vidas() const{
    return vidas;
}
