#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "grafoMisiones.cpp"
#include "grafoCiudades.cpp"

using namespace std;

int main(){
    int cantM, cantC, cantE;

    // Cargamos grafo misiones y vector misiones
    cin >> cantM;
    string* nombresM = new string[cantM+1]; 
    GrafoM* misiones = new GrafoM(cantM, true, false);
    for(int i = 0; i < cantM; i++){
        int idMision, idCiudad, idSigMision;
        string nomMision;
        cin >> idMision >> nomMision >> idCiudad;
        nombresM[idMision] = nomMision;
        bool sigo = true;
        while(sigo){
            cin >> idSigMision;
            if(idSigMision == 0) sigo = false;
            misiones->agregarArista(idMision, idCiudad, idSigMision, 1);
        }
    }
    
    // Cargamos grafo ciudades y vector con nombres
    cin >> cantC;
    string* nombresC = new string[cantC+1];
    GrafoC* ciudades = new GrafoC(cantC, false, true);
    for(int i = 0; i < cantC; i++){
        int idCiudad;
        string nomCiudad;
        cin >> idCiudad >> nomCiudad;
        nombresC[idCiudad] = nomCiudad;
    }

    cin >> cantE;
    for(int i = 0; i < cantE; i++){
        int idCiudadOrigen, idCiudadDestino, tiempoDesp;
        cin >> idCiudadOrigen >> idCiudadDestino >> tiempoDesp;
        ciudades->agregarArista(idCiudadOrigen, idCiudadDestino, tiempoDesp);
    }

    // Hacemos el OT del grafo misiones
    int* ordenMisiones = misiones->OT();

    // Inicializamos vectores para Dijkstra
    bool* visitados = new bool[cantC+1]();
    int* costos = initCostos(ordenMisiones[0], cantC);
    int* vengoDe = new int[cantC+1]();
    
    // Acumulador para tiempo total
    int tiempoTotal = 0;

    // Imprimimos salida
    for (int i = 0; i < cantM; i++) {
        int idMision = ordenMisiones[i];
        int idCiudadOrigen = misiones->getCiudadOrigen(idMision);
        int idCiudadDestino = misiones->getCiudadDestino(idMision);
        ciudades->dijkstra(idCiudadOrigen, visitados, costos, vengoDe);
        tiempoTotal += ciudades->imprimir(idMision, idCiudadOrigen, idCiudadDestino, costos, vengoDe, nombresM, nombresC);
    }

    cout << "Ciudad inicial: " << nombresC[ordenMisiones[0]] << endl;

    cout << "Misiones ejecutadas con exito." << endl;
    cout << "Tiempo total de viaje: " << tiempoTotal << endl;

}

int* initCostos(int origen, int cantC){
    int* costos = new int[cantC+1];
    for(int i = 1; i <= cantC; i++){
        if(i == origen) costos[i] = 0;
        else costos[i] = INT_MAX;
    }
    return costos;
}
