#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

// Función que devuelve el máximo de dos números
int max(int a, int b)
{
    return a > b ? a : b;
}

// Función mochila de 4 dimensiones
void mochila4D(int cantJugadores, int cantExtranjeros, int cantPresupuesto, int *valoraciones, int *salarios, int *extranjeros)
{
    // Generamos la matriz de tabulación (4 dimensiones)
    int ****tab = new int ***[cantJugadores + 1];

    for (int jugadorActual = 0; jugadorActual <= cantJugadores; jugadorActual++)
    {
        tab[jugadorActual] = new int **[cantPresupuesto + 1]();

        for (int presupuestoActual = 0; presupuestoActual <= cantPresupuesto; presupuestoActual++)
        {
            tab[jugadorActual][presupuestoActual] = new int *[cantExtranjeros + 1]();

            for (int cantExtranjerosActual = 0; cantExtranjerosActual <= cantExtranjeros; cantExtranjerosActual++)
            {
                tab[jugadorActual][presupuestoActual][cantExtranjerosActual] = new int[12](); // 12 porque son 11 jugadores + 1
            }
        }
    }

    for (int jugadorActual = 1; jugadorActual <= cantJugadores; jugadorActual++)
    {
        // Obtenemos los datos del jugador actual
        int valoracionJugadorActual = valoraciones[jugadorActual];
        int salarioJugadorActual = salarios[jugadorActual];
        int esExtranjeroJugadorActual = extranjeros[jugadorActual];

        for (int presupuestoActual = 0; presupuestoActual <= cantPresupuesto; presupuestoActual++)
        {
            for (int cantExtranjerosActual = 0; cantExtranjerosActual <= cantExtranjeros; cantExtranjerosActual++)
            {
                for (int cantJugadoresMaximosActual = 1; cantJugadoresMaximosActual <= 11; cantJugadoresMaximosActual++)
                {
                    // Si el jugador actual no entra (ya sea por capacidad de salario o extranjero)
                    if (salarioJugadorActual > presupuestoActual || esExtranjeroJugadorActual > cantExtranjerosActual)
                    {
                        tab[jugadorActual][presupuestoActual][cantExtranjerosActual][cantJugadoresMaximosActual] = tab[jugadorActual - 1][presupuestoActual][cantExtranjerosActual][cantJugadoresMaximosActual];
                    }

                    // Entra
                    else
                    {
                        // Comparamos valores de usar y no usar al jugador actual
                        int valorDeUsar = valoracionJugadorActual + tab[jugadorActual - 1][presupuestoActual - salarioJugadorActual][cantExtranjerosActual - esExtranjeroJugadorActual][cantJugadoresMaximosActual - 1];

                        int valorDeNoUsar = tab[jugadorActual - 1][presupuestoActual][cantExtranjerosActual][cantJugadoresMaximosActual];

                        // Se guarda el máximo de los valores
                        tab[jugadorActual][presupuestoActual][cantExtranjerosActual][cantJugadoresMaximosActual] = max(valorDeUsar, valorDeNoUsar);
                    }
                }
            }
        }
    }

    // El valor máximo alcanzado con los 11 jugadores seleccionados
    int valoracionTotal = tab[cantJugadores][cantPresupuesto][cantExtranjeros][11];

    // Devolvemos el promedio
    int promedioValoracion = valoracionTotal / 11;
    cout << promedioValoracion << endl;
}

int main()
{
    int j, presupuesto, cantExtranjeros;
    cin >> j;

    // Inicializamos vectores
    int *valoraciones = new int[j + 1]();
    int *salarios = new int[j + 1]();
    int *extranjeros = new int[j + 1]();

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
        // Guardamos datos de cada jugador
        valoraciones[i] = valoracion;
        salarios[i] = costo;
        extranjeros[i] = extranjero == "Si" ? 1 : 0;
    }

    cin >> presupuesto >> cantExtranjeros;

    // LLamamos a la mochila
    mochila4D(j, cantExtranjeros, presupuesto, valoraciones, salarios, extranjeros);

    delete[] valoraciones;
    delete[] salarios;
    delete[] extranjeros;

    return 0;
}
