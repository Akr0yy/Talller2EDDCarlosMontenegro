#pragma once
#include "Paciente.h"

// Representa un nodo dentro del arbol AVL de un servicio hospitalario
// Almacena un puntero al Paciente, su altura y punteros a sus hijos
class NodoAVL {
private:
    Paciente* paciente;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

public:
    NodoAVL(Paciente* p);
    ~NodoAVL();

    Paciente* getPaciente() const;
    NodoAVL* getIzquierdo() const;
    NodoAVL* getDerecho() const;
    int getAltura() const;

    void setIzquierdo(NodoAVL* izq);
    void setDerecho(NodoAVL* der);
    void setAltura(int alt);
    void setPaciente(Paciente* p);
};