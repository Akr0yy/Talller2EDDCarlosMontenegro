#include "NodoAVL.h"

NodoAVL::NodoAVL(Paciente* p)
    : paciente(p), izquierdo(nullptr), derecho(nullptr), altura(1) {}

NodoAVL::~NodoAVL() {
    
}

Paciente* NodoAVL::getPaciente() const {
    return paciente;
}

NodoAVL* NodoAVL::getIzquierdo() const {
    return izquierdo;
}

NodoAVL* NodoAVL::getDerecho() const {
    return derecho;
}

int NodoAVL::getAltura() const {
    return altura;
}

void NodoAVL::setIzquierdo(NodoAVL* izq) {
    izquierdo = izq;
}

void NodoAVL::setDerecho(NodoAVL* der) {
    derecho = der;
}

void NodoAVL::setAltura(int alt) {
    altura = alt;
}

void NodoAVL::setPaciente(Paciente* p) {
    paciente = p;
}