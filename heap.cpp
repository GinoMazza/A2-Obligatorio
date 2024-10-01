#include <iostream>
#include <cassert>

using namespace std;

class MinHeap{
    private:

    struct nodoHeap{
        int id;
        int precio;
        nodoHeap(int i, int p) : id(i), precio(p) {}
    };

    nodoHeap** vec;
    bool* aux;
    int capacidad;
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
        return pos + 1;
    }

    int hijoDer(int pos){
        return pos + 2;
    }

    int posMenorPrecio(int a, int b){
        return a < b ? a : b;
    }

    bool comparar(int posPadre, int pos){
        bool intercambia = false;
        if(vec[posPadre]->precio > vec[pos]->precio){
            intercambia = true;
        }
        else if(vec[posPadre]->precio == vec[pos]->precio && vec[posPadre]->id < vec[pos]->id){
            intercambia = true;
        }
       return intercambia; 
    }

    void intercambiar(int posPadre, int pos){
        nodoHeap* aux = vec[posPadre];
        vec[posPadre] = vec[pos];
        vec[pos] = aux;
    }

    void flotar(int pos){
        if (pos == 1) return; // CB
        int posPadre = padre(pos);
        if (comparar(posPadre, pos)){
            intercambiar(posPadre, pos);
            flotar(posPadre);
        }
    }

    void hundir(int pos){
        int izq = hijoIzq(pos);
        int der = hijoDer(pos);
        if (!vec[izq] && !vec[der]) return; // CB
        int posMenorHijo = posMenorPrecio(izq, der);
        if (comparar(pos, posMenorHijo)){
            intercambiar(pos, posMenorHijo);
            hundir(posMenorHijo);
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
    MinHeap(int cap){
        vec = new nodoHeap*[cap + 1]();
        aux = new bool[cap + 1]();
        capacidad = cap;
        primeroLibre = 1;
    }

    ~MinHeap(){
        destruir();
        delete[] aux;
        capacidad = 0;
        primeroLibre = 1;
    }

    void insertar(int id, int precio){
        if(!estaLleno()) {
            nodoHeap* nuevo = new nodoHeap(id, precio);
            vec[primeroLibre] = nuevo;
            flotar(primeroLibre);
            primeroLibre++;
        }
    }

    int peek(){
        return vec[1]->id;
    }

    void eliminar(){
        if(!estaVacio()){
            vec[1] = vec[primeroLibre - 1];
            delete vec[primeroLibre - 1];
            hundir(1);
            primeroLibre--;
        }
    }

    void imprimirK(int k){
        for(int i = 1; i < k; i++) {
            cout << vec[i]->id << endl;
        }
    }
};
