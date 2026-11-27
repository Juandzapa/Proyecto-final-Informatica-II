#include "nivel.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QTransform>
#include <QVector>
#include <cmath>
#include <stdexcept>
#include <QRandomGenerator>

Nivel::Nivel(NivelDificultad dif, float duracion, const QString &ruta_fondo, QObject *parent) : QGraphicsScene(parent), jugador(nullptr), papa(nullptr), dificultad(dif), linea_suelo(380.0f), carril_bajo(360.0f), carril_alto(325.0f), velocidad_fondo(200.0f), duracion(duracion), tiempo_transcurrido(0.0f), tiempo_generacion(0.0f), intervalo_generacion(1.5f), espera(1.5f), vista_fija(false), sombra(nullptr), frenesi_activo(false), terminado(false), ultimas_vidas(99), ultimo_porcentaje(999), ultimos_segundos(999)
{
    setSceneRect(0, 0, ANCHO_ESCENA, ALTO_ESCENA);

    crear_fondo(ruta_fondo);
    crear_jugador();

    imagenes_obstaculo[FAROL] = imagen_o_provisional(":/sprites/farol.png", 27, 55, QColor(150, 100, 50));
    imagenes_obstaculo[CARTEL] = imagen_o_provisional(":/sprites/cartel.png", 24, 38, QColor(120, 120, 120));
    imagenes_obstaculo[SABANA] = imagen_o_provisional(":/sprites/sabana.png", 72, 15, QColor(220, 215, 200));
    cuadros_dron.append(imagen_o_provisional(":/sprites/dron_1.png", 56, 18, QColor(60, 60, 70)));
    cuadros_dron.append(imagen_o_provisional(":/sprites/dron_2.png", 56, 18, QColor(60, 60, 70)));
    imagenes_obstaculo[DRON] = cuadros_dron.at(0);
    imagenes_obstaculo[PINCHOS] = imagen_o_provisional(":/sprites/pinchos.png", 32, 32, QColor(120, 120, 130));
    imagenes_obstaculo[BOMBA] = imagen_o_provisional(":/sprites/bomba.png", 23, 32, QColor(40, 40, 50));
    imagenes_obstaculo[METEORO] = imagen_o_provisional(":/sprites/meteoro.png", 22, 32, QColor(120, 70, 40));

    sombra = addRect(0, 0, ANCHO_ESCENA, ALTO_ESCENA, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 90)));
    sombra->setZValue(3);
    sombra->setVisible(false);
}

void Nivel::crear_fondo(const QString &ruta) {
    fondo[0] = nullptr;
    fondo[1] = nullptr;

    QPixmap imagen;
    try {
        imagen = cargar_imagen(ruta);
    }
    catch (const std::runtime_error &) {
        return;
    }
    imagen = imagen.scaled(ANCHO_ESCENA, ALTO_ESCENA, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap volteada = imagen.transformed(QTransform().scale(-1, 1));

    fondo[0] = addPixmap(imagen);
    fondo[1] = addPixmap(volteada);
    fondo[0]->setPos(0, 0);
    fondo[1]->setPos(ANCHO_ESCENA, 0);
    fondo[0]->setZValue(0);
    fondo[1]->setZValue(0);
}

void Nivel::crear_jugador() {
    QColor rosado(235, 130, 160);

    QVector<QPixmap> correr;
    for (int i = 1; i <= 4; i++) {
        QString ruta = QString(":/sprites/jugador_correr_%1.png").arg(i);
        correr.append(imagen_o_provisional(ruta, 42, 60, rosado));
    }

    QVector<QPixmap> salto;
    for (int i = 1; i <= 5; i++) {
        try {
            salto.append(cargar_imagen(QString(":/sprites/jugador_salto_%1.png").arg(i)));
        }
        catch (const std::runtime_error &) {
            salto.append(correr.at(0));
        }
    }

    QVector<QPixmap> deslizar;
    for (int i = 1; i <= 3; i++) {
        try {
            deslizar.append(cargar_imagen(QString(":/sprites/jugador_desliza_%1.png").arg(i)));
        }
        catch (const std::runtime_error &) {
            deslizar.append(correr.at(1).scaled(42, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }

    QVector<QPixmap> agachar;
    QVector<QPixmap> quieto;
    for (int i = 1; i <= 2; i++) {
        try {
            agachar.append(cargar_imagen(QString(":/sprites/jugador_agachado_%1.png").arg(i)));
        }
        catch (const std::runtime_error &) {
            agachar.append(deslizar.at(deslizar.size() - 1));
        }
        try {
            quieto.append(cargar_imagen(QString(":/sprites/jugador_quieto_%1.png").arg(i)));
        }
        catch (const std::runtime_error &) {
            quieto.append(correr.at(0));
        }
    }

    QPixmap golpe;
    try {
        golpe = cargar_imagen(":/sprites/jugador_golpe.png").scaled(42, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    catch (const std::runtime_error &) {
        golpe = correr.at(0);
        QPainter pintor(&golpe);
        pintor.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        pintor.fillRect(golpe.rect(), QColor(220, 30, 30, 130));
        pintor.end();
    }

    jugador = new Personaje(correr, salto, deslizar, agachar, quieto, golpe, linea_suelo, 360.0f);
    addItem(jugador);
}

QPixmap Nivel::cargar_imagen(const QString &ruta)
{
    QPixmap imagen(ruta);
    if (imagen.isNull()) {
        throw std::runtime_error("No se pudo cargar la imagen: " + ruta.toStdString());
    }
    return imagen;
}

QPixmap Nivel::imagen_o_provisional(const QString &ruta, int ancho, int alto, const QColor &color)
{
    try {
        QPixmap imagen = cargar_imagen(ruta);
        return imagen.scaled(ancho, alto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    catch (const std::runtime_error &) {
        QPixmap provisional(ancho, alto);
        provisional.fill(color);
        return provisional;
    }
}

void Nivel::actualizar_obstaculos(float dt)
{
    for (int i = 0; i < obstaculos.size(); i++) {
        obstaculos[i]->actualizar(dt);
    }

    tiempo_generacion += dt;
    if (tiempo_generacion >= espera) {
        crear_obstaculo();
        tiempo_generacion = 0.0f;

        float azar = 0.7f + QRandomGenerator::global()->bounded(60) / 100.0f;
        espera = intervalo_generacion * azar;
        if (espera < 0.8f) {
            espera = 0.8f;
        }
    }
}

void Nivel::crear_obstaculo()
{
    Obstaculo *nuevo = nullptr;
    int indice = QRandomGenerator::global()->bounded(static_cast<int>(tipos_obstaculo.size()));
    TipoObstaculo tipo = tipos_obstaculo.at(indice);

    if (vista_fija) {
        float x = 180.0f + QRandomGenerator::global()->bounded(560);
        nuevo = new Obstaculo(imagenes_obstaculo[tipo], tipo, x, linea_suelo, 0.0f);
        nuevo->soltar_desde_arriba(900.0f, 3.0f);
    }
    else {
        nuevo = new Obstaculo(imagenes_obstaculo[tipo], tipo, ANCHO_ESCENA, linea_suelo, velocidad_fondo);
        if (tipo == DRON) {
            nuevo->set_cuadros(cuadros_dron);
            nuevo->poner_en_altura(linea_suelo - 40.0f - nuevo->pixmap().height());
        }
    }
    addItem(nuevo);
    obstaculos.append(nuevo);
}

unsigned short Nivel::revisar_obstaculos()
{
    unsigned short superados = 0;

    for (int i = obstaculos.size() - 1; i >= 0; i--) {
        Obstaculo *obstaculo = obstaculos[i];

        if (!obstaculo->fue_golpeado() && jugador->collidesWithItem(obstaculo)) {
            obstaculo->marcar_golpeado();
            jugador->recibir_golpe(obstaculo->get_empuje(), dificultad.get_k_friccion());
            emit hubo_golpe();
        }
        else if (!vista_fija && !obstaculo->fue_superado() && obstaculo->x() + obstaculo->pixmap().width() < jugador->x()) {
            obstaculo->marcar_superado();
            superados++;
        }

        if (obstaculo->fuera_de_pantalla() || obstaculo->termino()) {
            removeItem(obstaculo);
            delete obstaculo;
            obstaculos.removeAt(i);
        }
    }
    return superados;
}

void Nivel::actualizar(float dt) {
    if (terminado) {
        return;
    }

    tiempo_transcurrido += dt;

    mover_fondo(dt);
    jugador->actualizar(dt);
    papa->actualizar(dt);
    actualizar_elementos(dt);
    revisar_colisiones();
    revisar_frenesi();
    revisar_fin();

    if (!terminado) {
        avisar_cambios();
    }
}

void Nivel::mover_fondo(float dt) {
    if (fondo[0] == nullptr || vista_fija) {
        return;
    }

    for (int i = 0; i < 2; i++) {
        float nueva_x = fondo[i]->x() - velocidad_fondo * dt;
        if (nueva_x <= -ANCHO_ESCENA) {
            nueva_x += 2 * ANCHO_ESCENA;
        }
        fondo[i]->setX(nueva_x);
    }
}

void Nivel::revisar_frenesi() {

    if (!frenesi_activo && tiempo_transcurrido >= 0.7f * duracion) {
        frenesi_activo = true;
        sombra->setVisible(true);
        intervalo_generacion = intervalo_generacion / 2.0f;
        papa->acelerar_disparos();
    }
}

void Nivel::revisar_fin() {

    bool atrapado = jugador->x() <= papa->x() + papa->pixmap().width();
    if (atrapado || jugador->get_vidas() == 0) {
        terminado = true;
        emit nivel_perdido();
        return;
    }

    if (tiempo_transcurrido >= duracion) {
        terminado = true;
        emit nivel_ganado();
    }
}

void Nivel::avisar_cambios() {

    unsigned short vidas = jugador->get_vidas();
    unsigned short porcentaje = static_cast<unsigned short>(100.0f * tiempo_transcurrido / duracion);
    unsigned short segundos = static_cast<unsigned short>(std::ceil(duracion - tiempo_transcurrido));

    if (vidas != ultimas_vidas || porcentaje != ultimo_porcentaje || segundos != ultimos_segundos) {
        ultimas_vidas = vidas;
        ultimo_porcentaje = porcentaje;
        ultimos_segundos = segundos;
        emit cambiaron_datos(vidas, porcentaje, segundos);
    }
}

void Nivel::procesar_tecla(int tecla, bool presionada)
{
    if (tecla == Qt::Key_W && presionada) {
        jugador->saltar();
    }
    else if (tecla == Qt::Key_S) {
        jugador->agacharse(presionada);
    }
    else if (tecla == Qt::Key_A) {
        if (presionada) {
            jugador->mover(-1);
        }
        else {
            jugador->mover(0);
        }
    }
    else if (tecla == Qt::Key_D) {
        if (presionada) {
            jugador->mover(1);
        }
        else {
            jugador->mover(0);
        }
    }
}
