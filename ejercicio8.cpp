#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <climits>
#include <math.h>

using namespace std;

// Struct para ciudades
struct Ciudad
{
    double x;
    double y;
    double poblacion;
    Ciudad(double xPos, double yPos, double pob) : x(xPos), y(yPos), poblacion(pob) {}
    Ciudad() {}
};

// Struct para pares de ciudades
struct ParCiudades
{
    Ciudad *ciudad1;
    Ciudad *ciudad2;
    ParCiudades(Ciudad *c1, Ciudad *c2) : ciudad1(c1), ciudad2(c2) {}
    ParCiudades() {}
};

double max(double a, double b)
{
    return a > b ? a : b;
}

// Función para calcular la distancia entre dos ciudades
double distanciaEfectiva(Ciudad *ciudad1, Ciudad *ciudad2)
{
    double restaX = ciudad2->x - ciudad1->x;
    double restaY = ciudad2->y - ciudad1->y;
    double absRestaP = abs(ciudad2->poblacion - ciudad1->poblacion);
    double poblacionMax = max(ciudad2->poblacion, ciudad1->poblacion);
    double distancia = sqrt(restaX * restaX + restaY * restaY) + (absRestaP / poblacionMax);

    return distancia;
}

// Función para construir franja de ciudades
Ciudad **construirFranja(Ciudad **ciudades, int inicio, int fin, int m, double menorDistancia, int &tam)
{
    Ciudad **franja = new Ciudad *[fin - inicio + 1];
    tam = 0;
    for (int i = inicio; i <= fin; i++)
    {
        if (abs(ciudades[i]->x - ciudades[m]->x) < menorDistancia)
        {
            franja[tam] = ciudades[i];
            tam++;
        }
    }
    return franja;
}

// Devuelve el par de ciudad con menor distancia efectiva
ParCiudades *compararPares(ParCiudades *par1, ParCiudades *par2)
{
    double distanciaPar1 = distanciaEfectiva(par1->ciudad1, par1->ciudad2);
    double distanciaPar2 = distanciaEfectiva(par2->ciudad1, par2->ciudad2);


    if (distanciaPar1 < distanciaPar2)
    {
        return par1;
    }
    else if (distanciaPar1 > distanciaPar2)
    {
        return par2;
    }
    else
    {
        // Si las distancias son iguales, comparar por la suma de poblaciones
        if ((par1->ciudad1->poblacion + par1->ciudad2->poblacion) >
            (par2->ciudad1->poblacion + par2->ciudad2->poblacion))
        {
            return par1;
        }
        else
        {
            return par2;
        }
    }
}

ParCiudades *verificarFranja(Ciudad **franja, int cantCiudades, int menorDistancia, int inicio)
{
    // Ordenamos la franja por coordenadas en y (ascendente)
    for (int i = 0; i < cantCiudades; i++)
    {
        for (int j = i + 1; j < cantCiudades; j++)
        {
            if (franja[i]->y > franja[j]->y)
            {
                Ciudad *aux = franja[i];
                franja[i] = franja[j];
                franja[j] = aux;
            }
        }
    }

    ParCiudades *parRetorno = new ParCiudades(franja[inicio], franja[inicio + 1]);

    // Buscar el mejor par dentro de la franja
    for (int i = 0; i < cantCiudades; i++)
    {
        for (int j = i + 1; j < cantCiudades && franja[j]->y - franja[i]->y < menorDistancia; j++)
        {
            ParCiudades *parActual = new ParCiudades(franja[i], franja[j]);
            parRetorno = compararPares(parRetorno, parActual);
        }
    }

    return parRetorno;
}

ParCiudades *mejorParDAC(Ciudad **ciudades, int inicio, int fin)
{
    // CB: 2 ciudades, retorna el par
    if (fin - inicio == 1)
    {
        return new ParCiudades(ciudades[inicio], ciudades[fin]);
    }

    // CB: 3 ciudades, retorna el par con menor distancia efectiva (comparando todas las distancias)
    if (fin - inicio == 2)
    {
        ParCiudades *par1 = new ParCiudades(ciudades[inicio], ciudades[inicio + 1]);
        ParCiudades *par2 = new ParCiudades(ciudades[inicio], ciudades[inicio + 2]);
        ParCiudades *par3 = new ParCiudades(ciudades[inicio + 1], ciudades[inicio + 2]);
        ParCiudades *mejorPar = compararPares(par1, par2);
        mejorPar = compararPares(mejorPar, par3);
        return mejorPar;
    }

    // Calculamos el medio
    int medio = (inicio + fin) / 2;

    // Calculamos el mejor par del lado izquierdo llamando recursivamente
    ParCiudades *parIzq = mejorParDAC(ciudades, inicio, medio);

    // Calculamos el mejor par del lado derecho llamando recursivamente
    ParCiudades *parDer = mejorParDAC(ciudades, medio + 1, fin);

    // Obtenemos el par con menor distancia efectiva comparando los dos pares
    ParCiudades *mejorPar = compararPares(parDer, parIzq);

    // Obtenemos la distancia efectiva entre las ciudades del mejor par
    double menorDistancia = distanciaEfectiva(mejorPar->ciudad1, mejorPar->ciudad2);

    // Construimos la franja de ciudades cercanas a la línea media
    int tamFranja = 0;
    Ciudad **franja = construirFranja(ciudades, inicio, fin, medio, menorDistancia, tamFranja);

    // Si la franja tiene más de un elemento, verificamos los pares dentro de la franja
    if (tamFranja > 1)
    {
        ParCiudades *mejorParFranja = verificarFranja(franja, tamFranja, menorDistancia, inicio);
        mejorPar = compararPares(mejorPar, mejorParFranja);
    }

    delete[] franja; 

    return mejorPar;
}

// Función para encontrar el par de ciudades llamando a DyC
ParCiudades *mejorPar(Ciudad **ciudades, int cantCiudades)
{
    // Ordenar el vector de ciudades por coordenada en x (ascendente)
    for (int i = 0; i < cantCiudades - 1; i++)
    {
        for (int j = i + 1; j < cantCiudades; j++)
        {
            if (ciudades[i]->x > ciudades[j]->x)
            {
                Ciudad *aux = ciudades[i];
                ciudades[i] = ciudades[j];
                ciudades[j] = aux;
            }
        }
    }

    // LLamamos a la función de DyC
    return mejorParDAC(ciudades, 0, cantCiudades); 
}

int main()
{
    int cantCiudades;
    cin >> cantCiudades;

    // Inicializamos vector de ciudades
    Ciudad **ciudades = new Ciudad *[cantCiudades];

    // Cargamos poblaciones de cada ciudad
    for (int i = 0; i < cantCiudades; i++)
    {
        double x, y, poblacion;
        cin >> x >> y >> poblacion;
        ciudades[i] = new Ciudad(x, y, poblacion);
    }

    // Encontramos el mejor par de ciudades
    ParCiudades *mejor = mejorPar(ciudades, cantCiudades - 1);

    // Imprimimos las dos ciudades
    if (mejor->ciudad1->x < mejor->ciudad2->x || (mejor->ciudad1->x == mejor->ciudad2->x && mejor->ciudad1->y < mejor->ciudad2->y))
    {
        cout << mejor->ciudad1->x << " " << mejor->ciudad1->y << " " << mejor->ciudad1->poblacion << endl;
        cout << mejor->ciudad2->x << " " << mejor->ciudad2->y << " " << mejor->ciudad2->poblacion << endl;
    }
    else
    {
        cout << mejor->ciudad2->x << " " << mejor->ciudad2->y << " " << mejor->ciudad2->poblacion << endl;
        cout << mejor->ciudad1->x << " " << mejor->ciudad1->y << " " << mejor->ciudad1->poblacion << endl;
    }

    // Liberamos memoria
    for (int i = 0; i < cantCiudades; i++)
    {
        delete ciudades[i];
    }
    delete[] ciudades;

    return 0;
}

