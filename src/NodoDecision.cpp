#include "NodoDecision.h"

NodoDecision::NodoDecision(const std::string& pregunta)
    : texto(pregunta), esHoja(false), si(nullptr), no(nullptr) {}

NodoDecision::NodoDecision(const std::string& servicio, bool hoja)
    : texto(servicio), esHoja(hoja), si(nullptr), no(nullptr) {}

NodoDecision::~NodoDecision() {
    delete si;
    delete no;
}

bool NodoDecision::getEsHoja() const {
    return esHoja;
}

std::string NodoDecision::getTexto() const {
    return texto;
}

NodoDecision* NodoDecision::getSi() const {
    return si;
}

NodoDecision* NodoDecision::getNo() const {
    return no;
}

void NodoDecision::setSi(NodoDecision* hijoSi) {
    si = hijoSi;
}

void NodoDecision::setNo(NodoDecision* hijoNo) {
    no = hijoNo;
}