#include "ArbolDecision.h"
#include <iostream>

ArbolDecision::ArbolDecision() : raiz(nullptr) {
    construirArbol();
}

ArbolDecision::~ArbolDecision() {
    delete raiz; // Libera recursivamente todos los nodos y subarboles
}

void ArbolDecision::construirArbol() {
    // 1. Nodos de decision con criterios clinicos
    NodoDecision* nUrgenciaVital = new NodoDecision("Temp > 39.0 o Dolor >= 9 o Presion >= 160");
    NodoDecision* nTrauma = new NodoDecision("Trauma == 1");
    NodoDecision* nCirugia = new NodoDecision("RequiereCirugia == 1");
    NodoDecision* nNeuro = new NodoDecision("SintomasNeurologicos == 1");
    NodoDecision* nPediatria = new NodoDecision("Edad < 14");
    NodoDecision* nCardio = new NodoDecision("TipoDolor == pecho o Presion >= 140");
    NodoDecision* nHosp = new NodoDecision("DiasHospitalizacion >= 2");

    // 2. Nodos hojas: los 8 servicios del hospital
    NodoDecision* hUrgencias = new NodoDecision("Urgencias", true);
    NodoDecision* hCirugia = new NodoDecision("Cirugia", true);
    NodoDecision* hTrauma = new NodoDecision("Traumatologia", true);
    NodoDecision* hNeuro = new NodoDecision("Neurologia", true);
    NodoDecision* hPediatria = new NodoDecision("Pediatria", true);
    NodoDecision* hCardio = new NodoDecision("Cardiologia", true);
    NodoDecision* hHospitalizacion = new NodoDecision("Hospitalizacion", true);
    NodoDecision* hMedGeneral = new NodoDecision("Medicina General", true);

    // 3. Enlace de nodos segun la jerarquia de triaje
    // Nivel 1: Urgencia vital inmediata
    nUrgenciaVital->setSi(hUrgencias);
    nUrgenciaVital->setNo(nTrauma);

    // Nivel 2: Trauma fisico agudo
    nTrauma->setSi(nCirugia);
    nTrauma->setNo(nNeuro);

    // Nivel 2.1: Intervencion quirurgica de urgencia en trauma
    nCirugia->setSi(hCirugia);
    nCirugia->setNo(hTrauma);

    // Nivel 3: Afectacion neurologica
    nNeuro->setSi(hNeuro);
    nNeuro->setNo(nPediatria);

    // Nivel 4: Edad pediatrica
    nPediatria->setSi(hPediatria);
    nPediatria->setNo(nCardio);

    // Nivel 5: Riesgo cardiovascular
    nCardio->setSi(hCardio);
    nCardio->setNo(nHosp);

    // Nivel 6: Hospitalizacion requerida vs consulta general ambulatoria
    nHosp->setSi(hHospitalizacion);
    nHosp->setNo(hMedGeneral);

    raiz = nUrgenciaVital;
}

std::string ArbolDecision::evaluar(const Paciente* paciente, std::string& rutaSalida) const {
    if (!raiz || !paciente) {
        return "Medicina General";
    }

    NodoDecision* actual = raiz;
    rutaSalida = "";

    while (actual != nullptr && !actual->getEsHoja()) {
        std::string pregunta = actual->getTexto();

        if (pregunta == "Temp > 39.0 o Dolor >= 9 o Presion >= 160") {
            if (paciente->getTemperatura() > 39.0 || paciente->getDolor() >= 9 || paciente->getPresionArterial() >= 160) {
                rutaSalida += "Urgencia Vital (Temp > 39.0 o Dolor >= 9 o Presion >= 160)";
                actual = actual->getSi();
            } else {
                rutaSalida += "Sin Urgencia Vital -> ";
                actual = actual->getNo();
            }
        } else if (pregunta == "Trauma == 1") {
            if (paciente->getTrauma() == 1) {
                rutaSalida += "Trauma == 1 -> ";
                actual = actual->getSi();
            } else {
                rutaSalida += "Trauma == 0 -> ";
                actual = actual->getNo();
            }
        } else if (pregunta == "RequiereCirugia == 1") {
            if (paciente->getRequiereCirugia() == 1) {
                rutaSalida += "RequiereCirugia == 1";
                actual = actual->getSi();
            } else {
                rutaSalida += "RequiereCirugia == 0";
                actual = actual->getNo();
            }
        } else if (pregunta == "SintomasNeurologicos == 1") {
            if (paciente->getSintomasNeurologicos() == 1) {
                rutaSalida += "Sintomas Neurologicos == 1";
                actual = actual->getSi();
            } else {
                rutaSalida += "Sintomas Neurologicos == 0 -> ";
                actual = actual->getNo();
            }
        } else if (pregunta == "Edad < 14") {
            if (paciente->getEdad() < 14) {
                rutaSalida += "Edad < 14";
                actual = actual->getSi();
            } else {
                rutaSalida += "Edad >= 14 -> ";
                actual = actual->getNo();
            }
        } else if (pregunta == "TipoDolor == pecho o Presion >= 140") {
            if (paciente->getTipoDolor() == "pecho" || paciente->getPresionArterial() >= 140) {
                rutaSalida += "Sospecha Cardiaca (TipoDolor == pecho o Presion >= 140)";
                actual = actual->getSi();
            } else {
                rutaSalida += "Sin Sospecha Cardiaca -> ";
                actual = actual->getNo();
            }
        } else if (pregunta == "DiasHospitalizacion >= 2") {
            if (paciente->getDiasHospitalizacion() >= 2) {
                rutaSalida += "DiasHospitalizacion >= 2";
                actual = actual->getSi();
            } else {
                rutaSalida += "DiasHospitalizacion < 2";
                actual = actual->getNo();
            }
        } else {
            actual = actual->getNo();
        }
    }

    if (actual != nullptr && actual->getEsHoja()) {
        return actual->getTexto();
    }

    return "Medicina General"; // Fallback ante datos incompletos
}

void ArbolDecision::preordenAux(NodoDecision* actual) const {
    if (actual == nullptr) return;
    std::cout << "[" << actual->getTexto() << "] ";
    preordenAux(actual->getSi());
    preordenAux(actual->getNo());
}

void ArbolDecision::inordenAux(NodoDecision* actual) const {
    if (actual == nullptr) return;
    inordenAux(actual->getSi());
    std::cout << "[" << actual->getTexto() << "] ";
    inordenAux(actual->getNo());
}

void ArbolDecision::postordenAux(NodoDecision* actual) const {
    if (actual == nullptr) return;
    postordenAux(actual->getSi());
    postordenAux(actual->getNo());
    std::cout << "[" << actual->getTexto() << "] ";
}

void ArbolDecision::mostrarPreorden() const {
    preordenAux(raiz);
    std::cout << std::endl;
}

void ArbolDecision::mostrarInorden() const {
    inordenAux(raiz);
    std::cout << std::endl;
}

void ArbolDecision::mostrarPostorden() const {
    postordenAux(raiz);
    std::cout << std::endl;
}