#include "Servicio.h"
#include <iostream>

Servicio::Servicio(const std::string& nombre)
    : nombre(nombre), cantidad(0) {}

Servicio::~Servicio() {}

std::string Servicio::getNombre() const {
    return nombre;
}

int Servicio::getCantidad() const {
    return cantidad;
}

void Servicio::agregarPaciente(Paciente* paciente) {
    if (paciente != nullptr) {
        pacientesAVL.insertar(paciente);
        cantidad++;
    }
}

Paciente* Servicio::buscarPacientePorId(const std::string& id, std::string& ruta) const {
    return pacientesAVL.buscarPorId(id, ruta);
}

Paciente* Servicio::buscarPacientePorNombre(const std::string& nombre) const {
    return pacientesAVL.buscarPorNombre(nombre);
}

bool Servicio::eliminarPacientePorId(const std::string& id) {
    bool eliminado = pacientesAVL.eliminarPorId(id);
    if (eliminado) {
        cantidad--;
    }
    return eliminado;
}

void Servicio::mostrarPacientes() const {
    std::cout << nombre << "\n";
    pacientesAVL.mostrarInorden();
}

void Servicio::mostrarRecorridos() const {
    std::cout << "--- Recorridos AVL: " << nombre << " ---" << std::endl;
    std::cout << "Inorden: ";
    pacientesAVL.mostrarInorden();
    std::cout << "Preorden: ";
    pacientesAVL.mostrarPreorden();
    std::cout << "Postorden: ";
    pacientesAVL.mostrarPostorden();
    std::cout << "Altura: " << pacientesAVL.getAlturaArbol() 
              << " | Factor Balance Raiz: " << pacientesAVL.getBalanceRaiz() << std::endl;
}

int Servicio::getAlturaAVL() const {
    return pacientesAVL.getAlturaArbol();
}

int Servicio::getBalanceAVL() const {
    return pacientesAVL.getBalanceRaiz();
}