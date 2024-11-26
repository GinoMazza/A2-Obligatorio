#include <cassert>
#include <cstring>
#include <iostream>
#include <climits>

using namespace std;

// Struct para flores
struct Flor
{
    string color;
    int fila;
    Flor(string c, int f) : color(c), fila(f) {}
    Flor() {}
};

// Aunque no sea la mejor, toda cantidad de flores puestas es una solución
bool esSolucion()
{
    return true;
}

// Es mejor solución si mejora la cantidad de flores puestas
bool esMejorSolucion(int floresPuestasActuales, int maxFloresPuestas)
{
    return floresPuestasActuales > maxFloresPuestas;
}

// Una flor es válida si es menor a la cantidad de flores totales y mayor o igual a 0
bool esFlorValida(int florActual, int cantFloresDistintas)
{
    return florActual < cantFloresDistintas && florActual >= 0;
}

// Verifica que no haya otra flor del mismo color en celdas adyacentes y que pueda ser puesta en la fila actual
bool puedoPonerFlor(int filaActual, int columnaActual, int florActual, Flor *flores, string **jardin, int tamJardin)
{
    if (flores[florActual].fila != -1 && flores[florActual].fila != filaActual)
    {
        return false;
    }
    for (int i = filaActual - 1; i <= filaActual + 1; i++)
    {
        for (int j = columnaActual - 1; j <= columnaActual + 1; j++)
        {
            if (i >= 0 && i < tamJardin && j >= 0 && j < tamJardin)
            {
                // Si hay alguna flor adyacente del mismo color
                if (jardin[i][j] == flores[florActual].color)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// Poner la flor implica aumentar la cantidad de flores y reemplazar el color de la celda por el de la flor usada
void ponerFlor(int filaActual, int columnaActual, int florActual, Flor *flores, string **jardin, int &floresPuestasActuales)
{
    floresPuestasActuales++;
    jardin[filaActual][columnaActual] = flores[florActual].color;
}

// Sacar la flor implica disminuir la cantidad de flores y volver a poner la celda vacia
void sacarFlor(int filaActual, int columnaActual, int florActual, Flor *flores, string **jardin, int &floresPuestasActuales)
{
    floresPuestasActuales--;
    jardin[filaActual][columnaActual] = "";
}

// Devuelve el índice de la siguiente celda, bajando de fila si visite todas las columnas
void siguienteCelda(int filaActual, int columnaActual, int tamJardin, int &filaNueva, int &columnaNueva)
{
    if (columnaActual == tamJardin - 1)
    {
        filaNueva = filaActual + 1;
        columnaNueva = 0;
    }
    else
    {
        filaNueva = filaActual;
        columnaNueva = columnaActual + 1;
    }
}

// Funcion para podar en caso de no poder superar la mejor cantidad de flores
bool noPuedoSuperar(int tamJardin, int filaActual, int columnaActual, int maxFloresPuestas, int floresPuestasActuales)
{
    int celdasTotales = tamJardin * tamJardin;
    int celdasVisitadas = filaActual * tamJardin + columnaActual;
    int celdasRestantes = celdasTotales - celdasVisitadas;

    // Se que no puedo superarlo
    if (floresPuestasActuales + celdasRestantes <= maxFloresPuestas)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Backtracking
void floresBT(int filaActual, int columnaActual, int cantFloresDistintas, Flor *flores, int tamJardin, string **jardin, int floresPuestasActuales, int &maxFloresPuestas)
{
    // Si es mejor se actualiza la cantidad de flores puestas (cantidad máxima)
    if (esSolucion() && esMejorSolucion(floresPuestasActuales, maxFloresPuestas))
    {
        maxFloresPuestas = floresPuestasActuales;
    }

    // CB: Recorrimos todo el jardín
    if (filaActual >= tamJardin)
    {
        return;
    }

    // CB: Podamos si con las celdas que nos quedan libres no podemos superar la mejor cantidad de flores
    if (noPuedoSuperar(tamJardin, filaActual, columnaActual, maxFloresPuestas, floresPuestasActuales))
    {
        return;
    }

    // Obtenemos siguiente fila y columna
    int filaNueva, columnaNueva;
    siguienteCelda(filaActual, columnaActual, tamJardin, filaNueva, columnaNueva);

    // Para cada flor pruebo ponerla en la celda actual
    for (int florActual = 0; florActual < cantFloresDistintas; florActual++)
    {
        if (esFlorValida(florActual, cantFloresDistintas))
        {
            // Hay que verificar que pueda usarse la flor
            if (puedoPonerFlor(filaActual, columnaActual, florActual, flores, jardin, tamJardin))
            {
                // Ponemos flor, mandamos recursion y sacamos flor
                ponerFlor(filaActual, columnaActual, florActual, flores, jardin, floresPuestasActuales);

                floresBT(filaNueva, columnaNueva, cantFloresDistintas, flores, tamJardin, jardin, floresPuestasActuales, maxFloresPuestas);

                sacarFlor(filaActual, columnaActual, florActual, flores, jardin, floresPuestasActuales);
            }
        }
    }
    // No colocar ninguna flor en la celda actual
    floresBT(filaNueva, columnaNueva, cantFloresDistintas, flores, tamJardin, jardin, floresPuestasActuales, maxFloresPuestas);
}

int main()
{
    int tamJardin, cantFloresDistintas;
    cin >> tamJardin >> cantFloresDistintas;

    // Inicializamos matriz (jardín)
    string **jardin = new string *[tamJardin];
    for (int i = 0; i < tamJardin; i++)
    {
        jardin[i] = new string[tamJardin];
        for (int j = 0; j < tamJardin; j++)
        {
            jardin[i][j] = "";
        }
    }

    // Inicializamos vector con datos de las flores
    Flor *flores = new Flor[cantFloresDistintas];
    for (int i = 0; i < cantFloresDistintas; i++)
    {
        string color;
        int fila;
        cin >> color >> fila;
        Flor nuevaFlor = Flor(color, fila);
        flores[i] = nuevaFlor;
    }

    int maximasFlores = 0;

    // Llamamos a la funcion de backtracking
    floresBT(0, 0, cantFloresDistintas, flores, tamJardin, jardin, 0, maximasFlores);

    cout << maximasFlores << endl;

    delete[] flores;

    return 0;
}