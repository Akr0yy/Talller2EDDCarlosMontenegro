#include <iostream>
#include <string>
#include <limits>
#include "ColaPacientes.h"
#include "CargadorArchivo.h"
#include "Hospital.h"
#include "PilaHistorial.h"

// Lee un entero desde consola de forma segura. Si el usuario ingresa
// algo que no es un numero, limpia el estado de cin y lanza una
// excepcion para que quien llama decida como reaccionar.
int leerEntero() {
    int valor;
    std::cin >> valor;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::string("Debe ingresar un numero valido.");
    }

    return valor;
}

void mostrarMenu() {
    std::cout << "\n=== HOSPITAL MARMAJA ===" << std::endl;
    std::cout << "1. Atender pacientes" << std::endl;
    std::cout << "2. Ver departamento" << std::endl;
    std::cout << "3. Revisar historial de atencion" << std::endl;
    std::cout << "4. Buscar paciente (ID o Nombre)" << std::endl;
    std::cout << "5. Recorridos de Arbol de Decision" << std::endl;
    std::cout << "6. Salir" << std::endl;
    std::cout << "Seleccionar opcion: ";
}

void atenderPacientes(ColaPacientes& cola, Hospital& hospital, PilaHistorial& historial) {
    if (cola.estaVacia()) {
        std::cout << "No hay pacientes en espera." << std::endl;
        return;
    }

    std::cout << "\n=== PACIENTES EN ESPERA ===" << std::endl;
    cola.mostrar();

    std::cout << "Indique la cantidad de pacientes a atender: ";
    int cantidad;
    try {
        cantidad = leerEntero();
    } catch (const std::string& mensaje) {
        std::cout << mensaje << std::endl;
        return;
    }

    if (cantidad <= 0) {
        std::cout << "La cantidad debe ser mayor a 0." << std::endl;
        return;
    }

    std::cout << "\n=== ATENDIENDO PACIENTES ===" << std::endl;
    for (int i = 0; i < cantidad && !cola.estaVacia(); i++) {
        Paciente* paciente = cola.desencolar();

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Evaluando paciente:" << std::endl;
        paciente->mostrarInfoClinica();

        std::string rutaDecision;
        try {
            std::string servicioAsignado = hospital.procesarYDerivarPaciente(paciente, rutaDecision);

            std::cout << "\nRuta de evaluacion: " << rutaDecision << std::endl;
            std::cout << "Servicio asignado: " << servicioAsignado << std::endl;

            historial.agregarAtencion(new RegistroAtencion(
                paciente->getNombre(), paciente->getEdad(), servicioAsignado
            ));
        } catch (const std::string& mensaje) {
            std::cout << "No se pudo derivar al paciente: " << mensaje << std::endl;
            delete paciente;
        }
    }
}

void verDepartamento(Hospital& hospital) {
    std::cout << "\n=== DEPARTAMENTOS/SERVICIOS ===" << std::endl;
    hospital.mostrarServicios();

    std::cout << "Seleccionar opcion: ";
    int opcion;
    try {
        opcion = leerEntero();
        Servicio* servicio = hospital.obtenerPorIndice(opcion);

        std::string nombreMayus = servicio->getNombre();
        for (char& c : nombreMayus) c = toupper(c);

        std::cout << "\n=== ESTADO " << nombreMayus << " ===" << std::endl;
        servicio->mostrarPacientes();
        std::cout << "Total de pacientes: " << servicio->getCantidad() << std::endl;
        std::cout << "Altura arbol AVL: " << servicio->getAlturaAVL() << std::endl;
        std::cout << "Factor de balance raiz: " << servicio->getBalanceAVL() << std::endl;
    } catch (const std::string& mensaje) {
        std::cout << mensaje << std::endl;
    }
}

void revisarHistorial(PilaHistorial& historial) {
    std::cout << "\n=== HISTORIAL DE ULTIMAS ATENCIONES DEL HOSPITAL ===" << std::endl;
    historial.mostrarHistorial();
}

void buscarPaciente(ColaPacientes& cola, Hospital& hospital) {
    std::cout << "\n1. Buscar por ID" << std::endl;
    std::cout << "2. Buscar por Nombre" << std::endl;
    std::cout << "Seleccione modalidad: ";
    
    int tipoBusqueda;
    try {
        tipoBusqueda = leerEntero();
    } catch (const std::string& mensaje) {
        std::cout << mensaje << std::endl;
        return;
    }

    if (tipoBusqueda == 1) {
        std::cout << "Ingrese el ID del paciente a buscar: ";
        std::string id;
        std::cin >> id;

        Paciente* enCola = cola.buscarPorId(id);
        if (enCola != nullptr) {
            std::cout << "\n=== PACIENTE ENCONTRADO (en espera) ===" << std::endl;
            enCola->mostrarInfo();
            return;
        }

        std::string departamento;
        std::string ruta;
        Paciente* enServicio = hospital.buscarPacientePorId(id, departamento, ruta);
        if (enServicio != nullptr) {
            std::cout << "\n=== PACIENTE ENCONTRADO ===" << std::endl;
            std::cout << "Departamento: " << departamento << std::endl;
            enServicio->mostrarInfo();
            std::cout << "Ruta de busqueda AVL: " << ruta << std::endl;
            return;
        }

        std::cout << "No se encontro ningun paciente con ID " << id << "." << std::endl;

    } else if (tipoBusqueda == 2) {
        std::cout << "Ingrese el nombre del paciente a buscar: ";
        std::string nombre;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, nombre);

        std::string departamento;
        Paciente* enServicio = hospital.buscarPacientePorNombre(nombre, departamento);
        if (enServicio != nullptr) {
            std::cout << "\n=== PACIENTE ENCONTRADO ===" << std::endl;
            std::cout << "Departamento: " << departamento << std::endl;
            enServicio->mostrarInfo();
            return;
        }

        std::cout << "No se encontro ningun paciente con nombre '" << nombre << "'." << std::endl;
    } else {
        std::cout << "Opcion invalida." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string rutaArchivo = (argc > 1) ? argv[1] : "data/pacientes.txt";

    ColaPacientes cola;
    CargadorArchivo::cargarPacientes(rutaArchivo, cola);

    Hospital hospital;
    PilaHistorial historial;

    bool salir = false;
    while (!salir) {
        mostrarMenu();

        int opcion;
        try {
            opcion = leerEntero();
        } catch (const std::string& mensaje) {
            std::cout << mensaje << std::endl;
            continue;
        }

        switch (opcion) {
            case 1:
                atenderPacientes(cola, hospital, historial);
                break;
            case 2:
                verDepartamento(hospital);
                break;
            case 3:
                revisarHistorial(historial);
                break;
            case 4:
                buscarPaciente(cola, hospital);
                break;
            case 5:
                hospital.mostrarRecorridosArbolDecision();
                break;
            case 6:
                std::cout << "Hasta luego :D." << std::endl;
                salir = true;
                break;
            default:
                std::cout << "Opcion invalida." << std::endl;
        }
    }

    return 0;
}