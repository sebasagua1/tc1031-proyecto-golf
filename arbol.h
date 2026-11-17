#ifndef ARBOL_H
#define ARBOL_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "swing.h"
#include "lista.h"
using namespace std;

// Nodo del arbol: guarda un swing y apunta a su hijo izquierdo y al derecho
class NodoArbol {
public:
    Swing dato;
    NodoArbol *izq;
    NodoArbol *der;

    NodoArbol(Swing s) {
        dato = s;
        izq = nullptr;
        der = nullptr;
    }
};

// Arbol binario de busqueda con mis swings ordenados por distancia.
// A la izquierda van los swings mas cortos y a la derecha los que tienen
// la misma distancia o mas (puede haber varios swings con la misma distancia).
class ArbolSwings {
private:
    NodoArbol *raiz;
    int tam;

    // Baja por el arbol hasta encontrar el lugar del swing y lo pone ahi
    NodoArbol* insertar(NodoArbol *n, Swing s) {
        if (n == nullptr) {
            return new NodoArbol(s);
        }
        if (s.getDistancia() < n->dato.getDistancia()) {
            n->izq = insertar(n->izq, s);
        } else {
            n->der = insertar(n->der, s);
        }
        return n;
    }

    // Quita el nodo con la distancia mas chica del subarbol n y regresa su
    // swing en minimo. Regresa como queda la raiz del subarbol.
    NodoArbol* quitarMinimo(NodoArbol *n, Swing &minimo) {
        if (n->izq == nullptr) {
            minimo = n->dato;
            NodoArbol *derecha = n->der;
            delete n;
            return derecha;
        }
        n->izq = quitarMinimo(n->izq, minimo);
        return n;
    }

    // Busca un swing con la misma distancia, palo y velocidad y lo borra.
    // Regresa como queda la raiz del subarbol.
    NodoArbol* eliminar(NodoArbol *n, Swing s, bool &borrado) {
        if (n == nullptr) {
            return nullptr;
        }
        int d = s.getDistancia();

        if (d < n->dato.getDistancia()) {
            n->izq = eliminar(n->izq, s, borrado);
        } else if (d > n->dato.getDistancia() ||
                   n->dato.getPalo() != s.getPalo() ||
                   n->dato.getVelocidad() != s.getVelocidad()) {
            // si tiene la misma distancia pero es otro swing, los que tienen
            // la misma distancia quedaron a la derecha
            n->der = eliminar(n->der, s, borrado);
        } else {
            borrado = true;
            if (n->izq == nullptr) {
                NodoArbol *derecha = n->der;
                delete n;
                return derecha;
            }
            if (n->der == nullptr) {
                NodoArbol *izquierda = n->izq;
                delete n;
                return izquierda;
            }
            // tiene dos hijos: lo cambio por el swing mas corto de la derecha
            Swing sucesor;
            n->der = quitarMinimo(n->der, sucesor);
            n->dato = sucesor;
        }
        return n;
    }

    // Recorre en orden (izquierda, nodo, derecha) para que salgan de menor a
    // mayor distancia, pero solo entra a las ramas donde puede haber swings
    // entre min y max
    void buscarRango(NodoArbol *n, int min, int max, vector<Swing> &encontrados) {
        if (n == nullptr) {
            return;
        }
        int d = n->dato.getDistancia();
        if (d > min) {
            buscarRango(n->izq, min, max, encontrados);
        }
        if (d >= min && d <= max) {
            encontrados.push_back(n->dato);
        }
        if (d <= max) {
            buscarRango(n->der, min, max, encontrados);
        }
    }

    // Busca el swing con la distancia mas cercana a las yardas.
    // Baja por el arbol como si buscara las yardas y se queda con el mas cercano.
    bool masCercano(int yardas, Swing &cercano) {
        NodoArbol *n = raiz;
        int mejorDif = -1;
        while (n != nullptr) {
            int dif = abs(n->dato.getDistancia() - yardas);
            if (mejorDif == -1 || dif < mejorDif) {
                mejorDif = dif;
                cercano = n->dato;
            }
            if (yardas < n->dato.getDistancia()) {
                n = n->izq;
            } else {
                n = n->der;
            }
        }
        return mejorDif != -1;
    }

    void borrarTodo(NodoArbol *n) {
        if (n == nullptr) {
            return;
        }
        borrarTodo(n->izq);
        borrarTodo(n->der);
        delete n;
    }

public:
    ArbolSwings() {
        raiz = nullptr;
        tam = 0;
    }

    ~ArbolSwings() {
        borrarTodo(raiz);
    }

    int getTam() { return tam; }

    void insertar(Swing s) {
        raiz = insertar(raiz, s);
        tam++;
    }

    // Regresa true si encontro el swing y lo borro
    bool eliminar(Swing s) {
        bool borrado = false;
        raiz = eliminar(raiz, s, borrado);
        if (borrado) {
            tam--;
        }
        return borrado;
    }

    // Con las yardas que me quedan, busca los swings que cayeron cerca
    // (yardas +- margen) y recomienda el palo con el que mas veces he llegado
    // a esa distancia. Si dos palos empatan, gana el que tiene el promedio
    // mas cerca de las yardas.
    void recomendar(int yardas, int margen) {
        int min = yardas - margen;
        int max = yardas + margen;
        vector<Swing> cerca;
        buscarRango(raiz, min, max, cerca);

        if (cerca.size() == 0) {
            Swing cercano;
            if (masCercano(yardas, cercano)) {
                cout << endl << "No tienes swings entre " << min << " y " << max << " yd." << endl;
                cout << "El que mas se acerca: ";
                cercano.imprimir();
                cout << endl;
            } else {
                cout << endl << "Todavia no tienes swings" << endl << endl;
            }
            return;
        }

        cout << endl << "Tus swings entre " << min << " y " << max << " yd:" << endl;
        cout << "Palo\tDistancia\tVelocidad" << endl;
        for (int i = 0; i < (int)cerca.size(); i++) {
            cerca[i].imprimir();
        }

        // Cuento cuantos swings y cuantas yardas lleva cada palo en el rango
        vector<string> palos;
        vector<int> cuantos;
        vector<int> suma;
        for (int i = 0; i < (int)cerca.size(); i++) {
            int j = 0;
            while (j < (int)palos.size() && minusculas(palos[j]) != minusculas(cerca[i].getPalo())) {
                j++;
            }
            if (j == (int)palos.size()) {
                palos.push_back(cerca[i].getPalo());
                cuantos.push_back(0);
                suma.push_back(0);
            }
            cuantos[j]++;
            suma[j] += cerca[i].getDistancia();
        }

        cout << endl;
        int mejor = 0;
        for (int j = 0; j < (int)palos.size(); j++) {
            double promedio = (double)suma[j] / cuantos[j];
            cout << palos[j] << ": " << cuantos[j] << (cuantos[j] == 1 ? " swing" : " swings")
                 << ", promedio " << (int)(promedio + 0.5) << " yd" << endl;

            double difJ = abs(promedio - yardas);
            double difMejor = abs((double)suma[mejor] / cuantos[mejor] - yardas);
            if (cuantos[j] > cuantos[mejor] || (cuantos[j] == cuantos[mejor] && difJ < difMejor)) {
                mejor = j;
            }
        }

        cout << endl << "Te recomiendo el " << palos[mejor] << ": ";
        if (cuantos[mejor] == 1) {
            cout << "1 de tus swings con ese palo cayo";
        } else {
            cout << cuantos[mejor] << " de tus swings con ese palo cayeron";
        }
        cout << " entre " << min << " y " << max << " yd" << endl << endl;
    }
};

#endif
