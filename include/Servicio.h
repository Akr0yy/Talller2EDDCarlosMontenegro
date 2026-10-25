#pragma once
#include "ArbolAVL.h"
#include <string>

// Representa un servicio/departamento del hospital.
// Mantiene su propio arbol AVL manual de pacientes asignados.
class Servicio {
private:
    std::string nombre;
    ArbolAVL pacientesAVL;
    int cantidad;

public:
    Servicio(const std::string& nombre);
    ~Servicio(); // ArbolAVL libera recursivamente los nodos y los pacientes

    std::string getNombre() const;
    int getCantidad() const;

    // Agrega un paciente al AVL balanceado del servicio
    void agregarPaciente(Paciente* paciente);

    // Busca un paciente por id y obtiene la ruta de busqueda en el AVL
    Paciente* buscarPacientePorId(const std::string& id, std::string& ruta) const;

    // Busca un paciente por nombre recorriendo el AVL
    Paciente* buscarPacientePorNombre(const std::string& nombre) const;

    // Elimina un paciente por su id rebalanceando el AVL
    bool eliminarPacientePorId(const std::string& id);

    // Muestra los pacientes en formato inorden
    void mostrarPacientes() const;

    void mostrarRecorridosCompletos() const;

    // Consultas de la estructura AVL
    void mostrarRecorridos() const;
    int getAlturaAVL() const;
    int getBalanceAVL() const;
};