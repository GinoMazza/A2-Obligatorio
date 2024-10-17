#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>

using namespace std;

class Cola{
    private:
    
    struct nodoCola{
        int dato;
        nodoCola* sig;
        nodoCola(int e) : dato(e), sig(NULL){}
    };

    nodoCola* principio;    
    nodoCola* ultimo;
    int cantElementos;

    public:

    Cola() {
        principio = ultimo = NULL;
        cantElementos = 0;
    }

    ~Cola() {
        while (!esVacia()) desencolar();
        delete principio;
        delete ultimo;
    }

    void encolar(int e) {
        nodoCola* nuevo = new nodoCola(e);
        if (esVacia()) principio = nuevo;
        else ultimo->sig = nuevo;
        ultimo = nuevo;
        cantElementos++;
    }

    int desencolar() {
        if (esVacia()) return -1;
        int ret = principio->dato;
        nodoCola* aBorrar = principio;
        principio = principio->sig;
        if (!principio) ultimo = NULL;
        delete aBorrar;
        cantElementos--;
        return ret;
    }

    bool esVacia() {
        return cantElementos == 0;
    }

    int cantidadElementos() {
        return cantElementos;
    }
};
 