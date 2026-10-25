#include "CargadorArchivo.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cctype>

// Convierte un texto a minusculas para comparaciones insensibles a mayusculas
static std::string aMinusculas(const std::string& str) {
    std::string resultado = str;
    for (char& c : resultado) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return resultado;
}

bool CargadorArchivo::parsearLinea(const std::string& linea, std::string campos[11]) {
    const char* actual = linea.c_str();
    int indiceCampo = 0;

    while (*actual != '\0') {
        if (indiceCampo >= 11) {
            // Rechaza si la linea contiene mas de 11 campos
            return false;
        }

        const char* inicioCampo = actual;
        while (*actual != ';' && *actual != '\0') {
            actual++;
        }

        campos[indiceCampo] = std::string(inicioCampo, actual - inicioCampo);
        indiceCampo++;

        if (*actual == ';') {
            actual++;
            // Caso de borde: linea termina con ';'
            if (*actual == '\0' && indiceCampo < 11) {
                campos[indiceCampo] = "";
                indiceCampo++;
            }
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

        // Limpieza de retorno de carro de Windows (\r\n)
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }

        std::string campos[11];
        if (!parsearLinea(linea, campos)) {
            std::cerr << "Linea " << numeroLinea << " invalida (cantidad de campos incorrecta): " << linea << std::endl;
            rechazados++;
            continue;
        }

        const std::string& id = campos[0];
        const std::string& nombre = campos[1];
        const std::string& edadTexto = campos[2];
        const std::string& tempTexto = campos[3];
        const std::string& presionTexto = campos[4];
        const std::string& dolorTexto = campos[5];
        std::string tipoDolor = aMinusculas(campos[6]);
        const std::string& traumaTexto = campos[7];
        const std::string& sintNeuTexto = campos[8];
        const std::string& reqCirTexto = campos[9];
        const std::string& diasHospTexto = campos[10];

        // Validacion de campos vacios
        if (id.empty() || nombre.empty() || tipoDolor.empty()) {
            std::cerr << "Linea " << numeroLinea << " invalida (campos obligatorios vacios): " << linea << std::endl;
            rechazados++;
            continue;
        }

        // Validacion de formato numerico basico
        if (!esEnteroValido(edadTexto) || !esDecimalValido(tempTexto) || !esEnteroValido(presionTexto) ||
            !esEnteroValido(dolorTexto) || !esEnteroValido(traumaTexto) || !esEnteroValido(sintNeuTexto) ||
            !esEnteroValido(reqCirTexto) || !esEnteroValido(diasHospTexto)) {
            std::cerr << "Linea " << numeroLinea << " invalida (datos numericos invalidos): " << linea << std::endl;
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

        // Validacion de rangos clinicos y limites permitidos
        if (edad < 0 || edad > 130) {
            std::cerr << "Linea " << numeroLinea << " invalida (edad fuera de rango clinico [0-130]): " << linea << std::endl;
            rechazados++;
            continue;
        }

        if (temperatura < 30.0 || temperatura > 45.0) {
            std::cerr << "Linea " << numeroLinea << " invalida (temperatura corporal no fisiologica [30-45]): " << linea << std::endl;
            rechazados++;
            continue;
        }

        if (dolor < 0 || dolor > 10) {
            std::cerr << "Linea " << numeroLinea << " invalida (escala de dolor debe estar entre 0 y 10): " << linea << std::endl;
            rechazados++;
            continue;
        }

        if ((trauma != 0 && trauma != 1) || 
            (sintNeu != 0 && sintNeu != 1) || 
            (reqCir != 0 && reqCir != 1)) {
            std::cerr << "Linea " << numeroLinea << " invalida (indicadores binarios deben ser estrictamente 0 o 1): " << linea << std::endl;
            rechazados++;
            continue;
        }

        if (tipoDolor != "pecho" && tipoDolor != "cabeza" && tipoDolor != "abdomen" && 
            tipoDolor != "extremidad" && tipoDolor != "otro") {
            std::cerr << "Linea " << numeroLinea << " invalida (tipo de dolor no reconocido): " << linea << std::endl;
            rechazados++;
            continue;
        }

        // Verificacion de duplicado en cola
        if (cola.existeId(id)) {
            std::cerr << "Linea " << numeroLinea << ": paciente duplicado, id " << id << " ya existe." << std::endl;
            rechazados++;
            continue;
        }

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