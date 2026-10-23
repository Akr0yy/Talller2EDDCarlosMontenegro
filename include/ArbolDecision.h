#pragma once
#include "NodoDecision.h"
#include "Paciente.h"
#include <string>

// Arbol binario manual para clasificar pacientes segun criterios
class ArbolDecision {
private:
    NodoDecision* raiz;

    // Metodos auxiliares recursivos para recorridos
    void preordenAux(NodoDecision* actual) const;
    void inordenAux(NodoDecision* actual) const;
    void postordenAux(NodoDecision* actual) const;

public:
    ArbolDecision();
    ~ArbolDecision();

    // Construye la jerarquia binaria de clasificacion
    void construirArbol();

    // Evalua las condiciones del paciente, construye la ruta en texto y retorna el servicio
    std::string evaluar(const Paciente* paciente, std::string& rutaSalida) const;

    // Recorridos del arbol
    void mostrarPreorden() const;
    void mostrarInorden() const;
    void mostrarPostorden() const;
};