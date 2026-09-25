#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include "swing.h"
using namespace std;

// Bubble sort: ordena los swings por distancia de mayor a menor.
// Compara cada par de swings vecinos y los cambia si estan al reves.
void bubbleSort(vector<Swing> &v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        bool huboCambio = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (v[j].getDistancia() < v[j + 1].getDistancia()) {
                Swing temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
                huboCambio = true;
            }
        }
        // si no se cambio nada ya esta ordenado
        if (!huboCambio) {
            break;
        }
    }
}

#endif
