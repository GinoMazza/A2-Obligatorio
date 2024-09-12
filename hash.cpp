#include <iostream>
using namespace std;

class THash{
    private:
    struct nodoHash {
		int id;
        string titulo;
        char estado;
		nodoHash(int i, string t) : id(i), titulo(t), estado('H') {}
	};

    nodoHash* tabla;
    int totales;
    int habilitados;

    void destruir(nodoHash* vector){
        delete[] vector;
        vector = NULL;
    }

    public:
    THash(){
        tabla = NULL;
		totales = 0;
		habilitados = 0;
    }    

    ~THash(){
		destruir(tabla);
		totales = 0;
		habilitados = 0;
	}
};
