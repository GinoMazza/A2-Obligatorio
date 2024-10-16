#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>

using namespace std;

struct Arista{
    int destino;
    int peso;
    Arista* sig;
};

class GrafoLista {
    private:
        Arista** vertices;
        int V;
        bool dirigido;
        bool ponderado;

    public:
        GrafoLista(int cantV, bool esDirigido, bool esPonderado) {
            vertices = new Arista*[cantV + 1]();
            V = cantV;
            dirigido = esDirigido;
            ponderado = esPonderado;
        }

        ~GrafoLista() {
            delete[] vertices;
        }

        int getV(){
            return V;
        }

        Arista* adyacentes(int vertice) {
            return vertices[vertice];
        }

        void agregarArista(int origen, int destino, int peso) {
            Arista* nuevaArista = new Arista();
            nuevaArista->destino = destino;
            nuevaArista->peso = peso;
            nuevaArista->sig = vertices[origen];
            vertices[origen] = nuevaArista;
            if (!dirigido) {
                Arista * inversa = new Arista();
                inversa->destino = origen;
                inversa->peso = peso;
                inversa->sig = vertices[destino];
                vertices[destino] = inversa;
            }
        }
};