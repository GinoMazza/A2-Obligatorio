#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>

using namespace std;

class CPEGrafo
{
private:
    struct nodoHeap
    {
        int idMision;
        int tiempo;
        nodoHeap(int i, int t) : idMision(i), tiempo(t) {}
    };

    nodoHeap **vec;
    int *posiciones;
    int capacidad;
    int cantElem;
    int primeroLibre;

    bool estaLleno()
    {
        return primeroLibre > capacidad;
    }

    bool existe(int id)
    {
        return posiciones[id] != 0;
    }

    int padre(int pos)
    {
        return pos / 2;
    }

    int hijoIzq(int pos)
    {
        return pos * 2;
    }

    int hijoDer(int pos)
    {
        return (pos * 2) + 1;
    }

    // Se actualiza el tiempo del nodo (flota o se hunde)
    void actualizar(int id, int tiempo)
    {
        int pos = posiciones[id];
        vec[pos]->tiempo = tiempo;
        flotar(pos);
        hundir(pos);
    }

    // Funcion que devuelve true si hay que intercambiar los nodos
    bool comparar(int posPadre, int pos)
    {
        bool intercambia = false;
        if (vec[posPadre]->tiempo > vec[pos]->tiempo)
            intercambia = true;
        else if (vec[posPadre]->tiempo == vec[pos]->tiempo)
        {
            if (vec[posPadre]->idMision > vec[pos]->idMision)
                intercambia = true;
        }
        return intercambia;
    }

    // Se intercambian los nodos
    void intercambiar(int posPadre, int pos)
    {
        // Intercambiamos en el vector
        int aux1 = posiciones[posPadre];
        posiciones[posPadre] = posiciones[pos];
        posiciones[pos] = aux1;

        // Intercambiamos en el heap
        nodoHeap *aux2 = vec[posPadre];
        vec[posPadre] = vec[pos];
        vec[pos] = aux2;
    }

    // Funcion que flota el nodo recibido
    void flotar(int pos)
    {
        // CB
        if (pos == 1)
            return;

        int posPadre = padre(pos);

        // Si da true, hay que intercambiar
        if (comparar(posPadre, pos))
        {
            intercambiar(posPadre, pos);
            flotar(posPadre);
        }
    }

    // Funcion que hunde el nodo recibido
    void hundir(int pos)
    {
        int izq = hijoIzq(pos);
        int der = hijoDer(pos);

        // CB
        if (izq >= primeroLibre && der >= primeroLibre)
            return;

        int hijoMenorPrioridad = izq;
        if (der < primeroLibre)
        {
            if (comparar(izq, der))
            {
                hijoMenorPrioridad = der;
            }
        }
        if (comparar(pos, hijoMenorPrioridad))
        {
            intercambiar(pos, hijoMenorPrioridad);
            hundir(hijoMenorPrioridad);
        }
    }

    void destruir()
    {
        for (int i = 0; i < capacidad; i++)
        {
            delete vec[i];
            vec[i] = NULL;
        }
        delete[] vec;
        delete[] posiciones;
        vec = NULL;
        posiciones = NULL;
    }

public:
    // Constructor
    CPEGrafo(int cap)
    {
        vec = new nodoHeap *[cap + 1]();
        posiciones = new int[cap + 1]();
        capacidad = cap;
        primeroLibre = 1;
    }

    // Destructor
    ~CPEGrafo()
    {
        destruir();
    }

    bool estaVacio()
    {
        return primeroLibre == 1;
    }

    void encolar(int id, int tiempo)
    {
        // Si ya existe, actualizar (checkear si ya lo encole y si ya esta lo actualiza)
        if (existe(id))
        {
            actualizar(id, tiempo);
        }

        else if (!estaLleno())
        {
            nodoHeap *nuevo = new nodoHeap(id, tiempo);
            vec[primeroLibre] = nuevo;
            posiciones[id] = primeroLibre;
            flotar(primeroLibre);
            primeroLibre++;
            cantElem++;
        }
    }

    int peek()
    {
        return vec[1]->idMision;
    }

    int desencolar()
    {
        int ret = peek();
        if (!estaVacio())
        {
            posiciones[ret] = 0;
            intercambiar(1, primeroLibre - 1);
            delete vec[primeroLibre - 1];
            vec[primeroLibre - 1] = NULL;
            primeroLibre--;
            hundir(1);
            flotar(1);
        }
        return ret;
    }
};
