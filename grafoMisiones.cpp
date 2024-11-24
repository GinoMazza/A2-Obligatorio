#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>
#include "cpeGrafo.cpp"
#include "grafoCiudades.cpp"

using namespace std;

// Struct para aristas
struct AristaMis
{
    int ciudadOrigen;
    int sigMision;
    int tiempo;
    AristaMis *sig;
    AristaMis() {}
};

class GrafoM
{
private:
    AristaMis **vertices;
    int V;
    bool dirigido;
    bool ponderado;

    // Calculamos grados de entrada de todos los nodos
    void calcularGrados(int *grados)
    {
        for (int i = 1; i <= V; i++)
        {
            AristaMis *ady = adyacentes(i);
            while (ady)
            {
                grados[ady->sigMision]++;
                ady = ady->sig;
            }
        }
    }

    // Encolamos los nodos con grado de entrada cero
    void encolarGradoCero(CPEGrafo *s, int *grados, int *tiempoInicial)
    {
        for (int i = 1; i <= V; i++)
        {
            if (grados[i] == 0)
                s->encolar(i, tiempoInicial[vertices[i]->ciudadOrigen]);
        }
    }

public:
    // Constructor
    GrafoM(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new AristaMis *[cantV + 1]();
        V = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
    }

    // Destructor
    ~GrafoM()
    {
        for (int i = 1; i <= V; i++)
        {
            AristaMis *a = vertices[i];
            while (a)
            {
                AristaMis *aBorrar = a;
                a = a->sig;
                delete aBorrar;
            }
        }
        delete[] vertices;
    }

    // Funcion para obtener la cantidad de vertices
    int getV()
    {
        return V;
    }

    // Funcion para obtener la ciudad de origen de la mision (vertice recibido)
    int getCiudadOrigen(int vertice)
    {
        return vertices[vertice]->ciudadOrigen;
    }

    // Funcion para obtener la lista de adyacentes al vertice recibido
    AristaMis *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    // Funcion para obtener la ciudad de destino de la mision (vertice recibido)
    int getCiudadDestino(int vertice)
    {
        AristaMis *ady = adyacentes(vertice);
        while (ady->sigMision != 0)
            ady = ady->sig;
        return ady->ciudadOrigen;
    }

    // Funcion para agregar una arista al grafo
    void agregarArista(int id, int ciudad, int siguiente, int peso)
    {
        AristaMis *nuevaArista = new AristaMis();
        nuevaArista->ciudadOrigen = ciudad;
        nuevaArista->sigMision = siguiente;
        nuevaArista->tiempo = peso;
        nuevaArista->sig = vertices[id];
        vertices[id] = nuevaArista;

        // Si el grafo no es dirigido agregamos la arista inversa
        if (!dirigido)
        {
            AristaMis *inversa = new AristaMis();
            inversa->ciudadOrigen = siguiente;
            inversa->sigMision = id;
            inversa->tiempo = peso;
            inversa->sig = vertices[siguiente];
            vertices[siguiente] = inversa;
        }
    }

    // Orden topológico
    int *OT(int ciudadInicio, GrafoC *ciudades)
    {
        // Inicializamos vector de orden topológico
        int *ot = new int[V]();

        // Inicializamos vector grados
        int *grados = new int[V + 1]();

        // Cargamos grados de entrada de cada vector
        calcularGrados(grados);

        // Inicializamos la cola de prioridad extendida
        CPEGrafo *s = new CPEGrafo(V + 1);

        // Dijkstra desde el origen recibido a todos los demás nodos
        int *tiempoInicial = ciudades->dijkstra(ciudadInicio);

        // Encolamos en la CPE los vertices con grado de entrada 0
        encolarGradoCero(s, grados, tiempoInicial);

        int pos = 0;
        while (!s->estaVacio())
        {
            // Desencolamos el vertice actual (sabemos que tiene grado de entrada 0)
            int actual = s->desencolar();

            // Lo cargamos en el OT
            ot[pos] = actual;

            pos++;

            // Obtenemos los tiempos aplicando dijkstra desde el vertice actual
            int *tiemposCiudades = ciudades->dijkstra(vertices[actual]->ciudadOrigen);

            // Obtenemos y recorremos los adyacentes del vertice actual
            AristaMis *ady = adyacentes(actual);
            while (ady)
            {
                if (ady->sigMision != 0)
                {
                    // Obtenemos destino
                    int dest = ady->sigMision;

                    // Disminuimos grado de entrada del destino
                    grados[dest]--;

                    // Si tiene grado de entrada 0, encolamos
                    if (grados[dest] == 0)
                    {
                        s->encolar(dest, tiemposCiudades[vertices[dest]->ciudadOrigen]);
                    }
                }
                ady = ady->sig;
            }
            delete[] tiemposCiudades;
        }
        return ot;
    }
};