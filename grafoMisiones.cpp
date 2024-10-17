#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>
#include "cola.cpp"

using namespace std;

struct Arista{
    int idMision;
    int ciudadOrigen;
    int sigMision;
    int peso;
    Arista* sig;
};

class GrafoM{
    private:
        Arista** vertices;
        int V;
        bool dirigido;
        bool ponderado;

        void calcularGrados(int* grados){
            for(int i = 1; i <= V; i++){
                Arista* ady = adyacentes(i);
                while(ady){
                    grados[ady->sigMision]++;
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
        GrafoM(int cantV, bool esDirigido, bool esPonderado) {
            vertices = new Arista*[cantV + 1]();
            V = cantV;
            dirigido = esDirigido;
            ponderado = esPonderado;
        }

        ~GrafoM() {
            delete[] vertices;
        }

        int getV(){
            return V;
        }

        int getCiudadOrigen(int vertice){
            return vertices[vertice]->ciudadOrigen;
        }

        Arista* adyacentes(int vertice) {
            return vertices[vertice];
        }

        int getCiudadDestino(int vertice){
            Arista* ady = adyacentes(vertice);
            while (ady->sigMision != 0) ady = ady->sig;
            return ady->ciudadOrigen;
        }

        void agregarArista(int id, int ciudad, int siguiente, int peso) {
            Arista* nuevaArista = new Arista();
            nuevaArista->idMision = id;
            nuevaArista->ciudadOrigen = ciudad;
            nuevaArista->sigMision = siguiente;
            nuevaArista->peso = peso;
            nuevaArista->sig = vertices[id];
            vertices[id] = nuevaArista;
            if (!dirigido) {
                Arista * inversa = new Arista();
                inversa->idMision = siguiente;
                inversa->sigMision = id;
                inversa->peso = peso;
                inversa->sig = vertices[siguiente];
                vertices[siguiente] = inversa;
            }
        }

        int* OT(){
            int* ret = new int[V]();
            int pos = 0;
            int* grados = new int[V+1]();
            calcularGrados(grados);
            Cola* s = new Cola(); //cpe!!!! encolamos solo los de grado entrada 0 y asociamos costo a la ciudad de destino desde la que estamos ahora
            // actualizar costos en la cpe cada vez que cambiamos de ciudad de origen en cada paso
            // cpe con heap y vector (no es necesario tabla de hash)
            encolarGradoCero(s, grados);
            // dijksttra con actual como origen para saber cual mision hago primero por el costo
            while(!s->esVacia()){
                int actual = s->desencolar();
                ret[pos] = actual;
                pos++;
                Arista* ady = adyacentes(actual);
                while(ady){
                    int dest = ady->sigMision;
                    grados[dest]--;
                    if(grados[dest] == 0){
                        s->encolar(dest);
                    }
                    ady = ady->sig;
                }  
            }
            return ret;
        }
};