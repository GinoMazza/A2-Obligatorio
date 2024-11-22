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
        cout << "ESTOY DENTRO DE DIJSKTRA" << endl;
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
        cout << "chau me voy de dijsktra" << endl;
        delete[] visitados;
        return tiempos;
    }

    //* Funciones para ejercicio6

    void duplicarCamino(int origen, int destino)
    {
        if (vengoDe[destino] != 0)
        {
            Arista *a1 = vertices[destino];
            bool sigo1 = true;
            while (a1 && sigo1)
            {
                if (a1->destino == vengoDe[destino])
                {
                    a1->tiempo = a1->tiempo * 2;
                    sigo1 = false;
                }
                a1 = a1->sig;
            }
            Arista *a2 = vertices[vengoDe[destino]];
            bool sigo2 = true;
            while (a2 && sigo2)
            {
                if (a2->destino == destino)
                {
                    a2->tiempo = a2->tiempo * 2;
                    sigo2 = false;
                }
                a2 = a2->sig;
            }
            delete[] a1;
            delete[] a2;
            duplicarCamino(origen, vengoDe[destino]);
        }
    }

    //* Fin funciones para ejercicio6
};