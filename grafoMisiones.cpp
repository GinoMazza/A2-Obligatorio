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
        cout << "entro a OT" << endl;
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
            cout << "mando djkstra" << endl;
            int *tiemposCiudades = ciudades->dijkstra(vertices[actual]->ciudadOrigen);
            AristaMis *ady = adyacentes(actual);
            cout << "Sale de adyacentes de: " << actual << endl;
            while (ady)
            {
                if(ady->sigMision != 0){
                    cout << "En el while" << endl;
                    int dest = ady->sigMision;
                    cout << "Grado de: " << dest << " " << grados[dest] << endl;
                    grados[dest]--;
                    cout << "Grado disminuido de: " << dest << " " << grados[dest] << endl;
                    if (grados[dest] == 0)
                    {
                        cout << "Encolando: " << dest << endl;
                        cout << vertices[dest]->ciudadOrigen << endl;
                        cout << "Con tiempo: " << tiemposCiudades[vertices[dest]->ciudadOrigen] << endl;
                        s->encolar(dest, tiemposCiudades[vertices[dest]->ciudadOrigen]);
                        cout << "Encolado: " << dest << endl;
                    }
                }
                ady = ady->sig;
            }
            delete[] tiemposCiudades;
            cout << "Fin de adyacentes" << endl;
        }
        cout << "Termina OT" << endl;
        return ot;
    }
};