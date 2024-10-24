#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "grafoMisiones.cpp"

using namespace std;

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
                                        sigo = false;
                              else misiones->agregarArista(idMision, idCiudad, idSigMision, 1);
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

          cout << "Orden de misiones:" << endl;
          for(int i = 0; i < cantM; i++){
                    cout << nombresM[ordenMisiones[i]] << endl;
          }
}