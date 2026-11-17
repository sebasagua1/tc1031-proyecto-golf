#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include "swing.h"
#include "lista.h"
#include "arbol.h"
using namespace std;

// Revisa que un texto sea un numero entero mayor a 0 (de maximo 6 digitos),
// para poder pasarlo a stoi sin que el programa truene
bool esNumero(string s) {
    if (s == "" || s.size() > 6) {
        return false;
    }
    for (int i = 0; i < (int)s.size(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return stoi(s) > 0;
}

// Lee los swings del archivo csv y los va agregando al final de la lista
// y al arbol ordenado por distancia
void leerSwings(string nombre, ListaSwings &lista, ArbolSwings &arbol) {
    ifstream archivo(nombre);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << nombre << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // saltar los titulos

    while (getline(archivo, linea)) {
        // quitar el \r que dejan los archivos guardados en Windows
        if (linea != "" && linea[linea.size() - 1] == '\r') {
            linea.erase(linea.size() - 1);
        }
        if (linea == "") {
            continue;
        }
        stringstream ss(linea);
        string palo, dist, vel;
        getline(ss, palo, ',');
        getline(ss, dist, ',');
        getline(ss, vel, ',');
        // si al renglon le falta el palo o sus numeros no son validos, se lo salta
        if (palo == "" || !esNumero(dist) || !esNumero(vel)) {
            cout << "Se salto un renglon con datos no validos: " << linea << endl;
            continue;
        }
        Swing s(palo, stoi(dist), stoi(vel));
        lista.agregarFinal(s);
        arbol.insertar(s);
    }

    archivo.close();
}

// Guarda un swing nuevo al final del archivo, sin reescribir los que ya estaban.
// Si el archivo no existe o esta vacio, primero escribe los titulos.
bool guardarNuevo(string nombre, Swing s) {
    bool tieneDatos = false;
    bool faltaSalto = false;

    // Reviso si el archivo ya tiene algo y si termina en salto de linea,
    // para no pegar el swing nuevo al final del ultimo renglon
    ifstream lectura(nombre);
    if (lectura.is_open()) {
        lectura.seekg(0, ios::end);
        if (lectura.tellg() > 0) {
            tieneDatos = true;
            lectura.seekg(-1, ios::end);
            char ultimo;
            lectura.get(ultimo);
            faltaSalto = (ultimo != '\n');
        }
        lectura.close();
    }

    ofstream archivo(nombre, ios::app);
    if (!archivo.is_open()) {
        return false;
    }
    if (!tieneDatos) {
        archivo << "palo,distancia,velocidad" << endl;
    }
    if (faltaSalto) {
        archivo << endl;
    }
    archivo << s.aCSV() << endl;
    return archivo.good();
}

// Reescribe el archivo completo con los swings de la lista, en el mismo orden.
// Se usa despues de borrar un swing.
bool guardarTodos(string nombre, ListaSwings &lista) {
    ofstream archivo(nombre);
    if (!archivo.is_open()) {
        return false;
    }
    archivo << "palo,distancia,velocidad" << endl;
    lista.guardar(archivo);
    return archivo.good();
}

// Lee un numero del teclado, regresa -1 si lo que escribieron no es numero
int leerNumero() {
    int x;
    if (cin >> x) {
        return x;
    }
    if (!cin.eof()) {
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return -1;
}

int main() {
    ListaSwings swings;
    ArbolSwings arbol;
    leerSwings("swings.csv", swings, arbol);
    cout << "Se cargaron " << swings.getTam() << " swings" << endl << endl;

    int opcion = -1;
    while (opcion != 0) {
        cout << "1. Ver mis swings" << endl;
        cout << "2. Ver mis ultimos swings" << endl;
        cout << "3. Ordenar por distancia" << endl;
        cout << "4. Buscar por palo" << endl;
        cout << "5. Agregar un swing" << endl;
        cout << "6. Borrar un swing" << endl;
        cout << "7. Que palo uso" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        opcion = leerNumero();

        if (cin.eof()) {
            break;
        }

        if (opcion == 1) {
            swings.mostrar();
        } else if (opcion == 2) {
            cout << "Cuantos swings quieres ver: ";
            int k = leerNumero();
            if (k > 0) {
                swings.mostrarUltimos(k);
            } else {
                cout << "Numero no valido" << endl << endl;
            }
        } else if (opcion == 3) {
            swings.mostrarPorDistancia();
        } else if (opcion == 4) {
            cout << "Palo (Driver, Madera3, Hibrido, Hierro5, Hierro7, Hierro9, PW, SW): ";
            string palo;
            cin >> palo;
            swings.buscarPalo(palo);
        } else if (opcion == 5) {
            string palo;
            cout << "Palo: ";
            cin >> palo;
            cout << "Distancia (yd): ";
            int d = leerNumero();
            cout << "Velocidad (mph): ";
            int v = leerNumero();
            if (palo.find(',') != string::npos) {
                // una coma en el palo romperia el renglon en swings.csv
                cout << "El palo no puede tener comas" << endl << endl;
            } else if (d > 0 && v > 0 && d <= 999999 && v <= 999999) {
                // el mismo limite de 6 digitos que usa esNumero al leer el archivo
                Swing nuevo(palo, d, v);
                swings.agregarFinal(nuevo);
                arbol.insertar(nuevo);
                if (guardarNuevo("swings.csv", nuevo)) {
                    cout << "Swing agregado y guardado en swings.csv";
                } else {
                    cout << "Swing agregado, pero no se pudo guardar en swings.csv";
                }
                cout << ", ahora tienes " << swings.getTam() << endl << endl;
            } else {
                cout << "La distancia y la velocidad tienen que ser numeros entre 1 y 999999" << endl << endl;
            }
        } else if (opcion == 6) {
            cout << "Numero del swing a borrar (el # de la opcion 1): ";
            int pos = leerNumero();
            Swing borrado;
            if (swings.eliminar(pos, borrado)) {
                cout << "Se borro: ";
                borrado.imprimir();
                arbol.eliminar(borrado);
                if (guardarTodos("swings.csv", swings)) {
                    cout << "Se actualizo swings.csv" << endl << endl;
                } else {
                    cout << "No se pudo actualizar swings.csv" << endl << endl;
                }
            } else if (pos == -1) {
                cout << "Numero no valido" << endl << endl;
            } else {
                cout << "No existe el swing " << pos << endl << endl;
            }
        } else if (opcion == 7) {
            cout << "Cuantas yardas te quedan: ";
            int yardas = leerNumero();
            if (yardas > 0 && yardas <= 999999) {
                // busca los swings que cayeron a 5 yardas o menos de esa distancia
                arbol.recomendar(yardas, 5);
            } else {
                cout << "Numero no valido" << endl << endl;
            }
        } else if (opcion != 0) {
            cout << "Opcion no valida" << endl << endl;
        }
    }

    return 0;
}
