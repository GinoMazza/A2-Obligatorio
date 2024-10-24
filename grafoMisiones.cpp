#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>
#include "cpeGrafo.cpp"
#include "grafoCiudades.cpp"

using namespace std;

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
    GrafoM(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new AristaMis *[cantV + 1]();
        V = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
    }

    ~GrafoM()
    {
        delete[] vertices;
    }

    int getV()
    {
        return V;
    }

    int getCiudadOrigen(int vertice)
    {
        return vertices[vertice]->ciudadOrigen;
    }

    AristaMis *adyacentes(int vertice)
    {
        cout << "Entra a adyacentes con: " << vertice << endl;
        cout << vertices[vertice] << endl;
        return vertices[vertice];
    }

    int getCiudadDestino(int vertice)
    {
        AristaMis *ady = adyacentes(vertice);
        while (ady->sigMision != 0)
            ady = ady->sig;
        return ady->ciudadOrigen;
    }

    void agregarArista(int id, int ciudad, int siguiente, int peso)
    {
        AristaMis *nuevaArista = new AristaMis();
        nuevaArista->ciudadOrigen = ciudad;
        nuevaArista->sigMision = siguiente;
        nuevaArista->tiempo = peso;
        nuevaArista->sig = vertices[id];
        vertices[id] = nuevaArista;
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
        int *ot = new int[V]();
        int pos = 0;
        int *grados = new int[V + 1]();
        calcularGrados(grados);
        CPEGrafo *s = new CPEGrafo(V + 1);
        int *tiempoInicial = ciudades->dijkstra(ciudadInicio); // Dijkstra desde el origen recibido a todos los demás nodos
        encolarGradoCero(s, grados, tiempoInicial);
        while (!s->estaVacio())
        {
            int actual = s->desencolar();
            cout << "Actual: " << actual << endl;
            ot[pos] = actual;
            pos++;
            int *tiemposCiudades = ciudades->dijkstra(vertices[actual]->ciudadOrigen);
            AristaMis *ady = adyacentes(actual);
            cout << "Sale de adyacentes de: " << actual << endl;
            while (ady)
            {
                cout << "En el while" << endl;
                int dest = ady->sigMision;
                cout << "Grado de: " << dest << " " << grados[dest] << endl;
                grados[dest]--;
                cout << "Grado disminuido de: " << dest << " " << grados[dest] << endl;
                if (grados[dest] == 0)
                {
                    cout << "Encolando: " << dest << endl;
                    cout << "Con tiempo: " << tiemposCiudades[vertices[dest]->ciudadOrigen] << endl;
                    s->encolar(dest, tiemposCiudades[vertices[dest]->ciudadOrigen]);
                    cout << "Encolado: " << dest << endl;
                }
                ady = ady->sig;
            }
            cout << "Fin de adyacentes" << endl;
        }
        cout << "Termina OT" << endl;
        return ot;
    }

    // ENCOLAR ID DE MISION Y EL COSTO DE LLEGAR A LA CIUDAD DE ESA MISION DESDE LA QUE ESTAMOS

    //* cpe!!!! encolamos solo los de grado entrada 0 y asociamos costo a la ciudad de destino desde la que estamos ahora
    //* actualizar costos en la cpe cada vez que cambiamos de ciudad de origen en cada paso
    // dijkstra con actual como origen para saber cual mision hago primero por el costo
};