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

    // Caso Izquierda - Izquierda
    if (balance > 1 && paciente->getId() < nodo->getIzquierdo()->getPaciente()->getId()) {
        return rotacionDerecha(nodo);
    }

    // Caso Derecha - Derecha
    if (balance < -1 && paciente->getId() > nodo->getDerecho()->getPaciente()->getId()) {
        return rotacionIzquierda(nodo);
    }

    // Caso Izquierda - Derecha
    if (balance > 1 && paciente->getId() > nodo->getIzquierdo()->getPaciente()->getId()) {
        nodo->setIzquierdo(rotacionIzquierda(nodo->getIzquierdo()));
        return rotacionDerecha(nodo);
    }

    // Caso Derecha - Izquierda
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

NodoAVL* ArbolAVL::obtenerNodoConValorMinimo(NodoAVL* nodo) const {
    NodoAVL* actual = nodo;
    while (actual->getIzquierdo() != nullptr) {
        actual = actual->getIzquierdo();
    }
    return actual;
}

NodoAVL* ArbolAVL::eliminarAux(NodoAVL* nodo, const std::string& id, bool& eliminado) {
    if (nodo == nullptr) {
        return nodo;
    }

    if (id < nodo->getPaciente()->getId()) {
        nodo->setIzquierdo(eliminarAux(nodo->getIzquierdo(), id, eliminado));
    } else if (id > nodo->getPaciente()->getId()) {
        nodo->setDerecho(eliminarAux(nodo->getDerecho(), id, eliminado));
    } else {
        // Encontramos el nodo a eliminar
        eliminado = true;

        // Caso 1 o 2: un solo hijo o sin hijos
        if (nodo->getIzquierdo() == nullptr || nodo->getDerecho() == nullptr) {
            NodoAVL* temporal = nodo->getIzquierdo() ? nodo->getIzquierdo() : nodo->getDerecho();

            if (temporal == nullptr) {
                // Sin hijos
                temporal = nodo;
                nodo = nullptr;
            } else {
                // Un hijo: copiamos el contenido del hijo
                *nodo = *temporal;
            }

            delete temporal->getPaciente();
            delete temporal;
        } else {
            // Caso 3: con dos hijos. Obtenemos el sucesor inorden (el menor del subarbol derecho)
            NodoAVL* temporal = obtenerNodoConValorMinimo(nodo->getDerecho());

            // Liberamos el paciente actual y transferimos el puntero del sucesor
            delete nodo->getPaciente();
            nodo->setPaciente(temporal->getPaciente());

            // Evitamos que destruirArbol borre dos veces el mismo puntero al eliminar la hoja duplicada
            bool dummy = false;
            nodo->setDerecho(eliminarAux(nodo->getDerecho(), temporal->getPaciente()->getId(), dummy));
        }
    }

    if (nodo == nullptr) {
        return nodo;
    }

    // Actualizar altura
    nodo->setAltura(1 + maximo(obtenerAltura(nodo->getIzquierdo()), obtenerAltura(nodo->getDerecho())));

    // Rebalancear
    int balance = obtenerBalance(nodo);

    // Caso Izquierda - Izquierda
    if (balance > 1 && obtenerBalance(nodo->getIzquierdo()) >= 0) {
        return rotacionDerecha(nodo);
    }

    // Caso Izquierda - Derecha
    if (balance > 1 && obtenerBalance(nodo->getIzquierdo()) < 0) {
        nodo->setIzquierdo(rotacionIzquierda(nodo->getIzquierdo()));
        return rotacionDerecha(nodo);
    }

    // Caso Derecha - Derecha
    if (balance < -1 && obtenerBalance(nodo->getDerecho()) <= 0) {
        return rotacionIzquierda(nodo);
    }

    // Caso Derecha - Izquierda
    if (balance < -1 && obtenerBalance(nodo->getDerecho()) > 0) {
        nodo->setDerecho(rotacionDerecha(nodo->getDerecho()));
        return rotacionIzquierda(nodo);
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
        std::cout << nodo->getPaciente()->getNombre() << " (" << nodo->getPaciente()->getId() << ") ";
        inordenAux(nodo->getDerecho());
    }
}

void ArbolAVL::preordenAux(NodoAVL* nodo) const {
    if (nodo != nullptr) {
        std::cout << nodo->getPaciente()->getNombre() << " (" << nodo->getPaciente()->getId() << ") ";
        preordenAux(nodo->getIzquierdo());
        preordenAux(nodo->getDerecho());
    }
}

void ArbolAVL::postordenAux(NodoAVL* nodo) const {
    if (nodo != nullptr) {
        postordenAux(nodo->getIzquierdo());
        postordenAux(nodo->getDerecho());
        std::cout << nodo->getPaciente()->getNombre() << " (" << nodo->getPaciente()->getId() << ") ";
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