#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>
#include "hashCpe.cpp"

using namespace std;

class CPE{
    private:

    struct nodoHeap{
        int id;
        int prioridad;
        bool paraLlevar;
        string items;
        bool entregado;
        nodoHeap(int i, int p, bool pl, string it) : id(i), prioridad(p), paraLlevar(pl), items(it), entregado(false) {}
    };

    nodoHeap** vec;
    HashCPE* posiciones;
    int capacidad;
    int cantElem;
    int primeroLibre;

    bool estaLleno(){
        return primeroLibre > capacidad;
    }

    bool estaVacio(){
        return primeroLibre == 1;
    }

    int padre(int pos){
        return pos / 2;
    }

    int hijoIzq(int pos){
        return pos * 2;
    }

    int hijoDer(int pos){
        return (pos * 2) + 1;
    }

    bool comparar(int posPadre, int pos){
        bool intercambia = false;
        if(vec[posPadre]->prioridad > vec[pos]->prioridad) intercambia = true; //caso > prioridad
        else if(vec[posPadre]->prioridad == vec[pos]->prioridad){
            if(!vec[posPadre]->paraLlevar && vec[pos]->paraLlevar) intercambia = true; //caso paraLlevar
            else if(vec[posPadre]->paraLlevar == vec[pos]->paraLlevar){
                if(vec[posPadre]->id > vec[pos]->id) intercambia = true; //caso > id
            }
        }
        return intercambia; 
    }

    void intercambiar(int posPadre, int pos){
        posiciones->intercambiar(vec[posPadre]->id, vec[pos]->id);
        nodoHeap* aux = vec[posPadre];
        vec[posPadre] = vec[pos];
        vec[pos] = aux;
    }

    void flotar(int pos){
        if (pos == 1) {
            return; // CB
        }
        int posPadre = padre(pos);
        if (comparar(posPadre, pos)){
            intercambiar(posPadre, pos);
            flotar(posPadre);
        }
    }

    void hundir(int pos){
        int izq = hijoIzq(pos);
        int der = hijoDer(pos);
        if (izq >= primeroLibre && der >= primeroLibre) return; //CB
        
        int hijoMenorPrioridad = izq;
        if (der < primeroLibre) {
            if (comparar(izq, der)) { 
                hijoMenorPrioridad = der; 
            }
        }
        if (comparar(pos, hijoMenorPrioridad)){
            intercambiar(pos, hijoMenorPrioridad);
            hundir(hijoMenorPrioridad);
        }
    }

    void destruir(){
        for(int i = 0; i<capacidad; i++){
            delete vec[i];
            vec[i] = NULL;
        }
        delete[] vec;
        vec = NULL;
    }

public:
    CPE(int cap){
        vec = new nodoHeap*[cap + 1]();
        posiciones = new HashCPE(cap);
        capacidad = cap;
        primeroLibre = 1;
    }

    ~CPE(){
        destruir();
        capacidad = 0;
        primeroLibre = 1;
    }

    void insertar(int id, int prioridad, bool paraLlevar, string items){
        if(!estaLleno()) {
            nodoHeap* nuevo = new nodoHeap(id, prioridad, paraLlevar, items);
            vec[primeroLibre] = nuevo;
            posiciones->insertar(id, primeroLibre); 
            flotar(primeroLibre);
            primeroLibre++;
            cantElem++;
        }
    }

    int peek(){
        return vec[1]->id;
    }

    void eliminar(){
        if(!estaVacio()){
            intercambiar(1, primeroLibre - 1);
            delete vec[primeroLibre - 1];
            vec[primeroLibre - 1] = NULL;
            primeroLibre--;
            hundir(1);
            flotar(1);
        }
    }

    void entregar(int id){
        int pos = posiciones->getPos(id);
        vec[pos]->entregado = true;
    }

    void cambiar(int id){
        int pos = posiciones->getPos(id);
        vec[pos]->paraLlevar = true;
        flotar(pos);
        hundir(pos);
    }

    void imprimir(){
        for(int i = 1; i <= cantElem; i++) {
            nodoHeap* actual = vec[1];
            if(!actual->entregado){
                string paraLlevar;
                if(actual->paraLlevar == true) {
                    paraLlevar = "true";
                }
                else{
                    paraLlevar = "false";
                }
                cout << actual->id << " " << actual->prioridad << " " << paraLlevar << " " << actual->items << endl;
            }
            eliminar();
        }    
    }
};
