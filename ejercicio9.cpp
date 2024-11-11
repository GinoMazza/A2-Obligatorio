#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    int j, presupuesto, cantExtranjeros;
    cin >> j;
    int *valoraciones = new int[j + 1]();
    int *costos = new int[j + 1]();
    bool *extranjeros = new bool[j + 1]();
    for (int i = 1; i <= j; i++)
    {
        int valoracion = 0;
        int ritmo, tiro, pase, regate, defensa, fisico, costo;
        cin >> ritmo >> tiro >> pase >> regate >> defensa >> fisico;
        string formaFisica, extranjero, confianza;
        cin >> formaFisica;
        cin >> costo;
        cin >> extranjero >> confianza;
        if (formaFisica != "Lesionado")
        {
            valoracion = (ritmo + tiro + pase + regate + defensa + fisico) / 6;
            if (formaFisica == "Buena")
                valoracion += 5;
            else if (formaFisica == "Mala")
                valoracion -= 5;
            if (confianza == "Alta")
                valoracion += 5;
            else if (confianza == "Baja")
                valoracion -= 5;
        }
        valoraciones[i] = valoracion;
        costos[i] = costo;
        extranjeros[i] = extranjero == "Si" ? true : false;
    }
    cin >> presupuesto >> cantExtranjeros;
}

// return final;