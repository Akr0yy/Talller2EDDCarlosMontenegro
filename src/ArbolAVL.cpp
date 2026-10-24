#include "ArbolAVL.h"
#include <iostream>

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

ArbolAVL::~ArbolAVL() {
    destruirArbol(raiz);
}

void ArbolAVL::destruirArbol(NodoAVL* nodo) {
    if (nodo != nullptr) {
        destruirArbol(nodo->getIzquierdo());
        destruirArbol(nodo->getDerecho());
        delete nodo->getPaciente(); // Libera la memoria del objeto Paciente
        delete nodo;                 // Libera el nodo del arbol
    }
}

int ArbolAVL::obtenerAltura(NodoAVL* nodo) const {
    if (nodo == nullptr) {
        return 0;
    }
    return nodo->getAltura();
}

int ArbolAVL::maximo(int a, int b) const {
    return (a > b) ? a : b;
}

int ArbolAVL::obtenerBalance(NodoAVL* nodo) const {
    if (nodo == nullptr) {
        return 0;
    }
    return obtenerAltura(nodo->getIzquierdo()) - obtenerAltura(nodo->getDerecho());
}

// Rotacion Simple Derecha (RSD / Caso Izquierda-Izquierda)
NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->getIzquierdo();
    NodoAVL* T2 = x->getDerecho();

    // realizar rotacion
    x->setDerecho(y);
    y->setIzquierdo(T2);

    // actualizar alturas
    y->setAltura(maximo(obtenerAltura(y->getIzquierdo()), obtenerAltura(y->getDerecho())) + 1);
    x->setAltura(maximo(obtenerAltura(x->getIzquierdo()), obtenerAltura(x->getDerecho())) + 1);

    return x; // Nueva raiz del subarbol
}

// Rotacion Simple Izquierda (RSI / Caso Derecha-Derecha)
NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->getDerecho();
    NodoAVL* T2 = y->getIzquierdo();

    // realizar rotacion
    y->setIzquierdo(x);
    x->setDerecho(T2);

    // actualizar alturas
    x->setAltura(maximo(obtenerAltura(x->getIzquierdo()), obtenerAltura(x->getDerecho())) + 1);
    y->setAltura(maximo(obtenerAltura(y->getIzquierdo()), obtenerAltura(y->getDerecho())) + 1);

    return y; // Nueva raiz del subarbol
}

NodoAVL* ArbolAVL::insertarAux(NodoAVL* nodo, Paciente* paciente) {
    // 1. Insercion normal de un BST basada en el ID
    if (nodo == nullptr) {
        return new NodoAVL(paciente);
    }

    if (paciente->getId() < nodo->getPaciente()->getId()) {
        nodo->setIzquierdo(insertarAux(nodo->getIzquierdo(), paciente));
    } else if (paciente->getId() > nodo->getPaciente()->getId()) {
        nodo->setDerecho(insertarAux(nodo->getDerecho(), paciente));
    } else {
        // ID duplicado dentro del mismo AVL, se rechaza la insercion
        std::cerr << "Advertencia: Paciente con ID " << paciente->getId() << " ya existe en este servicio." << std::endl;
        delete paciente;
        return nodo;
    }

    // 2. Actualizar altura del nodo ancestro
    nodo->setAltura(1 + maximo(obtenerAltura(nodo->getIzquierdo()), obtenerAltura(nodo->getDerecho())));

    // 3. Obtener el factor de balance
    int balance = obtenerBalance(nodo);

    // 4. Casos de rotacion si el nodo quedo desbalanceado

    // Caso 1: Izquierda - Izquierda (Rotacion Simple Derecha)
    if (balance > 1 && paciente->getId() < nodo->getIzquierdo()->getPaciente()->getId()) {
        return rotacionDerecha(nodo);
    }

    // Caso 2: Derecha - Derecha (Rotacion Simple Izquierda)
    if (balance < -1 && paciente->getId() > nodo->getDerecho()->getPaciente()->getId()) {
        return rotacionIzquierda(nodo);
    }

    // Caso 3: Izquierda - Derecha (Rotacion Doble Izquierda-Derecha)
    if (balance > 1 && paciente->getId() > nodo->getIzquierdo()->getPaciente()->getId()) {
        nodo->setIzquierdo(rotacionIzquierda(nodo->getIzquierdo()));
        return rotacionDerecha(nodo);
    }

    // Caso 4: Derecha - Izquierda (Rotacion Doble Derecha-Izquierda)
    if (balance < -1 && paciente->getId() < nodo->getDerecho()->getPaciente()->getId()) {
        nodo->setDerecho(rotacionDerecha(nodo->getDerecho()));
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

void ArbolAVL::insertar(Paciente* paciente) {
    if (paciente != nullptr) {
        raiz = insertarAux(raiz, paciente);
    }
}

int ArbolAVL::getAlturaArbol() const {
    return obtenerAltura(raiz);
}

int ArbolAVL::getBalanceRaiz() const {
    return obtenerBalance(raiz);
}

bool ArbolAVL::estaVacio() const {
    return raiz == nullptr;
}