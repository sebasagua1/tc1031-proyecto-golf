#ifndef TIRO_H
#define TIRO_H

#include <iostream>
#include <string>
using namespace std;

// Guarda los datos de un tiro que hice en el range
class Tiro {
private:
    string fecha;
    string palo;
    int distancia;       // en yardas
    int velocidad;       // velocidad de la bola en mph

public:
    Tiro() {
        fecha = "";
        palo = "";
        distancia = 0;
        velocidad = 0;
    }

    Tiro(string f, string p, int d, int v) {
        fecha = f;
        palo = p;
        distancia = d;
        velocidad = v;
    }

    string getFecha() { return fecha; }
    string getPalo() { return palo; }
    int getDistancia() { return distancia; }
    int getVelocidad() { return velocidad; }

    void imprimir() {
        cout << fecha << "  " << palo;
        // para que se vea alineado
        for (int i = palo.length(); i < 10; i++) {
            cout << " ";
        }
        cout << distancia << " yd   " << velocidad << " mph" << endl;
    }
};

#endif
