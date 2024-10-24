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
        int *vengoDe = new int[V + 1];
        for (int i = 1; i <= V; i++)
        {
            if (i == origen)
                vengoDe[i] = origen;
            else
                vengoDe[i] = -1;
        }
        return vengoDe;
    }

public:
    GrafoC(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        V = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
    }

    ~GrafoC()
    {
        delete[] vertices;
    }

    int getV()
    {
        return V;
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
        int *vengoDe = initVengoDe(ciudadOrigen);
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
                    cp->encolar(dest, tiempoViaje);
                }
                ady = ady->sig;
            }
        }
        return tiempos;
    }

    int imprimir(int idMision, int idCiudadOrigen, int idCiudadDestino, int *costos, int *vengoDe, string *nombresM, string *nombresC)
    {
        // recursivo en vengoDe
        // cout << nombresC[vengoDe[x]] << "-> ";
        cout << "Mision: " << nombresM[idMision] << " - ";
        cout << nombresC[idCiudadOrigen] << " - ";
        cout << "Tiempo de viaje: " << costos[idCiudadDestino];
    }
};