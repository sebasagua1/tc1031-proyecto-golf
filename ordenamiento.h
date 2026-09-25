#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include <vector>
#include "tiro.h"
using namespace std;

// criterio 1 = por distancia (de mayor a menor)
// criterio 2 = por palo (orden alfabetico)
bool vaPrimero(Tiro a, Tiro b, int criterio) {
    if (criterio == 1) {
        return a.getDistancia() >= b.getDistancia();
    }
    return a.getPalo() <= b.getPalo();
}

// Junta las dos mitades ya ordenadas: v[ini..mid] y v[mid+1..fin]
void merge(vector<Tiro> &v, int ini, int mid, int fin, int criterio) {
    vector<Tiro> aux;
    int i = ini;
    int j = mid + 1;

    while (i <= mid && j <= fin) {
        if (vaPrimero(v[i], v[j], criterio)) {
            aux.push_back(v[i]);
            i++;
        } else {
            aux.push_back(v[j]);
            j++;
        }
    }
    // lo que haya sobrado de cada mitad
    while (i <= mid) {
        aux.push_back(v[i]);
        i++;
    }
    while (j <= fin) {
        aux.push_back(v[j]);
        j++;
    }

    for (int k = 0; k < (int)aux.size(); k++) {
        v[ini + k] = aux[k];
    }
}

void mergeSort(vector<Tiro> &v, int ini, int fin, int criterio) {
    if (ini >= fin) {
        return;
    }
    int mid = (ini + fin) / 2;
    mergeSort(v, ini, mid, criterio);
    mergeSort(v, mid + 1, fin, criterio);
    merge(v, ini, mid, fin, criterio);
}

void ordenar(vector<Tiro> &v, int criterio) {
    mergeSort(v, 0, v.size() - 1, criterio);
}

#endif
