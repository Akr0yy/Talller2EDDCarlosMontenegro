#include "Paciente.h"
#include <iostream>

Paciente::Paciente(const std::string& id, const std::string& nombre, int edad,
                   double temperatura, int presionArterial, int dolor,
                   const std::string& tipoDolor, int trauma,
                   int sintomasNeurologicos, int requiereCirugia,
                   int diasHospitalizacion)
    : Persona(nombre, edad),
      id(id),
      temperatura(temperatura),
      presionArterial(presionArterial),
      dolor(dolor),
      tipoDolor(tipoDolor),
      trauma(trauma),
      sintomasNeurologicos(sintomasNeurologicos),
      requiereCirugia(requiereCirugia),
      diasHospitalizacion(diasHospitalizacion),
      servicioAsignado("Sin asignar") {}

Paciente::Paciente(const Paciente& otro)
    : Persona(otro),
      id(otro.id),
      temperatura(otro.temperatura),
      presionArterial(otro.presionArterial),
      dolor(otro.dolor),
      tipoDolor(otro.tipoDolor),
      trauma(otro.trauma),
      sintomasNeurologicos(otro.sintomasNeurologicos),
      requiereCirugia(otro.requiereCirugia),
      diasHospitalizacion(otro.diasHospitalizacion),
      servicioAsignado(otro.servicioAsignado) {}

Paciente::~Paciente() {}

std::string Paciente::getId() const {
    return id;
}

double Paciente::getTemperatura() const {
    return temperatura;
}

int Paciente::getPresionArterial() const {
    return presionArterial;
}

int Paciente::getDolor() const {
    return dolor;
}

std::string Paciente::getTipoDolor() const {
    return tipoDolor;
}

int Paciente::getTrauma() const {
    return trauma;
}

int Paciente::getSintomasNeurologicos() const {
    return sintomasNeurologicos;
}

int Paciente::getRequiereCirugia() const {
    return requiereCirugia;
}

int Paciente::getDiasHospitalizacion() const {
    return diasHospitalizacion;
}

std::string Paciente::getServicioAsignado() const {
    return servicioAsignado;
}

std::string Paciente::getServicioDestino() const {
    return servicioAsignado;
}

void Paciente::setServicioAsignado(const std::string& servicio) {
    servicioAsignado = servicio;
}

void Paciente::mostrarInfo() const {
    std::cout << id << " - " << nombre << " (" << edad << ")";
    if (servicioAsignado != "Sin asignar") {
        std::cout << " -> " << servicioAsignado;
    }
    std::cout << std::endl;
}

void Paciente::mostrarInfoClinica() const {
    std::cout << "ID: " << id << "\n"
              << "Nombre: " << nombre << "\n"
              << "Edad: " << edad << "\n"
              << "Temperatura: " << temperatura << "\n"
              << "Dolor: " << dolor << "\n"
              << "TipoDolor: " << tipoDolor << "\n"
              << "Trauma: " << trauma << "\n"
              << "Sintomas Neurologicos: " << sintomasNeurologicos << "\n"
              << "Requiere Cirugia: " << requiereCirugia << "\n"
              << "DiasHospitalizacion: " << diasHospitalizacion << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Paciente& p) {
    os << p.id << " - " << p.nombre;
    return os;
}