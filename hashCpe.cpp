#include <iostream>
#include <cassert>

using namespace std;

class HashCPE
{
private:
    struct nodoHash
    {
        int id;
        int posHeap;
        nodoHash *sig;
        nodoHash(int i, int p) : id(i), posHeap(p), sig(NULL) {}
    };

    nodoHash **tabla;
    int tam;
    int cantElementos;

    // Función para verificar si un número es primo
    bool esPrimo(int n)
    {
        if (n <= 1)
            return false; // Los números menores o iguales a 1 no son primos
        if (n == 2 || n == 3)
            return true; // 2 y 3 son primos
        if (n % 2 == 0 || n % 3 == 0)
            return false; // Números divisibles por 2 o 3 no son primos

        // Probar divisores hasta la raíz cuadrada de n
        for (int i = 5; i * i <= n; i += 6)
        {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }
        return true;
    }

    // Función para encontrar el primo mayor más cercano
    int primoMayorCercano(int n)
    {
        int mayor = n + 1;

        // Buscar hacia arriba el próximo primo
        while (!esPrimo(mayor))
        {
            mayor++;
        }

        return mayor;
    }

    // Función que retorna el número primo más grande menor que el tamaño de la tabla
    int obtenerPrimoMenor(int tama)
    {
        for (int i = tama - 1; i >= 2; i--)
        {
            if (esPrimo(i))
            {
                return i;
            }
        }
        // Si no se encuentra ningún primo, retorna -1
        return -1;
    }

    int fHash(int id, int tama)
    {
        return abs(id) % tama;
    }

    bool esVacia()
    {
        return cantElementos == 0;
    }

    void destruir()
    {
        for (int i = 0; i < tam; i++)
        {
            delete tabla[i];
            tabla[i] = NULL;
        }
        delete[] tabla;
        tabla = NULL;
    }

    void rehash()
    {
        int nuevoTam = primoMayorCercano(tam * 2);
        nodoHash **nuevaTabla = new nodoHash *[nuevoTam]();
        for (int i = 0; i < tam; i++)
        {
            nodoHash *aux = tabla[i];
            while (aux)
            {
                int bucket = fHash(aux->id, nuevoTam);
                nodoHash *nuevo = new nodoHash(aux->id, aux->posHeap);
                nuevo->sig = nuevaTabla[bucket];
                nuevaTabla[bucket] = nuevo;
                nodoHash *aBorrar = aux;
                aux = aux->sig;
                delete aBorrar;
            }
        }
        delete[] tabla;
        tabla = nuevaTabla;
        tam = nuevoTam;
    }

    void insertarAux(nodoHash *&lista, int id, int pos)
    {
        if (!lista)
        {
            lista = new nodoHash(id, pos);
            cantElementos++;
            return;
        }
        else
        {
            insertarAux(lista->sig, id, pos);
        }
    }

    void eliminarAux(nodoHash *&lista, int id)
    {
        if (lista->id == id)
        {
            nodoHash *aBorrar = lista;
            lista = lista->sig;
            delete aBorrar;
            aBorrar = NULL;
            cantElementos--;
        }
        else
        {
            eliminarAux(lista->sig, id);
        }
    }

    void intercambiarAux(nodoHash *l1, nodoHash *l2, int id1, int id2)
    {
        while (l1->id != id1)
        {
            l1 = l1->sig;
        }
        while (l2->id != id2)
        {
            l2 = l2->sig;
        }
        int aux = l1->posHeap;
        l1->posHeap = l2->posHeap;
        l2->posHeap = aux;
    }

    int getPosAux(nodoHash *lista, int id)
    {
        while (lista)
        {
            if (lista->id == id)
            {
                return lista->posHeap;
            }
            lista = lista->sig;
        }
        return -1;
    }

public:
    HashCPE(int largo)
    {
        tabla = new nodoHash *[largo]();
        tam = largo;
        cantElementos = 0;
    }

    HashCPE() {}

    ~HashCPE()
    {
        destruir();
        tam = 0;
        cantElementos = 0;
    }

    void insertar(int id, int pos)
    {
        float fc = (float)cantElementos / (float)tam;
        if (fc > 0.7)
        {
            rehash();
        }
        int bucket = fHash(id, tam);
        insertarAux(tabla[bucket], id, pos);
    }

    void eliminar(int id)
    {
        if (esVacia())
            return;
        int bucket = fHash(id, tam);
        eliminarAux(tabla[bucket], id);
    }

    void intercambiar(int id1, int id2)
    {
        int bucket1 = fHash(id1, tam);
        int bucket2 = fHash(id2, tam);
        intercambiarAux(tabla[bucket1], tabla[bucket2], id1, id2);
    }

    int getPos(int id)
    {
        int bucket = fHash(id, tam);
        return getPosAux(tabla[bucket], id);
    }
};