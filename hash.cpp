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
    int tam;
    int totales;
    int habilitados;

    int fHash1(int id) {
        return id % tam;
    }

    int fHash2(int id) {
        return 1 + (id % (tam - 1));
    }

    nodoHash* rehash(){
        //Implementar
    }

    void destruir(){
        delete[] tabla;
        tabla = NULL;
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


    void insertarAux(int id, string titulo, int bucket){ 
        if(tabla[bucket].estado == 'V'){
            nodoHash nuevo;
            nuevo.id = id;
            nuevo.titulo = titulo;
            nuevo.estado = 'H';
            tabla[bucket] = nuevo;
            totales++;
            habilitados++;
        }
        else if(tabla[bucket].id == id){
            tabla[bucket].titulo = titulo;
            if(tabla[bucket].estado = 'D'){
                tabla[bucket].estado = 'H';
                habilitados++;
            }
        }
        else if(tabla[bucket].id != id){
            int nuevoBucket = proximoVacio(bucket, id);
            insertarAux(id, titulo, nuevoBucket);
        }
    }

    void toggleAux(int id, int bucket){
        if(tabla[bucket].estado == 'V') cout << "libro_no_encontrado" << endl;
        else if(tabla[bucket].id == id){
            if(tabla[bucket].estado == 'D'){
                tabla[bucket].estado = 'H';
                habilitados++;
            }
            else{
                tabla[bucket].estado = 'D';
                habilitados--;
            }
        }
        else if(tabla[bucket].id != id) toggleAux(id, proximoVacio(bucket, id));  
    }

    void findAux(int id, int bucket){
        if(tabla[bucket].estado == 'V') cout << "libro_no_encontrado" << endl;
        else if(tabla[bucket].id == id) cout << tabla[bucket].titulo << " " << tabla[bucket].estado << endl;
        else if(tabla[bucket].id != id) findAux(id, proximoVacio(bucket, id));
    }

    public:
    THash(int largo){
        tabla = new nodoHash[tam]();
        tam = largo;
		totales = 0;
		habilitados = 0;
    }    

    ~THash(){
		destruir();
        tam = 0;
		totales = 0;
		habilitados = 0;
	}

    void add(int id, string titulo) {
        float fc = totales / (float)tam;
        if(fc > 0,7) rehash();
        int bucket = fHash1(id);
        insertarAux(id, titulo, bucket);
	}

    void find(int id){
        int bucket = fHash1(id);
        findAux(id, bucket);
    }

    void toggle(int id){
        int bucket = fHash1(id);
        toggleAux(id, bucket);
    }

    void count(){
        cout << totales << " " << habilitados << " " << totales-habilitados << endl;
    }
};
