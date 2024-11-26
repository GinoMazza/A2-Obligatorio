#include <cassert>
#include <cstring>
#include <iostream>
#include <climits>

using namespace std;

// Función que verifica si es posible repartir las canciones entre los estudiantes sin superar el tiempo maximo ni la cantidad de alumnos necesarios
bool repartirNoSuperaMax(int *duraciones, int cantCanciones, int cantEstudiantes, int tiempoMaximo, int *&posicionesInicio, int *&posicionesFinal, int *&tiemposTotales)
{
    int estudiantes = 1; 
    int tiempoActual = 0;

    for (int i = 1; i <= cantCanciones; i++)
    {
        int duracionActual = duraciones[i];

        // Si al agregar la canción excedemos el tiempo permitido para este estudiante
        if (tiempoActual + duracionActual > tiempoMaximo)
        {
            posicionesFinal[estudiantes] = i - 1;
            tiemposTotales[estudiantes] = tiempoActual;
            estudiantes++;                              
            posicionesInicio[estudiantes] = i;          
            tiempoActual = duracionActual;              

            // Si necesitamos más estudiantes de los permitidos para repartir
            if (estudiantes > cantEstudiantes)
            {
                return false;
            }
        }
        else
        {
            // Acumulamos la duración de la canción para este estudiante
            tiempoActual += duracionActual;
        }
    }

    // Actualizamos para el ultimo estudiante, ya que no se actualiza en el for
    posicionesFinal[estudiantes] = cantCanciones;
    tiemposTotales[estudiantes] = tiempoActual;

    return true;
}

int main()
{
    // Inicializamos variables y vector de duraciones de las canciones
    int cantCanciones, cantEstudiantes;
    int duracionMinima = INT_MIN;
    int duracionTotal = 0;
    cin >> cantCanciones;
    int *duraciones = new int[cantCanciones + 1]();

    // Cargamos vector
    for (int i = 1; i <= cantCanciones; i++)
    {
        int duracion;
        cin >> duracion;
        duraciones[i] = duracion;
        duracionTotal += duracion;
    }

    cin >> cantEstudiantes;

    // Inicializamos vectores posiciones iniciales y finales de cada estudiante y tiempos totales escuchados por cada estudiante
    int *posicionesInicio = new int[cantEstudiantes + 1]();
    posicionesInicio[1] = 1;
    int *posicionesFinal = new int[cantEstudiantes + 1]();
    int *tiemposTotales = new int[cantEstudiantes + 1]();

    // Obtenemos la duración mayor del vector, que es la minima duracion para escuchar todas las canciones en el mejor caso
    for (int i = 1; i <= cantCanciones; i++)
    {
        if (duraciones[i] > duracionMinima)
        {
            duracionMinima = duraciones[i];
        }
    }

    // Obtenemos duración total de todas las canciones
    for (int i = 1; i <= cantCanciones; i++)
    {
        duracionTotal += duraciones[i];
    }

    int resultado = duracionTotal;

    // Búsqueda binaria
    while (duracionMinima <= duracionTotal)
    {
        // Calculamos el medio
        int medio = (duracionMinima + duracionTotal) / 2;

        for (int i = 1; i <= cantEstudiantes; i++)
        {
            posicionesInicio[i] = 0;
            posicionesFinal[i] = 0;
            tiemposTotales[i] = 0;
        }
        posicionesInicio[1] = 1;

        // Si es posible repartir las canciones entre los estudiantes sin superar el tiempo máximo
        if (repartirNoSuperaMax(duraciones, cantCanciones, cantEstudiantes, medio, posicionesInicio, posicionesFinal, tiemposTotales))
        {
            resultado = medio;
            duracionTotal = medio - 1; // Intentamos buscar un tiempo menor (sabemos que con medio funciona sin ser duracionTotal)
        }
        else
        {
            duracionMinima = medio + 1; // Aumentamos el tiempo máximo permitido
        }
    }

    // Imprimimos resultados
    for (int i = 1; i <= cantEstudiantes; i++)
    {
        if (posicionesFinal[i - 1] != cantCanciones && tiemposTotales[i] != 0)
        {
            cout << tiemposTotales[i] << " " << posicionesInicio[i] << " " << posicionesFinal[i] << endl;
        }
    }
    cout << resultado << endl;

    return 0;
}
