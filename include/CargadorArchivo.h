#pragma once
#include "ColaPacientes.h"
#include <string>

// Lee el archivo de texto de pacientes y los encola en ColaPacientes,
// validando el formato de cada linea
class CargadorArchivo {
public:
    static bool cargarPacientes(const std::string& rutaArchivo, ColaPacientes& cola);

private:
    // Divide la linea usando aritmetica de punteros.
    static bool parsearLinea(const std::string& linea, std::string campos[11]);
    static bool esEnteroValido(const std::string& texto);
    static bool esDecimalValido(const std::string& texto);
};