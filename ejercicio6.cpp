#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "grafoCiudades.cpp"

using namespace std;

int *actualizarCostos(GrafoC *ciudades, int &costoTotal, int *&costos, int origen, int destino)
{
    costos = ciudades->dijkstra(origen);
    costoTotal += costos[destino];
    ciudades->duplicarCamino(origen, destino);
    return ciudades->getVengoDe();
}

void imprimirCamino(string *nombreCiudades, int *vengoDe, int destino)
{
    if (vengoDe[destino] != 0)
    {
        imprimirCamino(nombreCiudades, vengoDe, vengoDe[destino]);
    }
    cout << nombreCiudades[destino] << " -> ";
}

int main()
{
    int cantCiudades, idStart, idEntity, idTeam, idPoint, cantAristas;
    cin >> cantCiudades;
    string *nombreCiudades = new string[cantCiudades + 1];
    for (int i = 1; i <= cantCiudades; i++)
    {
        int id;
        string nombre;
        cin >> id >> nombre;
        nombreCiudades[id] = nombre;
    }
    cin >> idStart >> idEntity >> idTeam >> idPoint >> cantAristas;
    GrafoC *ciudadesT = new GrafoC(cantCiudades, false, true);
    GrafoC *ciudadesE = new GrafoC(cantCiudades, false, true);
    for (int i = 1; i <= cantAristas; i++)
    {
        int idOrigen, idDestino, costo;
        cin >> idOrigen >> idDestino >> costo;
        ciudadesT->agregarArista(idOrigen, idDestino, costo);
        ciudadesE->agregarArista(idOrigen, idDestino, costo);
    }

    // Inicializamos variables para calular mejor ruta
    int costoTotalT = 0;
    int costoTotalE = 0;
    int *costos;

    // Actualizamos costos de ruta empezando por Reunirse con el Equipo y obtenemos vector vengoDe
    int *vengoDeST = actualizarCostos(ciudadesT, costoTotalT, costos, idStart, idTeam);
    int *vengoDeTE = actualizarCostos(ciudadesT, costoTotalT, costos, idTeam, idEntity);
    int *vengoDeEP = actualizarCostos(ciudadesT, costoTotalT, costos, idEntity, idPoint);

    // Actualizamos costos de ruta empezando por Desactivar la Entidad y obtenemos vector vengoDe
    int *vengoDeSE = actualizarCostos(ciudadesE, costoTotalE, costos, idStart, idEntity);
    int *vengoDeET = actualizarCostos(ciudadesE, costoTotalE, costos, idEntity, idTeam);
    int *vengoDeTP = actualizarCostos(ciudadesE, costoTotalE, costos, idTeam, idPoint);

    // Imprimimos la ruta con menor costo
    if (costoTotalT < costoTotalE)
    {
        cout << "BE11, la mejor ruta es buscar equipo, Desactivar la Entidad y punto de extraccion con un costo de " << costoTotalT << endl;
        cout << "La otra opcion tiene un costo de " << costoTotalE << endl;
        cout << "Paso 1: ";
        imprimirCamino(nombreCiudades, vengoDeST, idTeam);
        cout << "Buscar equipo " << endl;
        cout << "Paso 2: ";
        imprimirCamino(nombreCiudades, vengoDeTE, idEntity);
        cout << "Desactivar la Entidad" << endl;
        cout << "Paso 3: ";
        imprimirCamino(nombreCiudades, vengoDeEP, idPoint);
        cout << "Ir a Punto de extraccion" << endl;
    }
    else
    {
        cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << costoTotalE << endl;
        cout << "La otra opcion tiene un costo de " << costoTotalT << endl;
        cout << "Paso 1: ";
        imprimirCamino(nombreCiudades, vengoDeSE, idEntity);
        cout << "Desactivar la Entidad" << endl;
        cout << "Paso 2: ";
        imprimirCamino(nombreCiudades, vengoDeET, idTeam);
        cout << "Buscar equipo" << endl;
        cout << "Paso 3: ";
        imprimirCamino(nombreCiudades, vengoDeTP, idPoint);
        cout << "Ir a Punto de extraccion" << endl;
    }
    delete[] nombreCiudades;
    delete[] costos;
    delete[] vengoDeTE;
    delete[] vengoDeEP;
    delete[] vengoDeST;
    delete[] vengoDeSE;
    delete[] vengoDeET; 
    delete[] vengoDeTP;
}