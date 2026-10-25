# Hospital Marmaja — Taller 2 Estructura de Datos

## Integrantes

- Nombre: Carlos Montenegro Pérez
- RUT: 22.154.893-0
- Usuario GitHub: Akr0yy
- Carrera: Ingeniería Civil en Informática (ICCI)
- Paralelo: C1

## Descripción

Extensión del sistema del Taller 1. El servicio del paciente ya no viene en el archivo: un **árbol de decisión** lo determina a partir de las características del paciente, y luego el paciente se guarda en el **árbol AVL** de ese servicio, ordenado por ID.

```
Archivo → Cola → Árbol de decisión → Servicio → AVL
```

Los servicios son los mismos del Taller 1: Urgencias, Medicina General, Cardiología, Neurología, Traumatología, Cirugía, Pediatría y Hospitalización. La Queue, el árbol de decisión, el AVL, la lista de servicios y la Stack del historial están implementados manualmente con nodos y punteros, sin contenedores de la STL.

## Compilación y ejecución

Probado en un entorno Codespace

```bash
# Con Makefile (genera el ejecutable "hospital")
make
./hospital data/pacientes.txt

# Sin Makefile
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o hospital
./hospital data/pacientes.txt
```

Si no se indica archivo, usa `data/pacientes.txt`. `make clean` borra lo generado.

## Archivo de entrada

Cada línea es un paciente con 11 campos separados por `;`:

```
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
```

| Campo | Significado | Valores válidos |
|-------|-------------|-----------------|
| ID | Identificador único, 3 dígitos (`001`, `002`, ...) | texto no vacío |
| Nombre | Nombre del paciente | no vacío |
| Edad | Años | 0 a 130 |
| Temperatura | °C, con punto decimal | 30 a 45 |
| PresionArterial | Presión sistólica en mmHg (definición del grupo) | 40 a 260 |
| Dolor | Escala de dolor | 0 a 10 |
| TipoDolor | Tipo de dolor | `pecho`, `cabeza`, `abdomen`, `extremidad`, `otro` |
| Trauma | 1 si existe, 0 si no | 0 o 1 |
| SintomasNeurologicos | 1 si existen, 0 si no | 0 o 1 |
| RequiereCirugia | 1 si requiere, 0 si no | 0 o 1 |
| DiasHospitalizacion | Días estimados | 0 a 365 |

Ejemplo: `010;Carlos Gomez;45;39.5;165;9;cabeza;0;0;0;0`

## Árbol de decisión

Es un árbol binario manual (`NodoDecision`): cada nodo interno hace una pregunta (`Sí` / `No`) y cada hoja es un servicio. Tiene 7 nodos internos, 8 hojas (una por servicio) y usa 9 atributos del paciente.

```
Temp > 39.0 o Dolor >= 9 o Presion >= 160 ?
|-- Si: Urgencias
|-- No: Trauma == 1 ?
    |-- Si: RequiereCirugia == 1 ?
    |   |-- Si: Cirugia
    |   |-- No: Traumatologia
    |-- No: SintomasNeurologicos == 1 ?
        |-- Si: Neurologia
        |-- No: Edad < 14 ?
            |-- Si: Pediatria
            |-- No: TipoDolor == pecho o Presion >= 140 ?
                |-- Si: Cardiologia
                |-- No: DiasHospitalizacion >= 2 ?
                    |-- Si: Hospitalizacion
                    |-- No: Medicina General
```

### Justificación

Las preguntas siguen un orden de triaje: primero el riesgo vital, luego las causas agudas, después la edad y el riesgo cardíaco, y al final hospitalización o consulta general.

| Decisión | Justificación |
|----------|---------------|
| `Temp > 39.0` o `Dolor >= 9` o `Presion >= 160` → Urgencias | Fiebre muy alta, dolor intenso o presión muy elevada son riesgo inmediato; se revisan primero para no retrasar la atención. Basta con que se cumpla una. |
| `Trauma == 1` | Un trauma agudo va a la rama traumatológica. |
| `RequiereCirugia == 1` (solo dentro de trauma) → Cirugía, si no → Traumatología | Un trauma que necesita intervención va directo a Cirugía; si no, lo ve Traumatología. |
| `SintomasNeurologicos == 1` → Neurología | Los síntomas neurológicos requieren la especialidad sin importar el resto. |
| `Edad < 14` → Pediatría | Los menores de 14 años sin urgencia, trauma ni síntomas neurológicos se atienden en Pediatría. |
| `TipoDolor == pecho` o `Presion >= 140` → Cardiología | Dolor de pecho o presión alta (bajo el umbral de urgencia) hacen sospechar un problema cardíaco. |
| `DiasHospitalizacion >= 2` → Hospitalización, si no → Medicina General | Con 2 o más días de internación estimados se hospitaliza; el resto es consulta general. |

### Empates, límites y datos inválidos

- **Empates:** las preguntas se evalúan siempre en el orden del diagrama y gana la primera que se cumple. Por ejemplo, un niño con fiebre de 40 °C va a Urgencias (se revisa antes que la edad).
- **Límites:** `Temp > 39.0` es estricto (39.0 no es urgencia, 39.1 sí). Los demás usan `>=` o `<` (14 años ya no es Pediatría; 2 días ya es Hospitalización).
- **Datos inválidos o insuficientes:** la línea se rechaza al cargar el archivo y el paciente nunca entra a la cola. Se rechazan las líneas con cantidad de campos incorrecta, campos vacíos, valores no numéricos o negativos, valores fuera de los rangos de la tabla anterior, tipo de dolor no reconocido (no distingue mayúsculas) y pacientes con ID duplicado. El programa informa el número de línea y el motivo.
- **Respaldo del árbol:** si el árbol recibe un paciente nulo, retorna Medicina General.

### Casos de prueba (con `data/pacientes.txt`)

| Paciente | Característica principal | Servicio |
|----------|--------------------------|----------|
| Carlos Gomez (010) | Temperatura 39.5, dolor 9 | Urgencias |
| Claudio Bravo (030) | Temperatura 39.8, presión 170 | Urgencias |
| Marta Fernandez (020) | Trauma y requiere cirugía | Cirugía |
| Roberto Sanchez (005) | Trauma sin cirugía | Traumatología |
| Elena Morales (015) | Síntomas neurológicos | Neurología |
| Lucia Valenzuela (002) | 8 años | Pediatría |
| Fernando Torres (008) | Dolor de pecho | Cardiología |
| Andrea Castro (025) | 4 días de hospitalización | Hospitalización |
| Diego Ramirez (018) | Sin ningún criterio especial | Medicina General |

Al atender, el programa muestra la ruta de decisiones y el servicio asignado. Los recorridos preorden, inorden y postorden del árbol están en la opción 6 del menú.

## Árboles AVL por servicio

Cada servicio tiene su propio AVL (`ArbolAVL`) y la clave de ordenamiento es el **ID**. El ID se compara como texto, por lo que se asume que todos los IDs tienen el mismo formato de 3 dígitos.

- **Insertar:** balancea al volver de la recursión; un ID repetido no se inserta.
- **Rotaciones:** simples (`rotacionDerecha`, `rotacionIzquierda`) y dobles (`rotacionIzquierdaDerecha`, `rotacionDerechaIzquierda`).
- **Buscar:** por ID (muestra la ruta de nodos visitados) o por nombre.
- **Eliminar por ID:** contempla nodo hoja, un hijo y dos hijos (reemplazo por el sucesor inorden), y rebalancea.
- **Recorridos:** inorden, preorden y postorden, mostrando altura y factor de balance de cada nodo.
- **Altura y balance:** una hoja tiene altura 1 y el factor de balance es `altura(izq) - altura(der)`, siempre entre -1 y 1.

## Menú

| Opción | Acción |
|--------|--------|
| 1. Atender pacientes | Retira de la Queue en orden FIFO, evalúa con el árbol de decisión, muestra la ruta y el servicio, inserta en el AVL y registra en el historial |
| 2. Ver departamento | Muestra los pacientes de un servicio, su altura, el balance de la raíz y los tres recorridos del AVL |
| 3. Revisar historial de atención | Muestra las atenciones, de la más reciente a la más antigua |
| 4. Buscar paciente | Busca por ID o nombre en la cola de espera y en los AVL |
| 5. Dar de alta paciente | Elimina un paciente por ID de su AVL |
| 6. Recorridos del árbol de decisión | Preorden, inorden y postorden |
| 7. Salir | Termina y libera la memoria |

## Manejo de errores y memoria

- **Archivo inexistente:** se informa la ruta que no se pudo abrir.
- **Líneas inválidas y duplicados:** solo se rechaza la línea afectada y se sigue con el resto; al final se informa cuántas líneas se cargaron y cuántas se rechazaron.
- **Entrada del usuario:** texto donde se espera un número, opciones fuera de rango y cantidades no positivas se informan sin que el programa se caiga.
- **Memoria:** todas las estructuras (Queue, Stack, AVL, árbol de decisión y lista de servicios) tienen destructores que liberan sus nodos y los pacientes que almacenan.

## Estructura del proyecto

```
├── include/   # Archivos .h
├── src/       # Archivos .cpp y Main.cpp
├── data/      # pacientes.txt
├── Makefile
└── README.md
```

| Clases | Función |
|--------|---------|
| `Persona`, `Paciente`, `RegistroAtencion` | Datos del paciente y de cada atención |
| `NodoPaciente`, `ColaPacientes`, `CargadorArchivo` | Queue FIFO de pacientes en espera y lectura/validación del archivo |
| `NodoDecision`, `ArbolDecision` | Árbol de decisión |
| `NodoAVL`, `ArbolAVL` | Árbol AVL |
| `Servicio`, `NodoServicio`, `Hospital` | Servicios (cada uno con su AVL) y lista enlazada del hospital |
| `NodoHistorial`, `PilaHistorial` | Stack del historial de atenciones |
| `Main.cpp` | Menú y flujo completo |
