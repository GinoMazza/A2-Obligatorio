#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>
#include <climits>
#include "cp.cpp"

using namespace std;

struct Arista
{
    int origen;
    int destino;
    int tiempo;
    Arista *sig;
    Arista() {}
};

class GrafoC
{
private:
    Arista **vertices;
    int V;
    bool dirigido;
    bool ponderado;
    int *vengoDe;

    int *initTiempos(int origen)
    {
        int *tiempos = new int[V + 1];
        for (int i = 1; i <= V; i++)
        {
            if (i == origen)
                tiempos[i] = 0;
            else
                tiempos[i] = INT_MAX;
        }
        return tiempos;
    }

    int *initVengoDe(int origen)
    {
        int *nuevo = new int[V + 1];
        for (int i = 1; i <= V; i++)
        {
            if (i == origen)
                nuevo[i] = 0;
            else
                nuevo[i] = -1;
        }
        return nuevo;
    }

public:
    GrafoC(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        V = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
        vengoDe = new int[cantV + 1]();
    }

    ~GrafoC()
    {
        for (int i = 1; i <= V; i++)
        {
            Arista *a = vertices[i];
            while (a)
            {
                Arista *aBorrar = a;
                a = a->sig;
                delete aBorrar;
            }
        }
        delete[] vertices;
        delete[] vengoDe;
    }

    int getV()
    {
        return V;
    }

    int *getVengoDe()
    {
        return vengoDe;
    }

    Arista *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    void agregarArista(int o, int dest, int tiempoViaje)
    {
        Arista *nuevaArista = new Arista();
        nuevaArista->origen = o;
        nuevaArista->destino = dest;
        nuevaArista->tiempo = tiempoViaje;
        nuevaArista->sig = vertices[o];
        vertices[o] = nuevaArista;
        if (!dirigido)
        {
            Arista *inversa = new Arista();
            inversa->origen = dest;
            inversa->destino = o;
            inversa->tiempo = tiempoViaje;
            inversa->sig = vertices[dest];
            vertices[dest] = inversa;
        }
    }

    int *dijkstra(int ciudadOrigen)
    {
        bool *visitados = new bool[V + 1]();
        int *tiempos = initTiempos(ciudadOrigen);
        vengoDe = initVengoDe(ciudadOrigen);
        CP *cp = new CP(V * V);
        cp->encolar(ciudadOrigen, 0);
        visitados[ciudadOrigen] = true;
        while (!cp->estaVacia())
        {
            int actual = cp->desencolar();
            visitados[actual] = true;
            Arista *ady = adyacentes(actual);
            while (ady)
            {
                int dest = ady->destino;
                int tiempoViaje = ady->tiempo;
                if (!visitados[dest] && tiempos[dest] > tiempos[actual] + tiempoViaje)
                {
                    tiempos[dest] = tiempos[actual] + tiempoViaje;
                    vengoDe[dest] = actual;
                    cp->encolar(dest, tiempos[dest]);
                }
                ady = ady->sig;
            }
        }
        delete[] visitados;
        return tiempos;
    }
};