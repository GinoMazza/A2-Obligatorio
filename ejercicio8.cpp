#include <cassert>
#include <cstring>
#include <iostream>
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
    double distancia;
    ParCiudades(Ciudad *c1, Ciudad *c2, double dist) : ciudad1(c1), ciudad2(c2), distancia(dist) {}
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

// Merge para el eje recibido
void merge(Ciudad **ciudades, int inicio, int medio, int fin, char eje)
{
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    // Vector para la izquierda
    Ciudad **izq = new Ciudad *[n1];

    // Vector para la derecha
    Ciudad **der = new Ciudad *[n2];

    // Copiamos datos
    for (int i = 0; i < n1; i++)
        izq[i] = ciudades[inicio + i];
    for (int j = 0; j < n2; j++)
        der[j] = ciudades[medio + 1 + j];

    // Combinamos los arreglos ordenados
    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2)
    {
        // Comparamos en x
        if (eje == 'x')
        {
            if (izq[i]->x <= der[j]->x)
            {
                ciudades[k] = izq[i];
                i++;
            }
            else
            {
                ciudades[k] = der[j];
                j++;
            }
        }
        // Comparamos en y
        else if (eje == 'y')
        {
            if (izq[i]->y <= der[j]->y)
            {
                ciudades[k] = izq[i];
                i++;
            }
            else
            {
                ciudades[k] = der[j];
                j++;
            }
        }
        k++;
    }

    // Copiamos elementos restantes
    while (i < n1)
    {
        ciudades[k] = izq[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        ciudades[k] = der[j];
        j++;
        k++;
    }

    // Liberamos memoria
    delete[] izq;
    delete[] der;
}

// Mergesort para ordenar por coordenadas del eje recibido (ascendentemente)
void mergeSort(Ciudad **ciudades, int inicio, int fin, char eje)
{
    if (inicio < fin)
    {
        int medio = inicio + (fin - inicio) / 2;

        // Ordenamos las mitades
        mergeSort(ciudades, inicio, medio, eje);
        mergeSort(ciudades, medio + 1, fin, eje);

        // Combinamos
        merge(ciudades, inicio, medio, fin, eje);
    }
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
    if (par1->distancia < par2->distancia)
    {
        return par1;
    }
    else if (par1->distancia > par2->distancia)
    {
        return par2;
    }
    else
    {
        // Si las distancias son iguales, comparamos por la suma de poblaciones
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
    // Ordenamos el vector de ciudades por coordenada en y (ascendente)
    mergeSort(franja, 0, cantCiudades - 1, 'y');

    double distancia = distanciaEfectiva(franja[0], franja[1]);
    ParCiudades *parRetorno = new ParCiudades(franja[0], franja[1], distancia);

    // Buscamos el mejor par dentro de la franja
    for (int i = 0; i < cantCiudades; i++)
    {
        for (int j = i + 1; j < cantCiudades && franja[j]->y - franja[i]->y < menorDistancia; j++)
        {
            double distancia = distanciaEfectiva(franja[i], franja[j]);
            ParCiudades *parActual = new ParCiudades(franja[i], franja[j], distancia);
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
        double distancia = distanciaEfectiva(ciudades[inicio], ciudades[fin]);
        return new ParCiudades(ciudades[inicio], ciudades[fin], distancia);
    }

    // CB: 3 ciudades, retorna el par con menor distancia efectiva (comparando todas las distancias)
    if (fin - inicio == 2)
    {
        double distancia1 = distanciaEfectiva(ciudades[inicio], ciudades[inicio + 1]);
        double distancia2 = distanciaEfectiva(ciudades[inicio], ciudades[inicio + 2]);
        double distancia3 = distanciaEfectiva(ciudades[inicio + 1], ciudades[inicio + 2]);
        ParCiudades *par1 = new ParCiudades(ciudades[inicio], ciudades[inicio + 1], distancia1);
        ParCiudades *par2 = new ParCiudades(ciudades[inicio], ciudades[inicio + 2], distancia2);
        ParCiudades *par3 = new ParCiudades(ciudades[inicio + 1], ciudades[inicio + 2], distancia3);
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
    double menorDistancia = mejorPar->distancia;

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
    mergeSort(ciudades, 0, cantCiudades - 1, 'x');

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