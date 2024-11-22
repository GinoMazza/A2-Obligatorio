#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "grafoMisiones.cpp"

using namespace std;

void imprimirCamino(int *vengoDe, int idDestino, string *nomC, string *nomM)
{
    if (vengoDe[idDestino] != 0)
    {
        imprimirCamino(vengoDe, vengoDe[idDestino], nomC, nomM);
    }
    cout << nomC[idDestino] << " -> ";
}

int main()
{
    int cantM, cantC, ciudadInicio, cantE;

    // Cargamos grafo misiones y vector misiones
    cin >> cantM;
    string *nombresM = new string[cantM + 1];
    GrafoM *misiones = new GrafoM(cantM, true, false);
    for (int i = 0; i < cantM; i++)
    {
        int idMision, idCiudad, idSigMision;
        string nomMision;
        cin >> idMision >> nomMision >> idCiudad;
        nombresM[idMision] = nomMision;
        bool sigo = true;
        while (sigo)
        {
            cin >> idSigMision;
            if (idSigMision == 0)
            {
                sigo = false;
            }
            misiones->agregarArista(idMision, idCiudad, idSigMision, 1);
        }
    }

    // Cargamos grafo ciudades y vector con nombres
    cin >> cantC;
    cin >> ciudadInicio;
    string *nombresC = new string[cantC + 1];
    GrafoC *ciudades = new GrafoC(cantC, false, true);
    for (int i = 0; i < cantC; i++)
    {
        int idCiudad;
        string nomCiudad;
        cin >> idCiudad >> nomCiudad;
        nombresC[idCiudad] = nomCiudad;
    }

    cin >> cantE;
    for (int i = 0; i < cantE; i++)
    {
        int idCiudadOrigen, idCiudadDestino, tiempoDesp;
        cin >> idCiudadOrigen >> idCiudadDestino >> tiempoDesp;
        ciudades->agregarArista(idCiudadOrigen, idCiudadDestino, tiempoDesp);
    }

    // Hacemos el OT del grafo misiones
    int *ordenMisiones = misiones->OT(ciudadInicio, ciudades);

    // Acumulador para tiempo total
    int tiempoTotal = 0;
    int origen = ciudadInicio;
    cout << "Ciudad inicial: " << nombresC[origen] << endl;

    cout << cantM << endl;
    // Recorremos las misiones en el orden obtenido
    for (int i = 0; i < cantM; i++)
    {
        cout << "for en posicion"<< " " << i << endl;
        cout << "entro a dijkstra" << endl;
        int *tiempos = ciudades->dijkstra(origen);
        cout << "agarro vengode" << endl;
        int *vengoDe = ciudades->getVengoDe();
        cout << "orden misones" << endl;
        int idMision = ordenMisiones[i];
        cout << "ciudad destino" << endl;
        cout << "entro a getorigen" << endl;
        int idCiudadDestino = misiones->getCiudadOrigen(idMision);
        cout << "salgo de getorigen, voy a tiempos" << endl;
        tiempoTotal += tiempos[idCiudadDestino];
        cout << "imprimo camino" << endl;
        imprimirCamino(vengoDe, idCiudadDestino, nombresC, nombresM);
        cout << "salgo de imprimir camino" << endl;
        cout << "Mision: " << nombresM[idMision] << " - " << nombresC[idCiudadDestino] << " - " << "Tiempo de viaje: " << tiempos[idCiudadDestino] << endl;
        origen = idCiudadDestino;
    }
    cout << "Misiones ejecutadas con exito." << endl;
    cout << "Tiempo total de viaje: " << tiempoTotal << endl;

    return 0;
}
