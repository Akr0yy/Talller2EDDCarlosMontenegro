#pragma once
#include <string>

// Representa un nodo en el arbol binario de decision.
// puede ser un nodo interno (pregunta o condicion) o una hoja (servicio asignado)
class NodoDecision {
private:
    std::string texto;
    bool esHoja;
    NodoDecision* si; // Hijo izq (si se cumple la condicion)
    NodoDecision* no; // Hijo der (no se cumple la condicion)

public:
    // Constructor para nodos de decision (preguntas)
    NodoDecision(const std::string& pregunta);

    // Constructor para nodos hoja de servicio
    NodoDecision(const std::string& servicio, bool hoja);

    ~NodoDecision();

    bool getEsHoja() const;
    std::string getTexto() const;
    NodoDecision* getSi() const;
    NodoDecision* getNo() const;

    void setSi(NodoDecision* hijoSi);
    void setNo(NodoDecision* hijoNo);
};