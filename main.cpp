#include <iostream>
#include <fstream>
#include <sstream>
#include "swing.h"
#include "lista.h"
using namespace std;

// Lee los swings del archivo csv y los va agregando al final de la lista
void leerSwings(string nombre, ListaSwings &lista) {
    ifstream archivo(nombre);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << nombre << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // saltar los titulos

    while (getline(archivo, linea)) {
        if (linea == "") {
            continue;
        }
        stringstream ss(linea);
        string palo, dist, vel;
        getline(ss, palo, ',');
        getline(ss, dist, ',');
        getline(ss, vel, ',');
        lista.agregarFinal(Swing(palo, stoi(dist), stoi(vel)));
    }

    archivo.close();
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
    leerSwings("swings.csv", swings);
    cout << "Se cargaron " << swings.getTam() << " swings" << endl << endl;

    int opcion = -1;
    while (opcion != 0) {
        cout << "1. Ver mis swings" << endl;
        cout << "2. Ver mis ultimos swings" << endl;
        cout << "3. Ordenar por distancia" << endl;
        cout << "4. Buscar por palo" << endl;
        cout << "5. Agregar un swing" << endl;
        cout << "6. Borrar un swing" << endl;
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
            if (d > 0 && v > 0) {
                swings.agregarFinal(Swing(palo, d, v));
                cout << "Swing agregado, ahora tienes " << swings.getTam() << endl << endl;
            } else {
                cout << "La distancia y la velocidad tienen que ser numeros mayores a 0" << endl << endl;
            }
        } else if (opcion == 6) {
            cout << "Numero del swing a borrar (el # de la opcion 1): ";
            int pos = leerNumero();
            Swing borrado;
            if (swings.eliminar(pos, borrado)) {
                cout << "Se borro: ";
                borrado.imprimir();
                cout << endl;
            } else {
                cout << "No existe el swing " << pos << endl << endl;
            }
        } else if (opcion != 0) {
            cout << "Opcion no valida" << endl << endl;
        }
    }

    return 0;
}
