#include "Hospital.h"
#include <iostream>

Hospital::Hospital() : inicio(nullptr) {
    // Los 8 servicios fijos del hospital, en orden
    agregarServicio("Urgencias");
    agregarServicio("Medicina General");
    agregarServicio("Cardiologia");
    agregarServicio("Neurologia");
    agregarServicio("Traumatologia");
    agregarServicio("Cirugia");
    agregarServicio("Pediatria");
    agregarServicio("Hospitalizacion");
}

Hospital::~Hospital() {
    NodoServicio* actual = inicio;
    while (actual != nullptr) {
        NodoServicio* temp = actual;
        actual = actual->siguiente;
        delete temp->dato; // libera el Servicio (el cual libera su arbol AVL interno)
        delete temp;
    }
}

void Hospital::agregarServicio(const std::string& nombre) {
    Servicio* nuevoServicio = new Servicio(nombre);
    NodoServicio* nuevoNodo = new NodoServicio(nuevoServicio);

    if (inicio == nullptr) {
        inicio = nuevoNodo;
        return;
    }

    NodoServicio* actual = inicio;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevoNodo;
}

Servicio* Hospital::buscarServicio(const std::string& nombre) const {
    NodoServicio* actual = inicio;
    while (actual != nullptr) {
        if (actual->dato->getNombre() == nombre) {
            return actual->dato;
        }
        actual = actual->siguiente;
    }

    throw std::string("Servicio no valido: '" + nombre + "'.");
}

std::string Hospital::procesarYDerivarPaciente(Paciente* paciente, std::string& rutaDecision) {
    if (!paciente) return "";

    // 1. Evaluar con el arbol de decision
    std::string servicioAsignado = arbolDecision.evaluar(paciente, rutaDecision);
    paciente->setServicioAsignado(servicioAsignado);

    // 2. Insertar en el AVL del servicio correspondiente
    Servicio* servicio = buscarServicio(servicioAsignado);
    servicio->agregarPaciente(paciente);

    return servicioAsignado;
}

void Hospital::mostrarServicios() const {
    NodoServicio* actual = inicio;
    int contador = 1;
    while (actual != nullptr) {
        std::cout << contador << ". " << actual->dato->getNombre() << std::endl;
        actual = actual->siguiente;
        contador++;
    }
}

void Hospital::mostrarEstadoDepartamentos() const {
    NodoServicio* actual = inicio;
    while (actual != nullptr) {
        actual->dato->mostrarPacientes();
        actual = actual->siguiente;
    }
}

Servicio* Hospital::obtenerPorIndice(int indice) const {
    if (indice < 1) {
        throw std::string("Opcion invalida.");
    }

    NodoServicio* actual = inicio;
    int contador = 1;
    while (actual != nullptr) {
        if (contador == indice) {
            return actual->dato;
        }
        actual = actual->siguiente;
        contador++;
    }

    throw std::string("Opcion invalida.");
}

Paciente* Hospital::buscarPacientePorId(const std::string& id, std::string& departamento, std::string& ruta) const {
    NodoServicio* actual = inicio;
    while (actual != nullptr) {
        ruta = "";
        Paciente* encontrado = actual->dato->buscarPacientePorId(id, ruta);
        if (encontrado != nullptr) {
            departamento = actual->dato->getNombre();
            return encontrado;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Paciente* Hospital::buscarPacientePorNombre(const std::string& nombre, std::string& departamento) const {
    NodoServicio* actual = inicio;
    while (actual != nullptr) {
        Paciente* encontrado = actual->dato->buscarPacientePorNombre(nombre);
        if (encontrado != nullptr) {
            departamento = actual->dato->getNombre();
            return encontrado;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void Hospital::mostrarRecorridosArbolDecision() const {
    std::cout << "--- Recorridos Arbol de Decision ---" << std::endl;
    std::cout << "Preorden: ";
    arbolDecision.mostrarPreorden();
    std::cout << "Inorden: ";
    arbolDecision.mostrarInorden();
    std::cout << "Postorden: ";
    arbolDecision.mostrarPostorden();
}

bool Hospital::darDeAltaPaciente(const std::string& id, std::string& departamento) {
    NodoServicio* actual = inicio;
    while (actual != nullptr) {
        if (actual->dato->eliminarPacientePorId(id)) {
            departamento = actual->dato->getNombre();
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}