#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "tiro.h"
#include "ordenamiento.h"
using namespace std;

// Lee el csv y regresa todos los tiros en un vector
vector<Tiro> leerArchivo(string nombre) {
    vector<Tiro> tiros;
    ifstream archivo(nombre);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << nombre << endl;
        return tiros;
    }

    string linea;
    getline(archivo, linea); // la primera linea son los titulos

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string fecha, palo, dist, vel;
        getline(ss, fecha, ',');
        getline(ss, palo, ',');
        getline(ss, dist, ',');
        getline(ss, vel, ',');
        tiros.push_back(Tiro(fecha, palo, stoi(dist), stoi(vel)));
    }

    archivo.close();
    return tiros;
}

void mostrar(vector<Tiro> &tiros) {
    cout << endl << "Fecha       Palo      Distancia  Velocidad" << endl;
    for (int i = 0; i < (int)tiros.size(); i++) {
        tiros[i].imprimir();
    }
    cout << endl;
}

int main() {
    vector<Tiro> tiros = leerArchivo("tiros.csv");
    cout << "Se cargaron " << tiros.size() << " tiros" << endl;

    int opcion = -1;
    while (opcion != 0) {
        cout << "---- Mis tiros de golf ----" << endl;
        cout << "1. Ver tiros" << endl;
        cout << "2. Ordenar por distancia (mayor a menor)" << endl;
        cout << "3. Ordenar por palo" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            mostrar(tiros);
        } else if (opcion == 2) {
            ordenar(tiros, 1);
            mostrar(tiros);
        } else if (opcion == 3) {
            ordenar(tiros, 2);
            mostrar(tiros);
        } else if (opcion != 0) {
            cout << "Opcion no valida" << endl;
        }
    }

    return 0;
}
