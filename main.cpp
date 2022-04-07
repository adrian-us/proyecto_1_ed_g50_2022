#include <iostream>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <random>

// ============================= MACROS ===========================================
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__) || defined(_WIN64)
#define COMANDO_LIMPIAR_TERMINAL "cls" // windows
#elif defined (macintosh) || defined (Macintosh) || defined (__APPLE__) && (__MACH__) || defined (DARWIN)
#define COMANDO_LIMPIAR_TERMINAL "clear" // mac
#elif defined(__ANDROID__)
#define COMANDO_LIMPIAR_TERMINAL "clear" // android (linux)
#elif defined(__linux__)
#define COMANDO_LIMPIAR_TERMINAL "clear" // linux
#endif

static const std::string MESES[12] = { "Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};

// ============================= FUNCIONES ========================================
/* Funcion que retorna un generador de numeros aleatorio.
 * (Implementado a partir de C++11)
 * Parametros : (void)
 * Retorna : std::mt19937& gen
 */
inline std::mt19937& generador() {
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

/* Funcion que retorna un numero aleatorio en un rango establecido por los
 * parametros. Usa un generador "sembrado" que permite mejorar la aleatoriedad
 * de la funcion. (Implementado a partir de C++11)
 * Parametros : (valor minimo, valor maximo)
 * Retorna : std::uniform_int_distribution<T>
 */
template<typename T>
__attribute__((unused)) T aleatorio(T minimo, T maximo) {
    std::uniform_int_distribution<T> distribution(minimo, maximo);
    return distribution(generador());
}
/* Funcion que valida el tipo de datos del usuario, retorna los datos
 * Parametros : (void)
 * Retorna : (T input deseado)
 */

/* Funcion que valida el tipo de datos del usuario, retorna los datos
 * Parametros : (void)
 * Retorna : (T input deseado)
 */
template <typename T> T recibirInput(){
    T variable;
    while (true){
        if (std::cin >> variable){
            std::cin.clear();
            std::cin.ignore(10000,'\n');
            return variable;
        } else {
            std::cin.clear();
            std::cin.ignore(10000,'\n');
            std::cout << "\nERROR: Tipo de dato recibido invalido\n\n";
        }
        std::cout << "Intente de nuevo : ";
    }
}

/*
 * Funcion que valida los tipos de datos del usuario con requerimientos minimos y
 * maximos especificos del mismo tipo
 * Pre-condiciones : typename T que permita operaciones comparativas, un minimo y maximo
 * del mismo tipo de datos
 * Parametros : (valor minimo, valor maximo)
 * Retorna : (T input deseado)
 */
template <typename T> T recibirInputRango(const T minRange,const T maxRange){
    T variable;
    while (true){
        if (std::cin >> variable){
            std::cin.clear();
            std::cin.ignore(10000,'\n');
            if (variable < minRange || variable > maxRange)
                std::cout << "\nERROR: Variable no esta dentro del rango establecido\n\n";
            else
                return variable;
        } else {
            std::cin.clear();
            std::cin.ignore(10000,'\n');
            std::cout << "\nERROR: Tipo de dato recibido invalido\n\n";
        }
        std::cout << "Intente de nuevo : ";
    }
}

/*
 * Funcion que confirma si un string es igual a otro sin importar
 * su capitalizacion. Tambien evalua si es mayor o menor en orden
 * alfabetico.
 * Si son iguales retorna un 0.
 * Si el primer string es mayor retorna un 1.
 * Si el primer string es menor retorna un -1.
 * Parametros : (primer string, segundo string)
 * Retorna : (int)
 */
int comparar_strings(std::string primerString, std::string segundoString){
    // Hay que pasar los nombres en formato string a mayuscula/minuscula para
    // poder compararlos correctamente
    std::transform(primerString.begin(), primerString.end(), primerString.begin(),
                   [] (unsigned char c) {return std::toupper(c);});
    std::transform(segundoString.begin(), segundoString.end(), segundoString.begin(),
                   [] (unsigned char c) {return std::toupper(c);});
    return primerString.compare(segundoString);
}

// ============================= ESTRUCTURAS ======================================
class SubActividad {
private:
    std::string nombre = {}, descripcion = {};
public:
    SubActividad * siguiente;
    SubActividad(std::string& nombre, std::string& descripcion) : nombre(nombre),descripcion(descripcion),
    siguiente(nullptr){}
    std::string toString() const {  return "[ " + nombre + " | " + descripcion + " ]";}

    friend class ListaSimple_SubActividad;
};

class ListaSimple_SubActividad {
private:
    SubActividad * primerNodo;
public:
    ListaSimple_SubActividad() : primerNodo(nullptr){}

    bool isEmpty(){ return primerNodo == nullptr; }

    int size(){
        int contador = 0;
        for (SubActividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente) contador++;
        return contador;
    }

    bool existe(const std::string& subactividad){
        for (SubActividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (comparar_strings(subactividad,tmp->nombre) == 0) return true;
        return false;
    }

    SubActividad * buscar(const std::string& subactividad){
        for (SubActividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (comparar_strings(subactividad,tmp->nombre) == 0) return tmp;
        return nullptr;
    }

    SubActividad * get(const int indice){
        int contador = 0;
        for (SubActividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
            if (contador == indice) return tmp;
            contador++;
        }
        return nullptr;
    }

    // Insercion al inicio
    void insertar(SubActividad * nuevo){
        nuevo->siguiente = primerNodo;
        primerNodo = nuevo;
    }

    void imprimir(){
        for (SubActividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            std::cout << tmp->toString() << "\n";
    }

    SubActividad * getPrimerNodo(){ return primerNodo; }
};

class Actividad {
private:
    std::string nombre = {}, descripcion = {};
    // Se tiene que inicializar la lista
    ListaSimple_SubActividad * subactividades = new ListaSimple_SubActividad();
public:
    Actividad * siguiente;

    Actividad(std::string& nombre, std::string& descripcion) : nombre(nombre),descripcion(descripcion),siguiente(nullptr){}
    std::string toString() const { return "[ " + nombre + " | " + descripcion + " ]"; }

    // Metodos de sub-lista de subactividades
    bool existeSubactividad(const std::string& nombreSubactividad){
        return subactividades->existe(nombreSubactividad);
    }

    SubActividad * buscarSubactividad(const std::string& nombreSubactividad){
        return subactividades->buscar(nombreSubactividad);
    }

    void insertarSubactividad(SubActividad * subactividad){ subactividades->insertar(subactividad); }

    void imprimirSubactividades(){ subactividades->imprimir(); }

    ListaSimple_SubActividad * getSubactividades() { return subactividades; }

    friend class ListaSimple_Actividad;
};

class ListaSimple_Actividad {
private:
    Actividad * primerNodo;

public:
    ListaSimple_Actividad() : primerNodo(nullptr){}

    bool isEmpty() { return primerNodo == nullptr; }

    int size(){
        int contador = 0;
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente) contador++;
        return contador;
    }

    SubActividad * getAleatorio(){
        SubActividad * subactividad = nullptr;
        while (!subactividad){
            int contador = 0;
            int indice = aleatorio(0,size()-1);
            for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
                if (contador == indice){
                    if (tmp->subactividades->isEmpty()) break;
                    else subactividad = tmp->subactividades->get(aleatorio(0,tmp->subactividades->size()-1));
                }
                contador++;
            }
        }
        return subactividad;
    }

    bool existeActividad(const std::string& actividad){
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (comparar_strings(actividad, tmp->nombre) == 0) return true;
        return false;
    }

    bool existeSubactividad(const std::string& subactividad){
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->existeSubactividad(subactividad)) return true;
        return false;
    }

    Actividad * buscarActividad(const std::string& actividad){
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (comparar_strings(actividad, tmp->nombre) == 0) return tmp;
        return nullptr;
    }

    SubActividad * buscarSubactividad(const std::string& subactividad){
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->existeSubactividad(subactividad)) return tmp->buscarSubactividad(subactividad);
        return nullptr;
    }

    // Insercion al final
    void insertar(Actividad * nuevo){
        if (isEmpty()) primerNodo = nuevo;
        else {
            Actividad * tmp = primerNodo;
            while (tmp->siguiente != nullptr) tmp = tmp->siguiente;
            tmp->siguiente = nuevo;
        }
    }

    void imprimirActividades(){
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            std::cout << tmp->toString() << "\n";
    }

    void imprimirSubactividades(){
        for (Actividad * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
            std::cout << "\n============= Actividad : " << tmp->nombre << " ============\n";
            tmp->imprimirSubactividades();
            std::cout << "=============================================================\n";
        }
    }

    Actividad * getPrimerNodo(){ return primerNodo; }
};

class Formacion {
private:
    std::string nombre = {};
public:

    Formacion * siguiente;

    explicit Formacion(std::string& nombre) : nombre(nombre),siguiente(nullptr){}
    std::string toString() const { return "[ " + nombre + " ]"; }
    std::string getNombre() { return nombre; }
    void setNombre(const std::string& nuevoNombre){ nombre = nuevoNombre; }
    friend class ListaCircular_Formacion;
};

class ListaCircular_Formacion {
private:
    Formacion * primerNodo;
public:
    ListaCircular_Formacion() : primerNodo(nullptr){}

    int size(){
        int contador = 0;
        if (!isEmpty()){
            Formacion * tmp = primerNodo;
            do {
                contador++;
                tmp = tmp->siguiente;
            } while (tmp != primerNodo);
        }
        return contador;
    }

    bool isEmpty(){ return primerNodo == nullptr; }

    bool existe(const std::string& formacion){
        if (!isEmpty()){
            Formacion * tmp = primerNodo;
            do {
                if (comparar_strings(formacion,tmp->nombre) == 0) return true;
                tmp = tmp->siguiente;
            } while (tmp != primerNodo);
        }
        return false;
    }

    Formacion * buscar(const std::string& formacion){
        if (!isEmpty()){
            Formacion * tmp = primerNodo;
            do {
                if (comparar_strings(formacion,tmp->nombre) == 0) return tmp;
                tmp = tmp->siguiente;
            } while (tmp != primerNodo);
        }
        return nullptr;
    }

    Formacion * get(const int indice){
        if (!isEmpty()){
            int contador = 0;
            Formacion * tmp = primerNodo;
            do {
                if (contador == indice) return tmp;
                tmp = tmp->siguiente;
                contador++;
            } while (tmp != primerNodo);
        }
        return nullptr;
    }

    // Insercion al final
    void insertar(Formacion * nuevo){
        if (isEmpty()){
            nuevo->siguiente = nuevo;
            primerNodo = nuevo;
        } else {
            Formacion * tmp = primerNodo;
            while (tmp->siguiente != primerNodo) tmp = tmp->siguiente;
            nuevo->siguiente = primerNodo;
            tmp->siguiente = nuevo;
        }
    }

    bool eliminar(const std::string& formacion){
        if (!isEmpty()){
            Formacion * tmp = primerNodo;
            // Primero hay que descartar que sea el primer nodo
            if (comparar_strings(formacion,primerNodo->nombre) == 0){
                // Si hay mas de un nodo
                if (primerNodo->siguiente != primerNodo){
                    Formacion * temp = primerNodo;
                    while (temp->siguiente != primerNodo) temp = temp->siguiente;
                    temp->siguiente = primerNodo->siguiente;
                    primerNodo = primerNodo->siguiente;
                } else primerNodo = nullptr;
                delete tmp; // Liberar memoria
                return true;
            } else {
                Formacion * anterior;
                do {
                    anterior = tmp;
                    tmp = tmp->siguiente;
                    if (comparar_strings(formacion,tmp->nombre) == 0){
                        anterior->siguiente = tmp->siguiente;
                        tmp = tmp->siguiente = nullptr;
                        delete tmp;
                        return true;
                    }
                } while (tmp != primerNodo);
            }
        }
        return false;
    }

    void imprimir(){
        if (!isEmpty()){
            Formacion * tmp = primerNodo;
            do {
                std::cout << tmp->toString() << "\n";
                tmp = tmp->siguiente;
            } while (tmp != primerNodo);
        }
    }

    Formacion * getPrimerNodo() { return primerNodo; }
};

class Puesto {
private:
    std::string nombre = {};
    int salarioMinimo = {};
public:

    Puesto * siguiente;

    Puesto(std::string& nombre,int const salario) : nombre(nombre), salarioMinimo(salario), siguiente(nullptr){}
    std::string toString() const { return "[ "+nombre+" | Salario minimo : "+std::to_string(salarioMinimo)+" ]";}
    friend class ListaSimple_Puesto;
};

class ListaSimple_Puesto {
private:
    Puesto * primerNodo;
public:
    ListaSimple_Puesto() : primerNodo(nullptr){}

    bool isEmpty(){ return primerNodo == nullptr; }

    int size(){
        int contador = 0;
        for (Puesto * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente) contador++;
        return contador;
    }

    bool existe(const std::string& puesto){
        for (Puesto * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (comparar_strings(puesto,tmp->nombre) == 0) return true;
        return false;
    }

    // Busca un puesto por nombre y lo retorna. Si no lo encuentra retorna 'nullptr'
    Puesto * buscar(const std::string& puesto){
        for (Puesto * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (comparar_strings(puesto,tmp->nombre) == 0) return tmp;
        return nullptr;
    }

    Puesto * get(const int indice){
        int contador = 0;
        for (Puesto * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
            if (contador == indice) return tmp;
            contador++;
        }
        return nullptr;
    }

    // Insercion al inicio
    void insertar(Puesto * nuevo){
        nuevo->siguiente = primerNodo;
        primerNodo = nuevo;
    }

    void imprimir(){
        for (Puesto * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            std::cout << tmp->toString() << "\n";
    }

    Puesto * getPrimerNodo() { return primerNodo; }
};

class SubActividad_Personal {
private:
    int horas = {}, horaInicio = {}, fechaDia = {}, fechaMes = {};
    SubActividad * subactividad;
    SubActividad_Personal * siguiente;
public:
    SubActividad_Personal(int horas,
                          int horaInicio,
                          int fechaDia,
                          int fechaMes,
                          SubActividad * subactividad) :
    horas(horas),
    horaInicio(horaInicio),
    fechaDia(fechaDia),
    fechaMes(fechaMes),
    subactividad(subactividad),
    siguiente(nullptr){}

    std::string toString() const {
        return "[ Fecha : " + MESES[fechaMes] + " " + std::to_string(fechaDia) +" | Subactividad : " + subactividad->toString() + " | Horas : " + std::to_string(horas) +
        " | Hora de inicio : " + std::to_string(horaInicio) + ":00 ]";
    }

    // Setters (Modificadores)
    void setHoras(int cantidadHoras){ horas = cantidadHoras; }
    void setHoraInicio(int nuevaHoraInicio) { horaInicio = nuevaHoraInicio; }
    void setFecha(int nuevaFechaDia,int nuevaFechaMes) {
        fechaDia = nuevaFechaDia;
        fechaMes = nuevaFechaMes;
    }

    // Getters
    int getHoras() const { return horas; }
    int getHoraInicio() const { return horaInicio; }
    int getFechaDia() const { return fechaDia; }
    int getFechaMes() const { return fechaMes; }

    friend class ListaSimple_SubActividadesPersonales;
};

class ListaSimple_SubActividadesPersonales {
private:
    // Implementar disponibilidad de horario por medio de booleanos
    bool disponibilidad[31][12][24] = {};
    int horasUsadas[31][12] = {};
    SubActividad_Personal * primerNodo;
public:
    ListaSimple_SubActividadesPersonales() : primerNodo(nullptr){
        for (auto & dia : disponibilidad)
            for (auto & mes : dia)
                for (bool & me : mes)
                    me = true;
    }

    bool isEmpty(){ return primerNodo == nullptr; }

    bool existe(SubActividad * subactividad){
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->subactividad == subactividad) return true;
        return false;
    }

    SubActividad_Personal * buscar(SubActividad * subactividad){
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->subactividad == subactividad) return tmp;
        return nullptr;
    }

    int size(){
        int tamano = 0;
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente) tamano++;
        return tamano;
    }

    bool posibleAgendar(int fechaDia, int fechaMes, int horas, int horaInicio){
        // Verificar que la duracion no sobrepase las 24 horas
        if (horasUsadas[fechaDia][fechaMes] + horas > 24) return false;
        // Verificar que el periodo de la sub-actividad no se pase de un dia a otro
        if (horaInicio + horas >= 24) return false;
        // Verificar que haya disponibilidad de horario en el periodo necesario
        // Empezar desde la hora de inicio e iterar la cantidad de horas
        for (int hora = horaInicio; hora < horaInicio+horas; hora++)
            if (!disponibilidad[fechaDia][fechaMes][hora]) return false;
        return true;
    }

    // Insertar al final
    void insertar(SubActividad_Personal * nuevo){
        if (isEmpty()) primerNodo = nuevo;
        else {
            SubActividad_Personal * tmp = primerNodo;
            while (tmp->siguiente != nullptr) tmp = tmp->siguiente;
            tmp->siguiente = nuevo;
        }
        registrarHoras(nuevo->fechaDia,nuevo->fechaMes,nuevo->horaInicio,nuevo->horas);
    }

    void registrarHoras(int fechaDia, int fechaMes, int horaInicio, int horas){
        // aumentar la cantidad de horas usadas del dia
        horasUsadas[fechaDia][fechaMes] += horas;
        // marcar el periodo de la actividad como NO disponible (false)
        for (int hora = horaInicio; hora < horaInicio+horas; hora++)
            disponibilidad[fechaDia][fechaMes][hora] = false;
    }

    void liberarHoras(int fechaDia, int fechaMes, int horaInicio, int horas){
        // disminuir la cantidad de horas usadas del dia
        horasUsadas[fechaDia][fechaMes] -= horas;
        // marcar el periodo de la actividad como SI disponible (true)
        for (int hora = horaInicio; hora < horaInicio+horas; hora++)
            disponibilidad[fechaDia][fechaMes][hora] = true;
    }

    /* Eliminar en posicion
     * retorna booleano
     * true : si el indice esta dentro del rango de la lista
     * false : si el indice esta fuera del rango de la lista
     */
    bool eliminarEnPosicion(int indice){
        // Agarrar Index Out of Range
        if (isEmpty() || this->size() <= indice) return false;
        else {
            int contador = 0;
            SubActividad_Personal * tmp = primerNodo;
            // liberar el plazo del dia dedicada a esa actividad
            liberarHoras(tmp->fechaDia,tmp->fechaMes,tmp->horaInicio,tmp->horas);
            // Si se quiere eliminar el primer elemento
            if (indice == 0){
                primerNodo = primerNodo->siguiente;
                delete tmp;
            } else {
                while (tmp != nullptr){
                    // Se detiene en el nodo anterior al que se quiere eliminar
                    if (contador == indice-1){
                        SubActividad_Personal * borrado = tmp->siguiente;
                        tmp->siguiente = tmp->siguiente->siguiente;
                        delete borrado;
                        break;
                    }
                    tmp = tmp->siguiente;
                    contador++;
                }
            }
            return true;
        }
    }

    /* Eliminar nodo si existe
     * retorna booleano
     * true : si lo logra eliminar , false : si no existe dicho nodo
     * */
    bool eliminar(SubActividad * subactividad){
        int contador = 0;
        SubActividad_Personal * tmp = primerNodo;
        while (tmp != nullptr){
            if (tmp->subactividad == subactividad) return eliminarEnPosicion(contador);
            tmp = tmp->siguiente;
            contador++;
        }
        return false;
    }

    int getHorasUsadas() {
        int cantidadHorasUsadas = 0;
        for (int mes = 0; mes < 12; mes++)
            for (int dia = 0; dia < 30; dia++)
                cantidadHorasUsadas += horasUsadas[dia][mes];
        return cantidadHorasUsadas;
    }

    void imprimirAgenda(int fechaDia, int fechaMes){
        std::cout << "Fecha : " << fechaDia << " / " << MESES[fechaMes] << "\n";
        for (int hora = 0; hora < 24; hora++)
            std::cout << "Hora : " << hora << ":00 - Disponible : " << std::boolalpha << disponibilidad[fechaDia][fechaMes][hora] << "\n";
    }

    std::string buscarActividadPorFecha(int fechaDia,int fechaMes,int hora){
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
            if (tmp->fechaDia == fechaDia && tmp->fechaMes == fechaMes && (hora >= tmp->horaInicio || hora <= tmp->horaInicio+tmp->horas-1))
                return tmp->toString();
        }
        return "";
    }

    void imprimirAgenda_Mejorada(int fechaDia, int fechaMes){
        std::cout << "Fecha : " << fechaDia << " / " << MESES[fechaMes] << "\n";
        for (int hora = 0; hora < 24; hora++){
            std::string actividad = (disponibilidad[fechaDia][fechaMes][hora]) ? "Disponible" : buscarActividadPorFecha(fechaDia,fechaMes,hora);
            std::cout << "Hora : " << hora << ":00 - " << actividad << "\n";
        }
    }

    void getDistribucionTiempo(int fechaDia, int fechaMes){
        std::cout << "Fecha : " << fechaDia << " / " << MESES[fechaMes] << "\n";
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
            if (tmp->fechaDia == fechaDia && tmp->fechaMes == fechaMes)
                std::cout << "\tTiempo invertido en : " << tmp->subactividad->toString() << " : " << tmp->horas << "\n";
        }
        std::cout << "Tiempo invertido en total : " << horasUsadas[fechaDia][fechaMes] << ":00 horas\n";
    }

    void getDistribucionSemana(int fechaDia, int fechaMes){
        // todo bretear mejor
        for (int i = 0; i < 7; i++){
            getDistribucionTiempo(fechaDia,fechaMes);
            if (fechaDia == 30){
                fechaDia = 0;
                fechaMes += 1;
            }
            fechaDia += 1;
        }
    }

    std::string toString() {
        int cantidadHorasUsadas = getHorasUsadas();
        std::string texto = "Cantidad de horas usadas : " + std::to_string(cantidadHorasUsadas)+"\n";
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            texto += tmp->toString() + "\n";
        return texto;
    }

    int getTiempoSubactividad(SubActividad * subactividad){
        int horas_subactividad = 0;
        for (SubActividad_Personal * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->subactividad == subactividad) horas_subactividad += tmp->horas;
        return horas_subactividad;
    }
};

class Persona {
private:
    int const cedula = {};
    std::string nombre = {},residencia = {};
    int edad = {};
    // vinculos
    Puesto * puesto;
    Formacion * formacion;
    int formacionInicio = {};
    int formacionFin = {};
    bool formacionCompleta = {};
    ListaSimple_SubActividadesPersonales * listaSubactividades = new ListaSimple_SubActividadesPersonales();

public:

    Persona * siguiente, * anterior;

    Persona(int const cedula, std::string& nombre, int const edad, std::string& residencia) :
    cedula(cedula),nombre(nombre),edad(edad),residencia(residencia),siguiente(nullptr),anterior(nullptr)
    ,puesto(nullptr),formacion(nullptr){}

    std::string getPuesto_str() const { return (puesto) ? puesto->toString() : "No registrado."; }
    std::string getFormacionCompleta_str() const { return (formacionCompleta) ? "Completa" : "En proceso"; }
    std::string getFormacion_str() const {
        return (formacion) ?
               "[ "+formacion->getNombre() + " | " + getFormacionCompleta_str() + " | Inicio " + std::to_string(formacionInicio) +
               " | Fin " + std::to_string(formacionFin) + " ]\n"
                           : "No registrado.";
    }
    std::string getActividades_str() const { return listaSubactividades->toString(); }

    std::string toString() const {
        return std::string("=====================================================================================\n")
               +std::string("[ ")+nombre+std::string(" | Cedula : ")+std::to_string(cedula)+std::string(" | Edad : ")+
               std::to_string(edad)+std::string(" | Residencia : ")+residencia+std::string(" ]")
               +"\n---------------------------------------------------------------------\n"+
               std::string("\tPuesto    ") + getPuesto_str()
               +"\n---------------------------------------------------------------------\n"+
               std::string("\tFormacion ") + getFormacion_str()+
               +"\n---------------------------------------------------------------------\n"+
               std::string("\tActividades ") + getActividades_str() +
               std::string("\n=====================================================================================\n\n");
    }

    // Setters (Modificadores) & Getters
    void setEdad(int nuevaEdad) { edad = nuevaEdad;}
    void setResidencia(const std::string &nuevaResidencia) { residencia = nuevaResidencia; }
    void setPuesto(Puesto * nuevoPuesto) { puesto = nuevoPuesto; }
    void setFormacion(Formacion * nuevaFormacion) { formacion = nuevaFormacion; }
    void setFormacionInicio(int nuevoInicio) { formacionInicio = nuevoInicio; }
    void setFormacionFin(int nuevoFin) { formacionFin = nuevoFin; }
    void setFormacionCompleta(bool nuevoEstado) { formacionCompleta = nuevoEstado; }
    Formacion * getFormacion() { return formacion; }

    // verificar si hay disponibilidad de horario para registrar una actividad
    bool posibleAgendar(int fechaDia, int fechaMes, int cantidadHoras, int horaInicio){
        return listaSubactividades->posibleAgendar(fechaDia,fechaMes,cantidadHoras,horaInicio);
    }

    // agregar una sub-actividad
    void agregarSubactividad(SubActividad_Personal * nuevaSubactividad){
        listaSubactividades->insertar(nuevaSubactividad);
    }

    // buscar una sub-actividad
    SubActividad_Personal * buscarSubactividadPersonal(SubActividad * subactividad){
        return listaSubactividades->buscar(subactividad);
    }

    // averiguar el tiempo que consume una sub-actividad
    int getTiempoSubactividad(SubActividad * subactividad){
        return listaSubactividades->getTiempoSubactividad(subactividad);
    }

    // mostrar la disponibilidad de agenda para esta persona en una fecha
    void imprimirDisponibilidad(int dia, int mes){
        listaSubactividades->imprimirAgenda(dia,mes);
    }

    // retornar la lista de subactividades para liberar y agendar espacio (modificar sub-actividad)
    ListaSimple_SubActividadesPersonales * getListaSubactividades() const { return listaSubactividades; }

    // mostrar la distribucion del tiempo dada una fecha
    void imprimirDistribucionFecha(int fechaDia, int fechaMes){
        std::cout << "=====================================================================================\n" <<
                     "[ " << nombre << " | Cedula : " << cedula << " | Edad : " << edad
                     << " | Residencia : " << residencia <<" ]" <<
                     "\n---------------------------------------------------------------------\n" <<
                     "\tPuesto    " << // +getPuesto_str()<<
                     "\n---------------------------------------------------------------------\n"<<
                     "\tFormacion " //+getFormacion_str()+
                     << "\n---------------------------------------------------------------------\n"
                     << "Fecha : " << MESES[fechaMes] << " / " << fechaDia << "\n";

        listaSubactividades->imprimirAgenda_Mejorada(fechaDia,fechaMes);
        listaSubactividades->getDistribucionTiempo(fechaDia,fechaMes);
    }

    void imprimirDistribucionSemana(int fechaDia, int fechaMes){
        std::cout << "=====================================================================================\n" <<
                  "[ " << nombre << " | Cedula : " << cedula << " | Edad : " << edad
                  << " | Residencia : " << residencia <<" ]" <<
                  "\n---------------------------------------------------------------------\n" <<
                  "\tPuesto    " << getPuesto_str() <<
                  "\n---------------------------------------------------------------------\n"<<
                  "\tFormacion " << getFormacion_str()
                  << "\n---------------------------------------------------------------------\n"
                  << "Fecha : " << MESES[fechaMes] << " / " << fechaDia << "\n";
        listaSubactividades->getDistribucionSemana(fechaDia,fechaMes);
    }

    friend class ListaSimple_Persona;
};

class ListaSimple_Persona{
private:
    Persona * primerNodo;

    bool isEmpty() { return primerNodo == nullptr; }

public:

    int size(){
        int contador = 0;
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            contador++;
        return contador;
    }

    /* Esta funcion itera por cada persona en la lista y si la cedula
     * esta asociada a alguna persona retorna true, indicando que si existe esa persona
     */
    bool existe(int cedula){
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->cedula == cedula) return true;
        return false;
    }

    /* Esta funcion se encarga de buscar un nodo persona
     * por su cedula y lo retorna
     */
    Persona * buscar(int cedula){
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->cedula == cedula) return tmp;
        return nullptr;
    }

    /*
     * Esta funcion recibe un nombre en formato string
     * e imprimie todos los elementos de la lista que tengan
     * el mismo nombre para que el usuario pueda visualizar mejor
     */
    void buscarNombre(const std::string& nombrePersona){
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente){
            if (comparar_strings(nombrePersona, tmp->nombre) == 0)
                std::cout << tmp->toString() << "\n";
        }
    }

    /*
    * Esta funcion inserta un nodo nuevo en el orden respectivo
    * segun el orden alfabetico. Hay tres opciones:
            * 1. Si la lista esta vacia
    * 2. Si la lista no esta vacia y el nuevo nodo se debe
    * insertar al inicio
    * 3. Si la lista no esta vacia y el nuevo nodo no se debe
            * insertar al inicio
    */
    void insertar(Persona * nuevo){
        if (isEmpty()) primerNodo = nuevo;
        else {
            // Primero hay que descartar que vaya de primero
            if (comparar_strings(nuevo->nombre,primerNodo->nombre) < 0){
                nuevo->siguiente = primerNodo;
                primerNodo->anterior = nuevo;
                primerNodo = nuevo;
            } else {
                // De no ser asi entonces se itera hasta llegar al final o algun nombre que este
                // despues de este, de lo que ser asi entonces se agarra el anterior para que apunte
                // al nuevo y al nuevo para que apunte al siguiente, colocandolo asi entre un nombre menor
                // y uno mayor. Insertando asi en su posicion respectiva segun el orden alfabetico
                Persona * tmp = primerNodo, * anterior;
                do {
                    anterior = tmp;
                    tmp = tmp->siguiente;
                } while ( tmp != nullptr && comparar_strings(nuevo->nombre,tmp->nombre) >= 0);
                nuevo->anterior = anterior;
                nuevo->siguiente = tmp;
                anterior->siguiente = nuevo;
                if (tmp) tmp->anterior = nuevo;
            }
        }
    }

    // Esta funcion imprime todas las personas al standard output
    void imprimir(){
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            std::cout << tmp->toString() << "\n";
    }

    bool eliminar(int cedula){
        Persona * tmp = primerNodo;
        while (tmp != nullptr){
            if (tmp->cedula == cedula){
                // Si el nodo a eliminar es el primer nodo
                if (tmp == primerNodo) primerNodo = tmp->siguiente;
                // Enlazar el nodo siguiente solamente si el nodo a eliminar
                // NO es el ultimo. Ya que habria problemas de manipulacion de punteros
                // nulos.
                if (tmp->siguiente != nullptr) tmp->siguiente->anterior = tmp->anterior;
                // Enlazar el nodo anterior solamente si el nodo a eliminar
                // NO es el primer nodo. Ya que habria problemas de manipulacion de punteros nulos.
                if (tmp->anterior != nullptr) tmp->anterior->siguiente = tmp->siguiente;
                // Finalmente, liberar la memoria ocupada por el nodo
                tmp = tmp->siguiente = tmp->anterior = nullptr;
                delete tmp;
                return true;
            }
            tmp = tmp->siguiente;
        }
        return false;
    }

    int consumoTiempoSubactividad(SubActividad * subactividad){
        int contador = 0;
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            contador += tmp->getTiempoSubactividad(subactividad);
        return contador;
    }

    // mostrar el porcentaje de una formacion
    float getPorcentajeFormacion(Formacion * formacion){
        float porcentaje = 0.0f;
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->formacion == formacion) porcentaje++;
        if (porcentaje == 0.0f || size() == 0) return 0.0f;
        return (porcentaje / (float) size()) * 100;
    }

    float getPorcentajePuesto(Puesto * puesto){
        float porcentaje = 0.0f;
        for (Persona * tmp = primerNodo; tmp != nullptr; tmp = tmp->siguiente)
            if (tmp->puesto == puesto) porcentaje++;
        if (porcentaje == 0.0f || size() == 0) return 0.0f;
        return (porcentaje / (float) size()) * 100;
    }

    Persona * getPrimerNodo() { return primerNodo; }
};

// =========== Funciones recursivas de consulta ================
SubActividad * getMayorConsumo_recursivo(ListaSimple_Persona *personas,Actividad *actividad,SubActividad *mayor){
    if (actividad == nullptr) return mayor;
    for (SubActividad * tmp = actividad->getSubactividades()->getPrimerNodo(); tmp != nullptr; tmp = tmp->siguiente)
        if (personas->consumoTiempoSubactividad(tmp) > personas->consumoTiempoSubactividad(mayor))
            mayor = tmp;
    return getMayorConsumo_recursivo(personas,actividad->siguiente,mayor);
}

SubActividad * getMenorConsumo_recursivo(ListaSimple_Persona *personas,Actividad *actividad,SubActividad *menor){
    if (actividad == nullptr) return menor;
    for (SubActividad * tmp = actividad->getSubactividades()->getPrimerNodo(); tmp != nullptr; tmp = tmp->siguiente)
        if (personas->consumoTiempoSubactividad(tmp) < personas->consumoTiempoSubactividad(menor))
            menor = tmp;
    return getMenorConsumo_recursivo(personas,actividad->siguiente,menor);
}

SubActividad * getMayorConsumo(ListaSimple_Persona * personas, Actividad * actividad){
    if (actividad == nullptr) return nullptr;
    return getMayorConsumo_recursivo(personas,actividad,actividad->getSubactividades()->getPrimerNodo());
}

SubActividad * getMenorConsumo(ListaSimple_Persona * personas, Actividad * actividad){
    if (actividad == nullptr) return nullptr;
    return getMenorConsumo_recursivo(personas,actividad,actividad->getSubactividades()->getPrimerNodo());
}

void getConsumoActividades(ListaSimple_Persona * personas, Actividad * actividad){
    if (actividad == nullptr) return;
    std::cout << "\nActividad : " << actividad->toString() << "\n";
    int cantidadTiempoActividad = 0;
    int consumoTiempoSubactividad = 0;
    for (SubActividad * tmp = actividad->getSubactividades()->getPrimerNodo(); tmp != nullptr; tmp = tmp->siguiente){
        consumoTiempoSubactividad = personas->consumoTiempoSubactividad(tmp);
        cantidadTiempoActividad += consumoTiempoSubactividad;
        std::cout << "\tSubactividad : " << tmp->toString() << " | Tiempo consumido : " << consumoTiempoSubactividad << " horas.\n";
    }
    std::cout << "Total de tiempo consumido por actividad : " << cantidadTiempoActividad << " horas.\n";
    return getConsumoActividades(personas,actividad->siguiente);
}

void getActividadCeroConsumo(ListaSimple_Persona * personas, Actividad * actividad) {
    if (actividad == nullptr) return;
    int cantidadTiempoActividad = 0;
    for (SubActividad *tmp = actividad->getSubactividades()->getPrimerNodo(); tmp != nullptr; tmp = tmp->siguiente)
        cantidadTiempoActividad += personas->consumoTiempoSubactividad(tmp);
    if (cantidadTiempoActividad == 0)
        std::cout << "Actividad : " << actividad->toString() << " | Total de tiempo consumido por actividad : " << cantidadTiempoActividad << " horas.\n";
    return getActividadCeroConsumo(personas, actividad->siguiente);
}

// esta no puede ser recursiva xq es circular y cuesta medir el fin
void imprimirPorcentajeFormacion(ListaSimple_Persona * personas, ListaCircular_Formacion * formaciones){
    Formacion * tmp = formaciones->getPrimerNodo();
    do {
        float porcentaje = personas->getPorcentajeFormacion(tmp);
        if (porcentaje != 0.0f)
            std::cout << tmp->toString() << " - " << porcentaje << " %\n";
        tmp = tmp->siguiente;
    } while (tmp != formaciones->getPrimerNodo());
}

void imprimirPorcentajePuesto(ListaSimple_Persona * personas, Puesto * puesto){
    if (puesto == nullptr) return;
    float porcentaje = personas->getPorcentajePuesto(puesto);
    if (porcentaje != 0.0f)
        std::cout << puesto->toString() << " - " << porcentaje << " %\n";
    return imprimirPorcentajePuesto(personas,puesto->siguiente);
}

void asignarPuestosFormaciones(Persona * persona, ListaCircular_Formacion * formaciones,ListaSimple_Puesto * puestos){
    if (persona == nullptr) return;
    // asignar formacion
    Formacion * formacion;
    Puesto * puesto;
    if (formaciones->isEmpty()){
        formacion = new Formacion((std::string &) "Secundaria");
        formaciones->insertar(formacion);
    }
    else formacion = formaciones->get(aleatorio(0,formaciones->size()-1));
    persona->setFormacion(formacion);
    if (puestos->isEmpty()){
        puesto = new Puesto((std::string &)"Cerrajero",315000);
        puestos->insertar(puesto);
    } else puesto = puestos->get(aleatorio(0,puestos->size()-1));
    persona->setPuesto(puesto);
    return asignarPuestosFormaciones(persona->siguiente,formaciones,puestos);
}

// ============================= MAIN =============================================

int main() {
    // Inicializar estructuras
    auto * listaFormaciones = new ListaCircular_Formacion();
    auto * listaPuestos = new ListaSimple_Puesto();
    auto * listaActividades = new ListaSimple_Actividad();
    auto * listaPersonas = new ListaSimple_Persona();
    // Cargar datos
    std::string linea;
    std::ifstream archivo("actividades.txt");
    if (archivo.is_open()){
        while (std::getline(archivo,linea)){
            char lineaChar[linea.size()+1];
            strcpy(lineaChar,linea.c_str());
            char * actividad = strtok(lineaChar,";");
            char * descripcion = strtok(nullptr,";");
            // c++ de una pasa a char * a std::string
            std::string actividad_str = actividad;
            std::string descripcion_str = descripcion;
            auto * nuevaActividad = new Actividad(actividad_str,descripcion_str);
            listaActividades->insertar(nuevaActividad);
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abir el archivo : " << "actividades" << ".txt\n";
        return 1;
    }
    archivo.open("subactividades.txt");
    if (archivo.is_open()){
        while (std::getline(archivo,linea)){
            char lineaChar[linea.size()+1];
            strcpy(lineaChar,linea.c_str());
            char * actividad = strtok(lineaChar,";");
            char * subactividad = strtok(nullptr,";");
            char * descripcion = strtok(nullptr,";");
            std::string actividad_str = actividad;
            std::string subactividad_str = subactividad;
            std::string descripcion_str = descripcion;
            Actividad * tipoActividad = listaActividades->buscarActividad(actividad_str);
            if (tipoActividad){
                auto * nuevoSubactividad = new SubActividad(subactividad_str,descripcion_str);
                tipoActividad->insertarSubactividad(nuevoSubactividad);
            }
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abir el archivo : " << "subactividades" << ".txt\n";
        return 1;
    }
    archivo.open("personas.txt");
    if (archivo.is_open()){
        while (std::getline(archivo,linea)){
            char lineaChar[linea.size()+1];
            strcpy(lineaChar,linea.c_str());
            char * distrito = strtok(lineaChar,";");
            char * canton = strtok(nullptr,";");
            char * provincia = strtok(nullptr,";");
            char * nombre = strtok(nullptr,";");
            std::string distrito_str(distrito),canton_str(canton),provincia_str(provincia),nombre_str(nombre);
            std::string residencia = distrito+std::string(",")+canton+std::string(",")+provincia;
            int cedula = aleatorio(100,999);
            int edad = aleatorio(20,60);
            while (listaPersonas->existe(cedula))
                cedula = aleatorio(100,999);
            listaPersonas->insertar(new Persona(cedula,nombre_str,edad,residencia));
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abir el archivo : " << "personas" << ".txt\n";
        return 1;
    }
    archivo.open("formacion.txt");
    if (archivo.is_open()){
        while (std::getline(archivo,linea)){
            char lineaChar[linea.size()+1];
            strcpy(lineaChar,linea.c_str());
            char * formacion = strtok(lineaChar,";");
            std::string formacion_str(formacion);
            listaFormaciones->insertar(new Formacion(formacion_str));
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abir el archivo : " << "formacion" << ".txt\n";
        return 1;
    }
    archivo.open("empleos.txt");
    if (archivo.is_open()){
        while (std::getline(archivo,linea)){
            char lineaChar[linea.size()+1];
            strcpy(lineaChar,linea.c_str());
            char * empleo = strtok(lineaChar,";");
            std::string empleo_str(empleo);
            int const salario = aleatorio(250000,800000);
            listaPuestos->insertar(new Puesto(empleo_str,salario));
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abir el archivo : " << "empleos" << ".txt\n";
        return 1;
    }
    // asignarles puestos y formaciones a cada persona
    asignarPuestosFormaciones(listaPersonas->getPrimerNodo(),listaFormaciones,listaPuestos);

    // Menu principal
    int decision;
    do {
        std::system(COMANDO_LIMPIAR_TERMINAL);
        std::cout << "\t\tA D M I N I S T R A   T U   T I E M P O\n";
        std::cout << "\nMantenimiento\n";
        std::cout << "\t1.  Insertar persona nueva\n";
        std::cout << "\t2.  Insertar puesto nuevo\n";
        std::cout << "\t3.  Insertar formacion nueva\n";
        std::cout << "\t4.  Insertar actividad nueva\n";
        std::cout << "\t5.  Insertar sub-actividad nueva\n";
        std::cout << "\t7.  Editar personal\n";
        std::cout << "\t11. Editar formacion\n";
        std::cout << "\t16. Eliminar persona\n";
        std::cout << "Reportes\n";
        /*
         *   1.	Imprimir la información de todas las listas. todo LISTO
         *   2.	Imprimir la información del total del tiempo registrado por persona en cada semana, según las actividades registradas,
         *   no se puede exceder de 24 horas, sino serían un error de no validación en la inserción. todo LISTO
         *   3.	Imprimir la distribución de los porcentajes según la formación del personal. Los que son ceros no se deben imprimir. todo LISTO
         *   4.	Imprimir la distribución de los porcentajes según los puestos. Los que son ceros no se deben imprimir. todo LISTO
         *   5.	Imprimir la distribución del tiempo en los porcentajes de las sub actividades de deportes. Los que son ceros no se deben imprimir. todo
         *   6.	Imprimir los porcentajes de las sub actividades de internet. Los que son ceros no se deben imprimir. todo
         */
        std::cout << "\t20. Visualizar personas\n";
        std::cout << "\t21. Visualizar puestos\n";
        std::cout << "\t22. Visualizar formaciones\n";
        std::cout << "\t23. Visualizar actividades\n";
        std::cout << "\t24. Visualizar sub-actividades\n";
        std::cout << "\t25. Visualizar porcentajes de formacion\n";
        std::cout << "\t25. Visualizar porcentajes de puesto\n";
        std::cout << "\t26. Visualizar porcentajes de sub-actividades de deportes\n";

        std::cout << "Consultas\n";
        /*   Consultas
         *   1.	¿Cual es el puesto mas frecuente en todas las personas registradas? todo
         *   2.	¿Cual es la formacion mas frecuente en todas las personas registradas? todo
         *   3.	¿Cual subactividad que tiene mayor consumo de tiempo según los datos registrados y cual es la que tiene menos tiempo registrado? todo LISTO
         *   4.	¿Cual o cuales son las actividades que no registran consumo de tiempo? todo LISTO
         *   5.	Consultar una persona X, y dar la distribucion de su tiempo gastado en un dia X (fecha exacta). todo LISTO
         *   6.	Consultar una persona X, y dar la distribucion de su tiempo gastado en una semana X (fecha inicial y de ahi sumar los 7 dias). todo MASOMENOS
         */
        std::cout << "\t32. Subactividad con mayor y menor consumo de tiempo\n";
        std::cout << "\t33. Consumo de tiempo por tipo de actividad\n";
        std::cout << "\t34. Actividades que no registran consumo de tiempo\n";
        std::cout << "\t35. Consultar distribucion de tiempo de una persona\n";
        std::cout << "\t36. Consultar distribicion de tiempo por semana de una persona\n";
        std::cout << "\t45. Salir\n";
        std::cout << "Digite la opcion : ";
        decision = recibirInput<int>();
        std::system(COMANDO_LIMPIAR_TERMINAL);
        switch (decision){
            case 1:{
                std::cout << "Ingresar una persona nueva al sistema\n\n";
                std::string nombre, distrito, canton, provincia;
                std::cout << "Digite la cedula : ";
                int cedula = recibirInput<int>();
                if (!listaPersonas->existe(cedula)){
                    std::cout << "Digite el nombre : ";
                    std::getline(std::cin,nombre);
                    std::cout << "Digite la provincia de residencia : ";
                    std::getline(std::cin,provincia);
                    std::cout << "Digite el canton de residencia : ";
                    std::getline(std::cin,canton);
                    std::cout << "Digite el distrito de residencia : ";
                    std::getline(std::cin,distrito);
                    std::cout << "Digite la edad : ";
                    int edad = recibirInput<int>();
                    std::string residencia = distrito+std::string(",")+canton+std::string(",")+provincia;
                    auto * nuevaPersona = new Persona(cedula,nombre,edad,residencia);
                    listaPersonas->insertar(nuevaPersona);
                    std::cout << nuevaPersona->toString() << " registrada con exito.\n";
                } else { std::cout << "Ya existe una persona registrada bajo esa cedula.\n";}
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 2:{
                std::cout << "Agregar un puesto al sistema\n\n";
                std::string nombrePuesto;
                std::cout << "Digite el nombre del puesto : ";
                std::getline(std::cin, nombrePuesto);
                // Verificar que el puesto no exista en el sistema
                if (!listaPuestos->existe(nombrePuesto)){
                    std::cout << "Digite el salario minimo : ";
                    int salarioMinimo = recibirInput<int>();
                    auto * puestoNuevo = new Puesto(nombrePuesto, salarioMinimo);
                    listaPuestos->insertar(puestoNuevo);
                    std::cout << puestoNuevo->toString() << " registrado con exito.\n";
                } else { std::cout << "Ya ese puesto esta registrado.\n";}
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 3:{
                std::cout << "Agregar una formacion al sistema\n\n";
                std::string nombreFormacion;
                std::cout << "Digite la formacion : ";
                std::getline(std::cin, nombreFormacion);
                // Verificar que la formacion no exista en el sistema
                if (!listaFormaciones->existe(nombreFormacion)){
                    auto * nuevaFormacion = new Formacion(nombreFormacion);
                    listaFormaciones->insertar(nuevaFormacion);
                    std::cout << nuevaFormacion->toString() << " registrada con exito.\n";
                } else { std::cout << "Ya esta formacion esta registrada.\n";}
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 4:{
                std::cout << "Agregar una actividad al sistema\n\n";
                std::string nombreActividad;
                std::cout << "Digite la actividad : ";
                std::getline(std::cin,nombreActividad);
                // Verificar que no este registrada ya
                if (!listaActividades->existeActividad(nombreActividad)){
                    std::string descripcion;
                    std::cout << "Digite una breve descripcion de la actividad : ";
                    std::getline(std::cin,descripcion);
                    auto * nuevaActividad = new Actividad(nombreActividad,descripcion);
                    listaActividades->insertar(nuevaActividad);
                    std::cout << nuevaActividad->toString() << " registrada con exito.\n";
                } else { std::cout << "Ya esta actividad esta registrada.\n"; }
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 5:{
                std::cout << "Agregar una sub-actividad al sistema\n\n";
                std::string nombreSubactividad;
                std::cout << "Digite la sub-actividad : ";
                std::getline(std::cin,nombreSubactividad);
                // Verificar que la sub-actividad no este
                // registrada ya en cualquier bajo cualquier actividad
                if (!listaActividades->existeSubactividad(nombreSubactividad)) {
                    std::string descripcion,nombreActividad;
                    std::cout << "Digite una breve descripcion de la sub-actividad : ";
                    std::getline(std::cin,descripcion);
                    std::cout << "Indique a que tipo de actividad pertenece : ";
                    std::getline(std::cin,nombreActividad);
                    if (listaActividades->existeActividad(nombreActividad)){
                        auto * nuevaSubactividad = new SubActividad(nombreSubactividad,descripcion);
                        listaActividades->buscarActividad(nombreActividad)->insertarSubactividad(nuevaSubactividad);
                        std::cout << nuevaSubactividad->toString() << " registrada con exito.\n";
                    } else { std::cout << "No existe un tipo de actividad con el nombre : "+nombreActividad+"\n"; }
                } else { std::cout << "Ya existe una sub-actividad registrada con el nombre : "+nombreSubactividad+"\n"; }
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 7:{
                std::cout << "Modificar al personal\n\n";
                std::string nombrePersona;
                std::cout << "Digite el nombre de la persona a modificar : ";
                std::getline(std::cin, nombrePersona);
                std::cout << "\nEstas son las personas registradas bajo el nombre : " << nombrePersona << "\n";
                listaPersonas->buscarNombre(nombrePersona);
                std::cout << "Digite la cedula de la persona a modificar : ";
                int cedula = recibirInput<int>();
                Persona * persona = listaPersonas->buscar(cedula);
                // verificar que la persona haya sido encontrada
                if (persona) {
                    std::system(COMANDO_LIMPIAR_TERMINAL);
                    std::cout << "Persona a modificar\n";
                    std::cout << persona->toString() << "\n";
                    std::cout << "Digite la informacion a editar\n";
                    std::cout << "\t1. Agregar puesto\n";
                    std::cout << "\t2. Agregar formacion\n";
                    std::cout << "\t3. Agregar sub-actividad\n";
                    std::cout << "\t4. Modificar formacion\n";
                    std::cout << "\t5. Modificar residencia\n";
                    std::cout << "\t6. Modificar edad\n";
                    std::cout << "\t7. Modificar sub-actividad\n";
                    std::cout << "\t8. Eliminar puesto\n";
                    std::cout << "\t9. Eliminar formacion\n";
                    std::cout << "\t10. Eliminar sub-actividad\n";
                    std::cout << "\t11. Mostrar disponibilidad\n";
                    std::cout << "Digite la opcion ";
                    int opcionModificacion = recibirInput<int>();
                    switch (opcionModificacion){
                        case 1:{
                            std::cout << "Agregar puesto\n\n";
                            std::string nombrePuesto;
                            std::cout << "Digite el nombre del puesto : ";
                            std::getline(std::cin,nombrePuesto);
                            Puesto * puesto;
                            // Si el puesto ya existe entonces se le asigna a la persona
                            if (listaPuestos->existe(nombrePuesto))
                                puesto = listaPuestos->buscar(nombrePuesto);
                                // Si el puesto no existe todavia entonces se crea y se le asigna
                            else {
                                std::cout << "Puesto no registrado.\n";
                                std::cout << "Digite el salario minimo : ";
                                int salarioMinimo = recibirInput<int>();
                                puesto = new Puesto(nombrePuesto, salarioMinimo);
                                listaPuestos->insertar(puesto);
                                std::cout << puesto->toString() << " registrado con exito.\n";
                            }
                            // Se le asigna a la persona el puesto
                            persona->setPuesto(puesto);
                            std::cout << persona->toString() << "\n";
                            std::cout << "Puesto agregado con exito.\n";
                            break;
                        }
                        case 2:{
                            std::cout << "Agregar formacion\n\n";
                            std::string nombreFormacion;
                            std::cout << "Digite el nombre de la formacion : ";
                            std::getline(std::cin, nombreFormacion);
                            Formacion * formacion;
                            if (listaFormaciones->existe(nombreFormacion))
                                formacion = listaFormaciones->buscar(nombreFormacion);
                            else {
                                std::cout << "Formacion no registrada.\n";
                                formacion = new Formacion(nombreFormacion);
                                listaFormaciones->insertar(formacion);
                                std::cout << formacion->toString() << " registrada con exito.\n";
                            }
                            std::cout << "Digite el estado de la formacion\n";
                            std::cout << "\t1. Completa\n\t2. Incompleta";
                            std::cout << "\nDigite la opcion : ";
                            int opcionCompletada = recibirInput<int>();
                            if (opcionCompletada != 1 && opcionCompletada != 2){
                                std::cout << "\nOpcion invalida.\n";
                                break;
                            }
                            bool formacionCompleta = (opcionCompletada == 1);
                            std::cout << "\nDigite el ano de inicio : ";
                            int formacionInicio = recibirInput<int>();
                            int formacionFin = {};
                            if (formacionCompleta){
                                std::cout << "\nDigite el ano de finalizacion : ";
                                formacionFin = recibirInput<int>();
                            }
                            persona->setFormacion(formacion);
                            persona->setFormacionCompleta(formacionCompleta);
                            persona->setFormacionInicio(formacionInicio);
                            persona->setFormacionFin(formacionFin);
                            std::cout << persona->toString() << "\n";
                            std::cout << "Formacion agregada con exito.\n";
                            break;
                        }
                        case 3:{
                            std::cout << "Agregar actividad\n\n";
                            std::string nombreSubactividad;
                            std::cout << "Indique la subactividad a registrar : ";
                            std::getline(std::cin, nombreSubactividad);
                            SubActividad * subactividad;
                            // verificar si la sub-actividad existe
                            if (listaActividades->existeSubactividad(nombreSubactividad))
                                subactividad = listaActividades->buscarSubactividad(nombreSubactividad);
                            else {
                                std::string descripcionSubactividad, tipoActividad;
                                std::cout << "No esta registrada dicha sub-actividad. Digite una descripcion : ";
                                std::getline(std::cin, descripcionSubactividad);
                                subactividad = new SubActividad(nombreSubactividad,descripcionSubactividad);
                                std::cout << "Digite al tipo de actividad a la que pertenece : ";
                                std::getline(std::cin, tipoActividad);
                                // verificar si el tipo de actividad ya existe
                                if (listaActividades->existeActividad(tipoActividad))
                                    listaActividades->buscarActividad(tipoActividad)->insertarSubactividad(subactividad);
                                else {
                                    std::string descripcionActividad;
                                    std::cout << "No esta registrado dicho tipo de actividad. Digite una descripcion : ";
                                    std::getline(std::cin,descripcionActividad);
                                    auto * nuevaActividad = new Actividad(tipoActividad,descripcionActividad);
                                    nuevaActividad->insertarSubactividad(subactividad);
                                    listaActividades->insertar(nuevaActividad);
                                }
                            }
                            std::cout << "Digite la fecha (mes) : ";
                            int fechaMes = recibirInputRango<int>(1,12) - 1;
                            std::cout << "Digite la fecha (dia) : " << MESES[fechaMes] << "/";
                            int fechaDia = recibirInputRango<int>(1,30);
                            std::cout << "Digite la cantidad de horas dedicadas : ";
                            int cantidadHoras = recibirInputRango(0,24);
                            std::cout << "Digite la hora de inicio : ";
                            int horaInicio = recibirInputRango<int>(0,23);
                            // verificar si se puede agendar la actividad por cuestiones de tiempo
                            if (persona->posibleAgendar(fechaDia,fechaMes,cantidadHoras,horaInicio)){
                                auto * nuevaSubactividad = new SubActividad_Personal(cantidadHoras,horaInicio,fechaDia,fechaMes,subactividad);
                                persona->agregarSubactividad(nuevaSubactividad);
                                std::cout << persona->toString() << "\n";
                                std::cout << "Sub-actividad agregada con exito.\n";
                            } else { std::cout << "\nError : No hay disponibilidad de horario para esa hora.\n"; }
                            break;
                        }
                        case 4:{
                            std::cout << "Modificar la formacion\n\n";
                            if (persona->getFormacion()){
                                std::cout << "Digite el estado de la formacion\n";
                                std::cout << "\t1. Completa\n\t2. Incompleta";
                                std::cout << "\nDigite la opcion : ";
                                int opcionCompletada = recibirInput<int>();
                                if (opcionCompletada != 1 && opcionCompletada != 2) break;
                                bool formacionCompleta = (opcionCompletada == 1);
                                std::cout << "\nDigite el ano de inicio : ";
                                int formacionInicio = recibirInput<int>();
                                int formacionFin = {};
                                if (formacionCompleta){
                                    std::cout << "\nDigite el ano de finalizacion : ";
                                    formacionFin = recibirInput<int>();
                                }
                                persona->setFormacionCompleta(formacionCompleta);
                                persona->setFormacionInicio(formacionInicio);
                                persona->setFormacionFin(formacionFin);
                                std::cout << persona->toString() << "\n";
                                std::cout << "Formacion modificada con exito.\n";
                            } else { std::cout << "Esta persona no tiene una formacion registrada.\n";}
                            break;
                        }
                        case 5:{
                            std::cout << "Modificar la residencia\n\n";
                            std::string provincia, canton, distrito;
                            std::cout << "Digite la provincia de residencia : ";
                            std::getline(std::cin,provincia);
                            std::cout << "Digite el canton de residencia : ";
                            std::getline(std::cin,canton);
                            std::cout << "Digite el distrito de residencia : ";
                            std::getline(std::cin,distrito);
                            persona->setResidencia(distrito+","+canton+","+provincia);
                            std::cout << persona->toString() << "\n";
                            std::cout << "Residencia modificada con exito.\n";
                            break;
                        }
                        case 6:{
                            std::cout << "Modificar la edad\n\n";
                            std::cout << "Digite la edad : ";
                            int nuevaEdad = recibirInput<int>();
                            persona->setEdad(nuevaEdad);
                            std::cout << persona->toString() << "\n";
                            std::cout << "Edad modificada con exito.\n";
                            break;
                        }
                        case 7:{
                            std::cout << "Modificar una sub-actividad\n\n";
                            std::string nombreSubactividad;
                            std::cout << "Digite la sub-actividad a modificar : ";
                            std::getline(std::cin,nombreSubactividad);
                            // verificar que la subactividad exista
                            if (listaActividades->existeSubactividad(nombreSubactividad)){
                                // verificar que la subactividad este registrada para esta persona
                                SubActividad_Personal * subactividad_personal = persona->buscarSubactividadPersonal
                                        (listaActividades->buscarSubactividad(nombreSubactividad));
                                if (subactividad_personal){
                                    // Recuperar el horario original por si no hay disponibilidad volver a agendar el horario original
                                    int fechaMesActual = subactividad_personal->getFechaMes();
                                    int fechaDiaActual = subactividad_personal->getFechaDia();
                                    int horasActual = subactividad_personal->getHoras();
                                    int horaInicioActual = subactividad_personal->getHoraInicio();
                                    // Borrar el horario original para poder verificar si se puede reagendar correctamente
                                    persona->getListaSubactividades()->liberarHoras(fechaDiaActual,fechaMesActual,horaInicioActual,horasActual);
                                    // Pedir
                                    std::cout << "Digite la fecha (mes) : ";
                                    int fechaMes = recibirInputRango<int>(1,12) - 1;
                                    std::cout << "Digite la fecha (dia) : " << MESES[fechaMes] << "/";
                                    int fechaDia = recibirInputRango<int>(1,30);
                                    std::cout << "Digite la cantidad de horas dedicadas : ";
                                    int cantidadHoras = recibirInputRango(0,24);
                                    std::cout << "Digite la hora de inicio : ";
                                    int horaInicio = recibirInputRango<int>(0,23);
                                    // verificar si se puede agendar la actividad por cuestiones de tiempo
                                    if (persona->posibleAgendar(fechaDia,fechaMes,cantidadHoras,horaInicio)){
                                        // reagendar con el nuevo horario
                                        persona->getListaSubactividades()->registrarHoras(fechaDia,fechaMes,horaInicio,cantidadHoras);
                                        // modificar la informacion de la sub-actividad personal
                                        subactividad_personal->setFecha(fechaDia,fechaMes);
                                        subactividad_personal->setHoraInicio(horaInicio);
                                        subactividad_personal->setHoras(cantidadHoras);
                                        std::cout << persona->toString() << "\n";
                                        std::cout << "Sub-actividad modificada con exito.\n";
                                    } else {
                                        // Volver a reagendar el horario orignal
                                        persona->getListaSubactividades()->registrarHoras(fechaDiaActual,fechaMesActual,horaInicioActual,horasActual);
                                        std::cout << "\nError : No hay disponibilidad de horario para esa hora.\n";
                                    }
                                } else { std::cout << "Esta persona no tiene registrada dicha sub-actividad\n"; }
                            } else { std::cout << "No existe una sub-actividad registrada bajo el nombre : " << nombreSubactividad << "\n"; }
                            break;
                        }
                        case 8:{
                            std::cout << "Eliminar el puesto\n\n";
                            persona->setPuesto(nullptr);
                            std::cout << persona->toString() << "\n";
                            std::cout << "Puesto eliminado con exito.\n";
                            break;
                        }
                        case 9:{
                            std::cout << "Eliminar la formacion\n\n";
                            persona->setFormacion(nullptr);
                            std::cout << persona->toString() << "\n";
                            std::cout << "Formacion eliminada con exito.\n";
                            break;
                        }
                        case 10:{
                            std::cout << "Eliminar sub-actividad\n\n";
                            std::string nombreSubactividad;
                            std::cout << "Digite la sub-actividad a modificar : ";
                            std::getline(std::cin,nombreSubactividad);
                            // verificar que la subactividad exista
                            if (listaActividades->existeSubactividad(nombreSubactividad)){
                                SubActividad* subactividad = listaActividades->buscarSubactividad(nombreSubactividad);
                                // verificar que la subactividad este registrada para esta persona
                                if (persona->getListaSubactividades()->existe(subactividad)){
                                    persona->getListaSubactividades()->eliminar(subactividad);
                                    std::cout << persona->toString() << "\n";
                                    std::cout << "Sub-actividad eliminada con exito.\n";
                                } else { std::cout << "Esta persona no tiene registrada dicha sub-actividad\n"; }
                            } else { std::cout << "No existe una sub-actividad registrada bajo el nombre : " << nombreSubactividad << "\n"; }
                        }
                        case 11:{
                            std::cout << "Mostrar la disponibilidad de un dia\n\n";
                            std::cout << "Digite el dia : ";
                            int dia = recibirInputRango<int>(1,30);
                            int mes = recibirInputRango<int>(1,12) - 1;
                            persona->imprimirDisponibilidad(dia,mes);
                            break;
                        }
                        default:{
                            std::cout << "\nOpcion invalida\n";
                            break;
                        }
                    }
                } else { std::cout << "No existe ninguna persona registrada con esa cedula.\n"; }
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 11:{
                std::cout << "Modificar formacion\n\n";
                std::cout << "Digite el nombre de la formacion a modificar : ";
                std::string nombreFormacion;
                std::getline(std::cin,nombreFormacion);
                if (listaFormaciones->existe(nombreFormacion)){
                    Formacion * formacion = listaFormaciones->buscar(nombreFormacion);
                    std::string nuevoNombreFormacion;
                    std::cout << "Digite el nuevo nombre de la formacion : ";
                    std::getline(std::cin,nuevoNombreFormacion);
                    // Si el nuevo nombre de la formacion ya esta registrado en el sistema
                    // entonces se le notifica al usuario
                    if (listaFormaciones->existe(nuevoNombreFormacion))
                        std::cout << "Ya existe otra formacion con el nombre : "+nombreFormacion+"\n";
                    else{
                        formacion->setNombre(nuevoNombreFormacion);
                        std::cout << formacion->toString() << "\n";
                        std::cout << "Formacion modificada con exito.\n";
                    }
                } else { std::cout << "No existe ninguna formacion registrada con el nombre : "+nombreFormacion+"\n"; }
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 16:{
                std::cout << "Eliminar una persona del sistema\n\n";
                std::string nombrePersona;
                std::cout << "Digite el nombre de la persona a modificar : ";
                std::getline(std::cin, nombrePersona);
                std::cout << "\nEstas son las personas registradas bajo el nombre : " << nombrePersona << "\n";
                listaPersonas->buscarNombre(nombrePersona);
                std::cout << "Digite la cedula de la persona a modificar : ";
                int cedula = recibirInput<int>();
                // notificar el estado de la eliminacion
                bool eliminacion = listaPersonas->eliminar(cedula);
                if (eliminacion)
                    std::cout << "Persona eliminada con exito.\n";
                else
                    std::cout << "Error : no existe una persona registrada bajo la cedula : " << cedula << "\n";
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 20:{
                std::cout << "Imprimir las personas\n\n";
                listaPersonas->imprimir();
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 21:{
                std::cout << "Imprimir los oficios\n\n";
                listaPuestos->imprimir();
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 22:{
                std::cout << "Imprimir las formaciones\n\n";
                listaFormaciones->imprimir();
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 23:{
                std::cout << "Imprimir las actividades\n\n";
                listaActividades->imprimirActividades();
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 24:{
                std::cout << "Imprimir las sub-actividades\n\n";
                listaActividades->imprimirSubactividades();
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 25:{
                std::cout << "Imprimir los porcentajes de las formaciones del personal\n\n";
                imprimirPorcentajeFormacion(listaPersonas,listaFormaciones);
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 26:{
                std::cout << "Imprimir los porcentajes de los puestos del personal\n\n";
                imprimirPorcentajePuesto(listaPersonas,listaPuestos->getPrimerNodo());
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 32:{
                std::cout << "Mostrar la sub-actividad con mayor y menor consumo de tiempo\n\n";
                auto * mayor = getMayorConsumo(listaPersonas,listaActividades->getPrimerNodo());
                auto * menor = getMenorConsumo(listaPersonas,listaActividades->getPrimerNodo());
                if (mayor){
                    int consumoTiempoMayor = listaPersonas->consumoTiempoSubactividad(mayor);
                    int consumoTiempoMenor = listaPersonas->consumoTiempoSubactividad(menor);
                    std::cout << "Sub-actividad con mayor tiempo consumido : " << mayor->toString() << " | Tiempo consumido : " <<
                    consumoTiempoMayor << " horas\n";
                    std::cout << "Sub-actividad con menor tiempo consumido : " << menor->toString() << " | Tiempo consumido : " <<
                    consumoTiempoMenor << " horas\n";
                } else
                    std::cout << "No hay sub-actividades registradas.\n";
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 33:{
                std::cout << "Cantidad de tiempo consumido por tipo de actividad\n\n";
                getConsumoActividades(listaPersonas,listaActividades->getPrimerNodo());
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 34:{
                std::cout << "Tipo de actividad sin consumo de tiempo\n\n";
                getActividadCeroConsumo(listaPersonas,listaActividades->getPrimerNodo());
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 35:{
                std::cout << "Consultar la distribucion del tiempo de una persona por fecha\n\n";
                std::string nombrePersona;
                std::cout << "Digite el nombre de la persona a modificar : ";
                std::getline(std::cin, nombrePersona);
                std::cout << "\nEstas son las personas registradas bajo el nombre : " << nombrePersona << "\n";
                listaPersonas->buscarNombre(nombrePersona);
                std::cout << "Digite la cedula de la persona a modificar : ";
                int cedula = recibirInput<int>();
                Persona * persona = listaPersonas->buscar(cedula);
                if (persona) {
                    std::cout << "Digite la fecha (mes) : ";
                    int fechaMes = recibirInputRango<int>(1,12) - 1;
                    std::cout << "Digite la fecha (dia) : " << MESES[fechaMes] << "/";
                    int fechaDia = recibirInputRango<int>(1,30);
                    persona->imprimirDistribucionFecha(fechaDia,fechaMes);
                } else { std::cout << "No existe ninguna persona registrada con esa cedula.\n"; }
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 36:{
                std::cout << "Consultar la distribucion del tiempo de una semana\n\n";
                std::string nombrePersona;
                std::cout << "Digite el nombre de la persona a modificar : ";
                std::getline(std::cin, nombrePersona);
                std::cout << "\nEstas son las personas registradas bajo el nombre : " << nombrePersona << "\n";
                listaPersonas->buscarNombre(nombrePersona);
                std::cout << "Digite la cedula de la persona a modificar : ";
                int cedula = recibirInput<int>();
                Persona * persona = listaPersonas->buscar(cedula);
                if (persona) {
                    std::cout << "Digite la fecha (mes) : ";
                    int fechaMes = recibirInputRango<int>(1,12) - 1;
                    std::cout << "Digite la fecha (dia) : " << MESES[fechaMes] << "/";
                    int fechaDia = recibirInputRango<int>(1,30);
                    persona->imprimirDistribucionSemana(fechaDia,fechaMes);
                } else { std::cout << "No existe ninguna persona registrada con esa cedula.\n"; }
                std::cout << "\nPresione Enter para continuar ";
                std::getchar();
                break;
            }
            case 45:{
                std::cout << "Hasta luego!\n";
                sleep(2);
                break;
            }
            default:{
                std::cout << "\nOpcion invalida\n";
                sleep(2);
                break;
            }
        }
    } while (decision != 45);
    return 0;
}
