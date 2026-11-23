#ifndef ESTADOS_H
#define ESTADOS_H

class Estados {
public:
    Estados();

    void asustar();
    void registrar_agachada();
    void actualizar(float dt);

    float factor_salto() const;
    float factor_gravedad() const;
    float factor_friccion() const;
    bool esta_asustado() const;
    bool esta_cansado() const;

private:
    float tiempo_asustado;
    float tiempo_cansado;
    unsigned short agachadas;
    float tiempo_agachadas;
};

#endif 
