#pragma once
#include "NodoAVL.h"
#include <string>

// AVL manual indexado por ID de paciente
class ArbolAVL {
private:
    NodoAVL* raiz;

    // Metodos auxiliares de altura y balance
    int obtenerAltura(NodoAVL* nodo) const;
    int obtenerBalance(NodoAVL* nodo) const;
    int maximo(int a, int b) const;

    // Rotaciones
    NodoAVL* rotacionDerecha(NodoAVL* y);
    NodoAVL* rotacionIzquierda(NodoAVL* x);

    // Insercion recursiva con balanceo
    NodoAVL* insertarAux(NodoAVL* nodo, Paciente* paciente);

    // Destruccion recursiva de nodos
    void destruirArbol(NodoAVL* nodo);

public:
    ArbolAVL();
    ~ArbolAVL();

    // Inserta un nuevo paciente manteniendo el balance
    void insertar(Paciente* paciente);

    // Retorna la altura total del arbol
    int getAlturaArbol() const;

    // Retorna el factor de balance de la raiz
    int getBalanceRaiz() const;

    bool estaVacio() const;
};