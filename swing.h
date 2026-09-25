#ifndef SWING_H
#define SWING_H

#include <iostream>
#include <string>
using namespace std;

// Datos de un swing que hice en el range
class Swing {
private:
    string palo;
    int distancia;   // yardas
    int velocidad;   // velocidad de la bola en mph

public:
    Swing() {
        palo = "";
        distancia = 0;
        velocidad = 0;
    }

    Swing(string p, int d, int v) {
        palo = p;
        distancia = d;
        velocidad = v;
    }

    string getPalo() { return palo; }
    int getDistancia() { return distancia; }
    int getVelocidad() { return velocidad; }

    void imprimir() {
        cout << palo << "\t" << distancia << " yd\t" << velocidad << " mph" << endl;
    }
};

#endif
