#include <iostream>
using namespace std;

class THash{
    private:
    struct nodoHash {
		int id;
        string titulo;
        char estado;
		nodoHash(int i, string t) : id(i), titulo(t), estado('H') {}
        nodoHash() : estado('V') {}
	};

    nodoHash* tabla;
    bool* borrados;
    int tam;
    int totales;
    int habilitados;

    int fHash1(int id) {
        return id % tam;
    }

    int fHash2(int id) {
        return 1 + (id % (tam - 1));
    }

    nodoHash* rehash(nodoHash* nodo){

    }

    void destruir(nodoHash* nodo){
        delete[] nodo;
        delete[] borrados;
        nodo = NULL;
        borrados = NULL;
    }

    int proximoVacio(int bucket, int id) {
        int i = 1;
        int nuevoBucket = bucket;
        while (tabla[nuevoBucket].estado != 'V' && tabla[nuevoBucket].id != id && i < tam) {
            nuevoBucket = (bucket + i * fHash2(id)) % tam;
            i++;
        }
        return nuevoBucket;
    }


    void insertarAux(nodoHash* nodo, int id, string titulo, int bucket){ 
        if(nodo[bucket].estado == 'V'){
            nodoHash nuevo;
            nuevo.id = id;
            nuevo.titulo = titulo;
            nuevo.estado = 'H';
            nodo[bucket] = nuevo;
            totales++;
            habilitados++;
        }
        else if(nodo[bucket].id == id){
            nodo[bucket].titulo = titulo;
            if(nodo[bucket].estado = 'D'){
                nodo[bucket].estado = 'H';
                habilitados++;
            }
        }
        else if(nodo[bucket].id != id){
            int nuevoBucket = proximoVacio(bucket);
            insertarAux(nodo, id, titulo, nuevoBucket);
        }
    }

    void toggleAux(nodoHash* nodo, int id){
        if(!nodo) cout << "libro_no_encontrado" << endl;
        else{
            if(nodo->estado == 'D'){
                nodo->estado = 'H';
                habilitados++;
            }
            else{
                nodo->estado = 'D';
                habilitados--;
            }
        }    
    }

    void findAux(nodoHash* nodo, int id){
        if(!nodo) cout << "libro_no_encontrado" << endl;
        else cout << nodo->titulo << " " << nodo->estado << endl;
    }

    public:
    THash(int largo){
        tabla = new nodoHash[tam]();
        borrados = new bool[tam]();
        tam = largo;
		totales = 0;
		habilitados = 0;
    }    

    ~THash(){
		destruir(tabla);
        tam = 0;
		totales = 0;
		habilitados = 0;
	}

    void add(int id, string titulo) {
        float fc = totales / (float)tam;
        if(fc > 0,7) rehash(tabla);
        int bucket = fHash1(id);
        insertarAux(tabla, id, titulo, bucket);
	}

    void find(int id){
        
    }

    void toggle(int id){
        
    }

    void count(){
        cout << totales << " " << habilitados << " " << totales-habilitados << endl;
    }
};
