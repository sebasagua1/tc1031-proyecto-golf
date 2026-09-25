#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "swing.h"
#include "sorts.h"
using namespace std;

// Lee los swings del archivo csv
vector<Swing> leerSwings(string nombre) {
    vector<Swing> swings;
    ifstream archivo(nombre);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << nombre << endl;
        return swings;
    }

    string linea;
    getline(archivo, linea); // saltar los titulos

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string palo, dist, vel;
        getline(ss, palo, ',');
        getline(ss, dist, ',');
        getline(ss, vel, ',');
        swings.push_back(Swing(palo, stoi(dist), stoi(vel)));
    }

    archivo.close();
    return swings;
}

void mostrar(vector<Swing> &swings) {
    cout << endl << "Palo\tDistancia\tVelocidad" << endl;
    for (int i = 0; i < (int)swings.size(); i++) {
        swings[i].imprimir();
    }
    cout << endl;
}

int main() {
    vector<Swing> swings = leerSwings("swings.csv");

    int opcion = -1;
    while (opcion != 0) {
        cout << "1. Ver mis swings" << endl;
        cout << "2. Ordenar por distancia" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            mostrar(swings);
        } else if (opcion == 2) {
            bubbleSort(swings);
            mostrar(swings);
        }
    }

    return 0;
}
