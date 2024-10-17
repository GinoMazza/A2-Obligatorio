#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>
#include "cola.cpp"

using namespace std;

struct Arista{
    int destino;
    int peso;
    Arista* sig;
};

class Grafo{
    private:
        Arista** vertices;
        int V;
        bool dirigido;
        bool ponderado;

    void calcularGrados(int* grados){
        for(int i = 1; i <= V; i++){
            Arista* ady = adyacentes(i);
            while(ady){
                grados[ady->destino]++;
                ady = ady->sig;
            }
        }
    }

    void encolarGradoCero(Cola* s, int* grados){
        for(int i = 1; i <= V; i++){
            if(grados[i] == 0) s->encolar(i);
        }
    }

    public:
        Grafo(int cantV, bool esDirigido, bool esPonderado) {
            vertices = new Arista*[cantV + 1]();
            V = cantV;
            dirigido = esDirigido;
            ponderado = esPonderado;
        }

        ~Grafo() {
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

        int* OT(){
            int* ret = new int[V]();
            int pos = 0;
            int* grados = new int[V+1]();
            calcularGrados(grados);
            Cola* s = new Cola();
            encolarGradoCero(s, grados);
            while(!s->esVacia()){
                int actual = s->desencolar();
                ret[pos] = actual;
                Arista* ady = adyacentes(actual);
                while(ady){
                    int dest = ady->destino;
                    grados[dest]--;
                    if(grados[dest] == 0){
                        s->encolar(dest);
                    }
                    ady = ady->sig;
                }
            }
            return ret;
        }

    void dijkstra(int origen) { //TERMINAR DIJKSTRA
        ColaPrioridad cp();
        cp.encolar(origen, 0); 
        while(!cp.estaVacia()) {
            int v = cp.desencolar(); 
            visitados[v] = true;
            para cada w adyacente a v
            if(!visitados[w] && costos[w] > costos[v] + dist(v,w)) {
                costos[w] = costos[v] + dist(v,w);
                vengo[w] = v;
                cp.encolar(w, costos[w]);
            }
        }
    }

};