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

// Rotación simple a la derecha
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

// Rotación simple a la izquierda
NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->getDerecho();
    NodoAVL* T2 = y->getIzquierdo();

    // realizar rotacion
    y->setIzquierdo(x);
    x->setDerecho(T2);

    // actualizar alturas
    x->setAltura(maximo(obtenerAltura(x->getIzquierdo()), obtenerAltura(x->getDerecho())) + 1);
    y->setAltura(maximo(obtenerAltura(y->getIzquierdo()), obtenerAltura(y->getDerecho())) + 1);

    return y;
}

// Rotación doble: Izquierda-Derecha (LR)
NodoAVL* ArbolAVL::rotacionIzquierdaDerecha(NodoAVL* nodo) {
    nodo->setIzquierdo(rotacionIzquierda(nodo->getIzquierdo()));
    return rotacionDerecha(nodo);
}

// Rotación doble: Derecha-Izquierda (RL)
NodoAVL* ArbolAVL::rotacionDerechaIzquierda(NodoAVL* nodo) {
    nodo->setDerecho(rotacionDerecha(nodo->getDerecho()));
    return rotacionIzquierda(nodo);
}

NodoAVL* ArbolAVL::insertarAux(NodoAVL* nodo, Paciente* paciente) {
    if (nodo == nullptr) {
        return new NodoAVL(paciente);
    }

    if (paciente->getId() < nodo->getPaciente()->getId()) {
        nodo->setIzquierdo(insertarAux(nodo->getIzquierdo(), paciente));
    } else if (paciente->getId() > nodo->getPaciente()->getId()) {
        nodo->setDerecho(insertarAux(nodo->getDerecho(), paciente));
    } else {
        std::cerr << "Advertencia: Paciente con ID " << paciente->getId() << " ya existe en este servicio." << std::endl;
        delete paciente;
        return nodo;
    }

    nodo->setAltura(1 + maximo(obtenerAltura(nodo->getIzquierdo()), obtenerAltura(nodo->getDerecho())));

    int balance = obtenerBalance(nodo);

    // Caso LL: Rotación Simple Derecha
    if (balance > 1 && paciente->getId() < nodo->getIzquierdo()->getPaciente()->getId()) {
        return rotacionDerecha(nodo);
    }

    // Caso RR: Rotación Simple Izquierda
    if (balance < -1 && paciente->getId() > nodo->getDerecho()->getPaciente()->getId()) {
        return rotacionIzquierda(nodo);
    }

    // Caso LR: Rotación Doble Izquierda-Derecha
    if (balance > 1 && paciente->getId() > nodo->getIzquierdo()->getPaciente()->getId()) {
        return rotacionIzquierdaDerecha(nodo);
    }

    // Caso RL: Rotación Doble Derecha-Izquierda
    if (balance < -1 && paciente->getId() < nodo->getDerecho()->getPaciente()->getId()) {
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

void ArbolAVL::insertar(Paciente* paciente) {
    if (paciente != nullptr) {
        raiz = insertarAux(raiz, paciente);
    }
}

NodoAVL* ArbolAVL::obtenerNodoConValorMinimo(NodoAVL* nodo) const {
    NodoAVL* actual = nodo;
    while (actual->getIzquierdo() != nullptr) {
        actual = actual->getIzquierdo();
    }
    return actual;
}

NodoAVL* ArbolAVL::eliminarAux(NodoAVL* nodo, const std::string& id, bool& eliminado) {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (id < nodo->getPaciente()->getId()) {
        nodo->setIzquierdo(eliminarAux(nodo->getIzquierdo(), id, eliminado));
    } else if (id > nodo->getPaciente()->getId()) {
        nodo->setDerecho(eliminarAux(nodo->getDerecho(), id, eliminado));
    } else {
        // Encontramos el nodo a eliminar
        eliminado = true;

        // Caso 1: Hoja (sin hijos)
        if (nodo->getIzquierdo() == nullptr && nodo->getDerecho() == nullptr) {
            delete nodo->getPaciente();
            delete nodo;
            return nullptr;
        }
        // Caso 2: Un solo hijo
        else if (nodo->getIzquierdo() == nullptr) {
            NodoAVL* hijo = nodo->getDerecho();
            delete nodo->getPaciente();
            delete nodo;
            return hijo;
        } else if (nodo->getDerecho() == nullptr) {
            NodoAVL* hijo = nodo->getIzquierdo();
            delete nodo->getPaciente();
            delete nodo;
            return hijo;
        }
        // Caso 3: Dos hijos
        else {
            // Obtenemos el menor del subarbol derecho (sucesor inorden)
            NodoAVL* sucesor = obtenerNodoConValorMinimo(nodo->getDerecho());

            // 1. Guardamos el ID del sucesor
            std::string idSucesor = sucesor->getPaciente()->getId();

            // 2. Liberamos el paciente que se elimina
            delete nodo->getPaciente();

            // 3. Clonamos el paciente del sucesor en el nodo actual
            nodo->setPaciente(new Paciente(*sucesor->getPaciente()));

            // 4. Eliminamos el nodo sucesor original de su posicion en el subarbol derecho
            bool dummy = false;
            nodo->setDerecho(eliminarAux(nodo->getDerecho(), idSucesor, dummy));
        }
    }

    // Actualizamos altura
    nodo->setAltura(1 + maximo(obtenerAltura(nodo->getIzquierdo()), obtenerAltura(nodo->getDerecho())));

    // Rebalanceamos
    int balance = obtenerBalance(nodo);

    // Caso LL
    if (balance > 1 && obtenerBalance(nodo->getIzquierdo()) >= 0) {
        return rotacionDerecha(nodo);
    }

    // Caso LR
    if (balance > 1 && obtenerBalance(nodo->getIzquierdo()) < 0) {
        return rotacionIzquierdaDerecha(nodo);
    }

    // Caso RR
    if (balance < -1 && obtenerBalance(nodo->getDerecho()) <= 0) {
        return rotacionIzquierda(nodo);
    }

    // Caso RL
    if (balance < -1 && obtenerBalance(nodo->getDerecho()) > 0) {
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

bool ArbolAVL::eliminarPorId(const std::string& id) {
    bool eliminado = false;
    raiz = eliminarAux(raiz, id, eliminado);
    return eliminado;
}

Paciente* ArbolAVL::buscarPorIdAux(NodoAVL* nodo, const std::string& id, std::string& ruta) const {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (ruta.empty()) {
        ruta = nodo->getPaciente()->getId();
    } else {
        ruta += " -> " + nodo->getPaciente()->getId();
    }

    if (nodo->getPaciente()->getId() == id) {
        return nodo->getPaciente();
    }

    if (id < nodo->getPaciente()->getId()) {
        return buscarPorIdAux(nodo->getIzquierdo(), id, ruta);
    } else {
        return buscarPorIdAux(nodo->getDerecho(), id, ruta);
    }
}

Paciente* ArbolAVL::buscarPorId(const std::string& id, std::string& ruta) const {
    ruta = "";
    return buscarPorIdAux(raiz, id, ruta);
}

Paciente* ArbolAVL::buscarPorNombreAux(NodoAVL* nodo, const std::string& nombre) const {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (nodo->getPaciente()->getNombre() == nombre) {
        return nodo->getPaciente();
    }

    Paciente* encontrado = buscarPorNombreAux(nodo->getIzquierdo(), nombre);
    if (encontrado != nullptr) {
        return encontrado;
    }

    return buscarPorNombreAux(nodo->getDerecho(), nombre);
}

Paciente* ArbolAVL::buscarPorNombre(const std::string& nombre) const {
    return buscarPorNombreAux(raiz, nombre);
}

void ArbolAVL::inordenAux(NodoAVL* nodo) const {
    if (nodo != nullptr) {
        inordenAux(nodo->getIzquierdo());
        std::cout << "\n   - [" << nodo->getPaciente()->getId() << "] " 
                  << nodo->getPaciente()->getNombre()
                  << " (h: " << nodo->getAltura() 
                  << ", fb: " << obtenerBalance(nodo) << ")";
        inordenAux(nodo->getDerecho());
    }
}

void ArbolAVL::preordenAux(NodoAVL* nodo) const {
    if (nodo != nullptr) {
        std::cout << "\n   - [" << nodo->getPaciente()->getId() << "] " 
                  << nodo->getPaciente()->getNombre()
                  << " (h: " << nodo->getAltura() 
                  << ", fb: " << obtenerBalance(nodo) << ")";
        preordenAux(nodo->getIzquierdo());
        preordenAux(nodo->getDerecho());
    }
}

void ArbolAVL::postordenAux(NodoAVL* nodo) const {
    if (nodo != nullptr) {
        postordenAux(nodo->getIzquierdo());
        postordenAux(nodo->getDerecho());
        std::cout << "\n   - [" << nodo->getPaciente()->getId() << "] " 
                  << nodo->getPaciente()->getNombre()
                  << " (h: " << nodo->getAltura() 
                  << ", fb: " << obtenerBalance(nodo) << ")";
    }
}

void ArbolAVL::mostrarInorden() const {
    if (raiz == nullptr) {
        std::cout << "Sin pacientes";
    } else {
        std::cout << "Pacientes: ";
        inordenAux(raiz);
    }
    std::cout << std::endl;
}

void ArbolAVL::mostrarPreorden() const {
    if (raiz == nullptr) {
        std::cout << "Sin pacientes";
    } else {
        preordenAux(raiz);
    }
    std::cout << std::endl;
}

void ArbolAVL::mostrarPostorden() const {
    if (raiz == nullptr) {
        std::cout << "Sin pacientes";
    } else {
        postordenAux(raiz);
    }
    std::cout << std::endl;
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