#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <string>
#include <cctype>
#include "swing.h"
using namespace std;

// Pasa un texto a minusculas para que "driver" y "Driver" cuenten igual
string minusculas(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

// Nodo de la lista: guarda un swing y apunta al anterior y al siguiente
class Nodo {
public:
    Swing dato;
    Nodo *ant;
    Nodo *sig;

    Nodo(Swing s) {
        dato = s;
        ant = nullptr;
        sig = nullptr;
    }
};

// Lista doblemente ligada con mis swings en el orden en que los pegue.
// head es el primer swing que registre y tail el mas reciente.
class ListaSwings {
private:
    Nodo *head;
    Nodo *tail;
    int tam;

    void cambiar(Nodo *a, Nodo *b) {
        Swing temp = a->dato;
        a->dato = b->dato;
        b->dato = temp;
    }

    // Particion de quicksort: usa como pivote la distancia del ultimo nodo,
    // deja antes del pivote los swings mas largos y despues los mas cortos.
    // Regresa el nodo donde quedo el pivote.
    Nodo* particion(Nodo *bajo, Nodo *alto) {
        int pivote = alto->dato.getDistancia();
        Nodo *i = bajo->ant;

        for (Nodo *j = bajo; j != alto; j = j->sig) {
            if (j->dato.getDistancia() > pivote) {
                if (i == nullptr) {
                    i = bajo;
                } else {
                    i = i->sig;
                }
                cambiar(i, j);
            }
        }

        if (i == nullptr) {
            i = bajo;
        } else {
            i = i->sig;
        }
        cambiar(i, alto);
        return i;
    }

    // Quicksort entre los nodos bajo y alto (incluidos)
    void quickSort(Nodo *bajo, Nodo *alto) {
        if (alto != nullptr && bajo != alto && bajo != alto->sig) {
            Nodo *p = particion(bajo, alto);
            quickSort(bajo, p->ant);
            quickSort(p->sig, alto);
        }
    }

public:
    ListaSwings() {
        head = nullptr;
        tail = nullptr;
        tam = 0;
    }

    ~ListaSwings() {
        Nodo *p = head;
        while (p != nullptr) {
            Nodo *siguiente = p->sig;
            delete p;
            p = siguiente;
        }
    }

    int getTam() { return tam; }

    // Agrega un swing al final, porque es el mas reciente
    void agregarFinal(Swing s) {
        Nodo *nuevo = new Nodo(s);
        if (head == nullptr) {
            head = nuevo;
            tail = nuevo;
        } else {
            tail->sig = nuevo;
            nuevo->ant = tail;
            tail = nuevo;
        }
        tam++;
    }

    // Borra el swing de la posicion pos (empieza en 1).
    // Si esta en la primera mitad lo busca desde head y si no desde tail.
    bool eliminar(int pos, Swing &borrado) {
        if (pos < 1 || pos > tam) {
            return false;
        }

        Nodo *p;
        if (pos <= tam / 2) {
            p = head;
            for (int i = 1; i < pos; i++) {
                p = p->sig;
            }
        } else {
            p = tail;
            for (int i = tam; i > pos; i--) {
                p = p->ant;
            }
        }

        if (p->ant != nullptr) {
            p->ant->sig = p->sig;
        } else {
            head = p->sig;
        }
        if (p->sig != nullptr) {
            p->sig->ant = p->ant;
        } else {
            tail = p->ant;
        }

        borrado = p->dato;
        delete p;
        tam--;
        return true;
    }

    // Muestra todos los swings en el orden en que los pegue
    void mostrar() {
        cout << endl << "#\tPalo\tDistancia\tVelocidad" << endl;
        int pos = 1;
        for (Nodo *p = head; p != nullptr; p = p->sig) {
            cout << pos << "\t";
            p->dato.imprimir();
            pos++;
        }
        cout << endl;
    }

    // Muestra los ultimos k swings, del mas reciente al mas viejo.
    // Empieza en tail y se va hacia atras con ant.
    void mostrarUltimos(int k) {
        cout << endl << "#\tPalo\tDistancia\tVelocidad" << endl;
        int pos = tam;
        Nodo *p = tail;
        while (p != nullptr && k > 0) {
            cout << pos << "\t";
            p->dato.imprimir();
            p = p->ant;
            pos--;
            k--;
        }
        cout << endl;
    }

    // Muestra los swings de mayor a menor distancia.
    // Ordena una copia para no perder el orden en que los pegue.
    void mostrarPorDistancia() {
        ListaSwings copia;
        for (Nodo *p = head; p != nullptr; p = p->sig) {
            copia.agregarFinal(p->dato);
        }
        copia.quickSort(copia.head, copia.tail);

        cout << endl << "Lugar\tPalo\tDistancia\tVelocidad" << endl;
        int lugar = 1;
        for (Nodo *p = copia.head; p != nullptr; p = p->sig) {
            cout << lugar << "\t";
            p->dato.imprimir();
            lugar++;
        }
        cout << endl;
    }

    // Muestra los swings de un palo con su promedio y su mejor tiro.
    // Regresa cuantos swings encontro.
    int buscarPalo(string palo) {
        int cuantos = 0;
        int sumaDist = 0;
        int sumaVel = 0;
        int mejor = 0;

        cout << endl << "#\tPalo\tDistancia\tVelocidad" << endl;
        int pos = 1;
        for (Nodo *p = head; p != nullptr; p = p->sig) {
            if (minusculas(p->dato.getPalo()) == minusculas(palo)) {
                cout << pos << "\t";
                p->dato.imprimir();
                cuantos++;
                sumaDist += p->dato.getDistancia();
                sumaVel += p->dato.getVelocidad();
                if (p->dato.getDistancia() > mejor) {
                    mejor = p->dato.getDistancia();
                }
            }
            pos++;
        }

        if (cuantos == 0) {
            cout << "No hay swings con " << palo << endl << endl;
        } else {
            cout << endl << "Swings: " << cuantos << endl;
            cout << "Distancia promedio: " << sumaDist / cuantos << " yd" << endl;
            cout << "Velocidad promedio: " << sumaVel / cuantos << " mph" << endl;
            cout << "Mejor distancia: " << mejor << " yd" << endl << endl;
        }
        return cuantos;
    }
};

#endif
