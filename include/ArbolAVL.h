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

    // Rotaciones simples 
    NodoAVL* rotacionDerecha(NodoAVL* y);
    NodoAVL* rotacionIzquierda(NodoAVL* x);

    // Rotaciones dobles 
    NodoAVL* rotacionIzquierdaDerecha(NodoAVL* nodo);
    NodoAVL* rotacionDerechaIzquierda(NodoAVL* nodo);

    // Insercion y eliminacion recursiva
    NodoAVL* insertarAux(NodoAVL* nodo, Paciente* paciente);
    NodoAVL* eliminarAux(NodoAVL* nodo, const std::string& id, bool& eliminado);
    NodoAVL* obtenerNodoConValorMinimo(NodoAVL* nodo) const;

    // Busquedas auxiliares
    Paciente* buscarPorIdAux(NodoAVL* nodo, const std::string& id, std::string& ruta) const;
    Paciente* buscarPorNombreAux(NodoAVL* nodo, const std::string& nombre) const;

    // Recorridos auxiliares
    void inordenAux(NodoAVL* nodo) const;
    void preordenAux(NodoAVL* nodo) const;
    void postordenAux(NodoAVL* nodo) const;

    // Destruccion recursiva
    void destruirArbol(NodoAVL* nodo);

public:
    ArbolAVL();
    ~ArbolAVL();

    // Insercion
    void insertar(Paciente* paciente);
    // Eliminacion
    bool eliminarPorId(const std::string& id);

    // Metodos de busqueda
    Paciente* buscarPorId(const std::string& id, std::string& ruta) const;
    Paciente* buscarPorNombre(const std::string& nombre) const;

    // Recorridos 
    void mostrarInorden() const;
    void mostrarPreorden() const;
    void mostrarPostorden() const;

    // Estado del arbol
    int getAlturaArbol() const;

    // Retorna el factor de balance de la raiz
    int getBalanceRaiz() const;

    bool estaVacio() const;
};