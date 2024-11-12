#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <climits>
#include <math.h>

using namespace std;

int max(int a, int b)
{
    return a > b ? a : b;
}

// Función para calcular la distancia entre dos ciudades
int distanciaEfectiva(int ciudad1, int ciudad2, int *x, int *y, int *poblaciones)
{
    int restaX = x[ciudad2] - x[ciudad1];
    int restaY = y[ciudad2] - y[ciudad1];
    int absRestaP = abs(poblaciones[ciudad2] - poblaciones[ciudad1]);
    int poblacionMax = max(poblaciones[ciudad2], poblaciones[ciudad1]);

    int distancia = sqrt(restaX * restaX + restaY * restaY) + (absRestaP / poblacionMax);

    return distancia;
}

int main()
{
    int n;
    cin >> n;

    // Inicializamos vectores de datos de cada ciudad
    int *x = new int[n + 1]();
    int *y = new int[n + 1]();
    int *poblaciones = new int[n + 1]();

    for (int i = 1; i <= n; i++)
    {
        cin >> x[i] >> y[i] >> poblaciones[i];
    }

    // Encontrar la minima distancia efectiva entre todas las ciudades usando dividir y conquistar
    int minDistancia = INT_MAX;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            minDistancia = min(minDistancia, distanciaEfectiva(i, j, x, y, poblaciones));
        }
    }

    //! le hablo a mi chatty a ver tambien je pelotudo >:( :D oki

    delete[] x;
    delete[] y;
    delete[] poblaciones;

    return 0;
}