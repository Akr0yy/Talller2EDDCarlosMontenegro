# Hospital Marmaja — Taller 1 Estructura de Datos

## Descripción del Proyecto

El hospital Marmaja es un recinto de salud con un flujo constante de pacientes y una larga fila de espera. Este proyecto implementa un sistema básico de gestión de pacientes que recibe los ingresos desde un archivo de texto, los procesa respetando estrictamente su orden de llegada, y los deriva al departamento correspondiente según su condición.

El sistema modela el hospital como una lista enlazada de servicios (Urgencias, Medicina General, Cardiología, Neurología, Traumatología, Cirugía, Pediatría y Hospitalización), donde cada servicio mantiene a su vez su propia lista enlazada de pacientes. Los pacientes ingresan mediante una Queue (cola FIFO) que respeta el orden de llegada, y cada atención realizada queda registrada en una Stack (pila LIFO) que funciona como historial del hospital.

Todas las estructuras (Queue, Linked List y Stack) están implementadas manualmente mediante nodos y punteros, sin utilizar contenedores de la STL, e incorporan programación orientada a objetos (herencia, constructor de copia, sobrecarga de operadores) y aritmética de punteros aplicada al parseo del archivo de entrada.

## Integrantes

- Nombre: Carlos Montenegro Pérez
- RUT: 22.154.893-0
- Usuario GitHub: Akr0yy

## Estructura del proyecto
El código está organizado en `include/` (cabeceras `.h`) y `src/` (implementación `.cpp`), separando la declaración de cada clase de su lógica:

* **Entidades del hospital** (agrupación lógica, no una carpeta física — todas viven en `include/`/`src/` junto al resto)
  * **Persona**: clase base que modela a cualquier persona del sistema (id, nombre, edad), con constructor de copia, destructor virtual y sobrecarga de `operator==`.
  * **Paciente**: clase derivada de `Persona`. Agrega el servicio de destino y el estado de atención; sobrescribe `mostrarInfo()` y sobrecarga `operator<<` para su impresión en la cola.
  * **RegistroAtencion**: representa un evento del historial (nombre, edad, departamento), con el formato exacto que exige el enunciado.

* **Queue de pacientes en espera**
  * **NodoPaciente**: nodo manual (puntero a `Paciente` + puntero al siguiente).
  * **ColaPacientes**: Queue manual FIFO. Permite encolar, desencolar (lanza excepción si está vacía), buscar por id y mostrar la fila completa.
  * **CargadorArchivo**: lee el archivo de texto, valida cada línea (formato, campos vacíos, edad numérica, duplicados) y encola los pacientes válidos. Usa aritmética de punteros para separar los campos de cada línea sin `stringstream`.

* **Lista enlazada principal del Hospital**
  * **NodoPacienteServicio**: nodo para la lista de pacientes de cada servicio.
  * **Servicio**: representa un departamento del hospital; mantiene su propia lista enlazada de pacientes (agregar, buscar por id, mostrar).
  * **NodoServicio**: nodo de la lista principal, apunta a un `Servicio`.
  * **Hospital**: lista enlazada de los 8 servicios fijos. Deriva pacientes al servicio correspondiente (lanza excepción si el servicio no es válido), busca pacientes en todos los servicios y permite seleccionar un servicio por índice.

* **Stack de historial**
  * **NodoHistorial**: nodo para la pila de historial.
  * **PilaHistorial**: Stack manual LIFO de `RegistroAtencion`; agrega y muestra el historial completo, del más reciente al más antiguo.

* **Main.cpp**: capa de interfaz. Implementa el menú de consola (atender pacientes, ver departamento, revisar historial, buscar paciente, salir), lee y valida la entrada del usuario, y coordina el flujo completo: Archivo → Cola → Atención → Servicio → Historial.


```
hospital-marmaja/
├── include/     # Archivos .h (declaraciones de clases)
├── src/         # Archivos .cpp (implementaciones) + main.cpp
├── data/        # pacientes.txt (ejemplo)
└── README.md
```

## Compilación

```bash
g++ -std=c++17 -Wall -Iinclude src/*.cpp -o hospital_marmaja
```

## Ejecución

```bash
./hospital_marmaja data/pacientes.txt
```

## Formato del archivo de entrada

Cada línea representa un paciente, separado por `;`:

```
ID;Nombre;Edad;Servicio
```

Ejemplo:

```
001;Juan Perez;25;Cardiologia
002;Maria Soto;67;Urgencias
```
