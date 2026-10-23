#include "CargadorArchivo.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

bool CargadorArchivo::parsearLinea(const std::string& linea, std::string campos[11]) {
    const char* actual = linea.c_str();
    int indiceCampo = 0;

    while (*actual != '\0' && indiceCampo < 11) {
        const char* inicioCampo = actual;

        while (*actual != ';' && *actual != '\0') {
            actual++; // aritmetica de punteros: avanza caracter a caracter
        }

        campos[indiceCampo] = std::string(inicioCampo, actual - inicioCampo);
        indiceCampo++;

        if (*actual == ';') {
            actual++;
        }
    }

    return indiceCampo == 11;
}

bool CargadorArchivo::esEnteroValido(const std::string& texto) {
    if (texto.empty()) return false;
    for (char c : texto) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

bool CargadorArchivo::esDecimalValido(const std::string& texto) {
    if (texto.empty()) return false;
    bool puntoEncontrado = false;
    for (char c : texto) {
        if (c == '.') {
            if (puntoEncontrado) return false;
            puntoEncontrado = true;
        } else if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

bool CargadorArchivo::cargarPacientes(const std::string& rutaArchivo, ColaPacientes& cola) {
    std::ifstream archivo(rutaArchivo);

    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo '" << rutaArchivo << "'." << std::endl;
        return false;
    }

    std::string linea;
    int numeroLinea = 0;
    int cargados = 0;
    int rechazados = 0;

    while (std::getline(archivo, linea)) {
        numeroLinea++;

        if (linea.empty()) {
            continue;
        }

        std::string campos[11];
        if (!parsearLinea(linea, campos)) {
            std::cerr << "Linea " << numeroLinea << " invalida (formato incorrecto): " << linea << std::endl;
            rechazados++;
            continue;
        }

        const std::string& id = campos[0];
        const std::string& nombre = campos[1];
        const std::string& edadTexto = campos[2];
        const std::string& tempTexto = campos[3];
        const std::string& presionTexto = campos[4];
        const std::string& dolorTexto = campos[5];
        const std::string& tipoDolor = campos[6];
        const std::string& traumaTexto = campos[7];
        const std::string& sintNeuTexto = campos[8];
        const std::string& reqCirTexto = campos[9];
        const std::string& diasHospTexto = campos[10];

        if (id.empty() || nombre.empty() || tipoDolor.empty()) {
            std::cerr << "Linea " << numeroLinea << " invalida (campos vacios): " << linea << std::endl;
            rechazados++;
            continue;
        }

        if (!esEnteroValido(edadTexto) || !esDecimalValido(tempTexto) || !esEnteroValido(presionTexto) ||
            !esEnteroValido(dolorTexto) || !esEnteroValido(traumaTexto) || !esEnteroValido(sintNeuTexto) ||
            !esEnteroValido(reqCirTexto) || !esEnteroValido(diasHospTexto)) {
            std::cerr << "Linea " << numeroLinea << " invalida (datos numericos invalidos): " << linea << std::endl;
            rechazados++;
            continue;
        }

        if (cola.existeId(id)) {
            std::cerr << "Linea " << numeroLinea << ": paciente duplicado, id " << id << " ya existe." << std::endl;
            rechazados++;
            continue;
        }

        int edad = std::atoi(edadTexto.c_str());
        double temperatura = std::atof(tempTexto.c_str());
        int presion = std::atoi(presionTexto.c_str());
        int dolor = std::atoi(dolorTexto.c_str());
        int trauma = std::atoi(traumaTexto.c_str());
        int sintNeu = std::atoi(sintNeuTexto.c_str());
        int reqCir = std::atoi(reqCirTexto.c_str());
        int diasHosp = std::atoi(diasHospTexto.c_str());

        Paciente* nuevoPaciente = new Paciente(id, nombre, edad, temperatura, presion, dolor,
                                               tipoDolor, trauma, sintNeu, reqCir, diasHosp);
        cola.encolar(nuevoPaciente);
        cargados++;
    }

    archivo.close();

    std::cout << "Carga finalizada: " << cargados << " pacientes cargados, "
              << rechazados << " lineas rechazadas." << std::endl;

    return true;
}