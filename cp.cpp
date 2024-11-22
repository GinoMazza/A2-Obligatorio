#include <iostream>
#include <cassert>

using namespace std;

class CP
{
private:
    struct nodoHeap
    {
        int id;
        int precio;
        nodoHeap(int i, int p) : id(i), precio(p) {}
    };

    nodoHeap **vec;
    bool *impresos;
    int capacidad;
    int primeroLibre;

    bool estaLleno()
    {
        return primeroLibre > capacidad;
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

    bool comparar(int posPadre, int pos)
    {
        bool intercambia = false;
        if (vec[posPadre]->precio > vec[pos]->precio)
        {
            intercambia = true;
        }
        else if (vec[posPadre]->precio == vec[pos]->precio && vec[posPadre]->id < vec[pos]->id)
        {
            intercambia = true;
        }
        return intercambia;
    }

    void intercambiar(int posPadre, int pos)
    {
        nodoHeap *aux = vec[posPadre];
        vec[posPadre] = vec[pos];
        vec[pos] = aux;
    }

    void flotar(int pos)
    {
        if (pos == 1)
            return; // CB
        int posPadre = padre(pos);
        if (comparar(posPadre, pos))
        {
            intercambiar(posPadre, pos);
            flotar(posPadre);
        }
    }

    void hundir(int pos)
    {
        int izq = hijoIzq(pos);
        int der = hijoDer(pos);
        if (izq >= primeroLibre && der >= primeroLibre)
            return; // CB

        int hijoMenorPrecio = izq;
        if (der < primeroLibre)
        {
            if (comparar(izq, der))
            {
                hijoMenorPrecio = der;
            }
        }
        if (comparar(pos, hijoMenorPrecio))
        {
            intercambiar(pos, hijoMenorPrecio);
            hundir(hijoMenorPrecio);
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
        vec = NULL;
    }

public:
    CP(int cap)
    {
        vec = new nodoHeap *[cap + 1]();
        impresos = new bool[cap + 1]();
        capacidad = cap;
        primeroLibre = 1;
    }

    ~CP()
    {
        destruir();
        delete[] impresos;
    }

    void encolar(int id, int precio)
    {
        if (!estaLleno())
        {
            nodoHeap *nuevo = new nodoHeap(id, precio);
            vec[primeroLibre] = nuevo;
            flotar(primeroLibre);
            primeroLibre++;
        }
    }

    int desencolar()
    {
        int ret = vec[1]->id;
        if (!estaVacia())
        {
            intercambiar(1, primeroLibre - 1);
            delete vec[primeroLibre - 1];
            vec[primeroLibre - 1] = NULL;
            primeroLibre--;
            hundir(1);
        }
        return ret;
    }

    bool estaVacia()
    {
        return primeroLibre == 1;
    }
};
