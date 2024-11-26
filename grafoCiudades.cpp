#include <climits>
#include "cp.cpp"

using namespace std;

// Struct para las aristas
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

    // Funcion para inicializar el vector tiempos
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

    // Funcion para inicializar el vector vengoDe
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
    // Constructor
    GrafoC(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        V = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
        vengoDe = new int[cantV + 1]();
    }

    // Destructor
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

    // Funcion para obtener la cantidad de vertices
    int getV()
    {
        return V;
    }

    // Funcion para obtener vengoDe
    int *getVengoDe()
    {
        return vengoDe;
    }

    // Funcion para obtener la lista de adyacentes al vertice recibido
    Arista *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    // Funcion para agregar una arista al grafo
    void agregarArista(int o, int dest, int tiempoViaje)
    {
        Arista *nuevaArista = new Arista();
        nuevaArista->origen = o;
        nuevaArista->destino = dest;
        nuevaArista->tiempo = tiempoViaje;
        nuevaArista->sig = vertices[o];
        vertices[o] = nuevaArista;

        // Si el grafo no es dirigido agregamos la arista inversa
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

    // Funcion para aplicar dijkstra desde la ciudad recibida (ciudad origen)
    int *dijkstra(int ciudadOrigen)
    {
        // Inicializamos vectores de visitados, tiempos y vengoDe
        bool *visitados = new bool[V + 1]();
        int *tiempos = initTiempos(ciudadOrigen);
        vengoDe = initVengoDe(ciudadOrigen);

        // Inicializamos cola de prioridad
        CP *cp = new CP(V * V);

        // Encolamos la ciudad de origen
        cp->encolar(ciudadOrigen, 0);

        // Marcamos la ciudad de origen como visitada
        visitados[ciudadOrigen] = true;

        // Mientras que la cola de prioridad no este vacia
        while (!cp->estaVacia())
        {
            // Desencolamos el vertice actual
            int actual = cp->desencolar();

            // Lo marcamos como visitado
            visitados[actual] = true;

            // Obtenemos sus adyacentes y los recorremos
            Arista *ady = adyacentes(actual);
            while (ady)
            {
                int dest = ady->destino;
                int tiempoViaje = ady->tiempo;

                // Si el tiempo es mejor, lo actualizamos, actualizamos vengoDe y lo encolamos
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

    // FUNCION PARA EL EJERCICIO 6

    // Funcion que duplica el costo de una arista
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

            duplicarCamino(origen, vengoDe[destino]);
        }
    }
};