#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "grafo.cpp"

using namespace std;

int main(){
    int cantM, cantC, cantE;
    cin >> cantM; 
    Grafo* misiones = new Grafo(cantM, true, false);
    for(int i = 0; i < cantM; i++){
        int idMision, idCiudad, idSigMision;
        string nomMision;
        cin >> idMision >> nomMision >> idCiudad;
        bool sigo = true;
        while(sigo){
            cin >> idSigMision;
            if(idSigMision == 0) sigo = false;
            misiones->agregarArista(idMision, idSigMision, 1);
        }
    }
    cin >> cantC;
    string* arrCiudades = new string[cantC+1];
    Grafo* ciudades = new Grafo(cantC, false, true);
    for(int i = 0; i < cantC; i++){
        int idCiudad;
        string nomCiudad;
        cin >> idCiudad >> nomCiudad;
        arrCiudades[idCiudad] = nomCiudad;
    }
    cin >> cantE;
    for(int i = 0; i < cantE; i++){
        int idCiudadOrigen, idCiudadDestino, tiempoDesp;
        cin >> idCiudadOrigen >> idCiudadDestino >> tiempoDesp;
        ciudades->agregarArista(idCiudadOrigen, idCiudadDestino, tiempoDesp);
    }

    // A PARTIR DE ACA VER Q CARAJO HACEMOS
    int* ordenMisiones = misiones->OT();
    bool* visitados = new bool[cantC+1]();
    int* costos = initCostos(ordenMisiones[0], cantC);
    int* vengoDe = new int[cantC+1]();
    ciudades->dijkstra(ordenMisiones, visitados, costos, vengoDe);
}

int* initCostos(int origen, int cantC){
    int* costos = new int[cantC+1];
    for(int i = 1; i <= cantC; i++){
        if(i == origen) costos[i] = 0;
        else costos[i] = INT_MAX;
    }
}
