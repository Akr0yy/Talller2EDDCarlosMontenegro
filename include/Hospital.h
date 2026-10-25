#pragma once
#include "NodoServicio.h"
#include "ArbolDecision.h"
#include <string>

// Lista enlazada principal del hospital con sus servicios y arbol de clasificacion
class Hospital {
private:
    NodoServicio* inicio;
    ArbolDecision arbolDecision;

    void agregarServicio(const std::string& nombre);

public:
    Hospital();
    ~Hospital(); // Libera todos los Servicios y sus respectivos arboles AVL

    Servicio* buscarServicio(const std::string& nombre) const;

    // Evalua al paciente con el Arbol de Decision y lo inserta en el AVL correspondiente
    std::string procesarYDerivarPaciente(Paciente* paciente, std::string& rutaDecision);

    void mostrarServicios() const;
    void mostrarEstadoDepartamentos() const;
    Servicio* obtenerPorIndice(int indice) const;

    // Busqueda de paciente en todos los arboles AVL
    Paciente* buscarPacientePorId(const std::string& id, std::string& departamento, std::string& ruta) const;
    Paciente* buscarPacientePorNombre(const std::string& nombre, std::string& departamento) const;

    // Recorridos del Arbol de Decision
    void mostrarRecorridosArbolDecision() const;

    // Da de alta (elimina) un paciente por ID de los arboles AVL del hospital
    bool darDeAltaPaciente(const std::string& id, std::string& departamento);
};