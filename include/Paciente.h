#pragma once
#include "Persona.h"
#include <string>
#include <ostream>

class Paciente : public Persona {
private:
    std::string id;
    double temperatura;
    int presionArterial;
    int dolor;
    std::string tipoDolor;
    int trauma;
    int sintomasNeurologicos;
    int requiereCirugia;
    int diasHospitalizacion;
    std::string servicioAsignado;

public:
    Paciente(const std::string& id, const std::string& nombre, int edad,
             double temperatura, int presionArterial, int dolor,
             const std::string& tipoDolor, int trauma,
             int sintomasNeurologicos, int requiereCirugia,
             int diasHospitalizacion);

    // constructor de copia (delega en el de Persona y copia lo propio)
    Paciente(const Paciente& otro);

    virtual ~Paciente();

    std::string getId() const;
    double getTemperatura() const;
    int getPresionArterial() const;
    int getDolor() const;
    std::string getTipoDolor() const;
    int getTrauma() const;
    int getSintomasNeurologicos() const;
    int getRequiereCirugia() const;
    int getDiasHospitalizacion() const;
    std::string getServicioAsignado() const;

    void setServicioAsignado(const std::string& servicio);

    void mostrarInfo() const;
    void mostrarInfoClinica() const;

    // sobrecarga de operador de salida, permite hacer "cout << paciente"
    // para un formato corto (id - nombre), util para listar la cola.
    friend std::ostream& operator<<(std::ostream& os, const Paciente& p);
};