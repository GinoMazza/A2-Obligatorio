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

    //**********************GPT PRUEBAS******************************
    // Función para verificar si un número es primo
    bool esPrimo(int n) {
        if (n <= 1) return false; // Los números menores o iguales a 1 no son primos
        if (n == 2 || n == 3) return true; // 2 y 3 son primos
        if (n % 2 == 0 || n % 3 == 0) return false; // Números divisibles por 2 o 3 no son primos

        // Probar divisores hasta la raíz cuadrada de n
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }
        return true;
    }

    // Función para encontrar el primo mayor más cercano
    int primoMayorCercano(int n) {
        int mayor = n + 1;

        // Buscar hacia arriba el próximo primo
        while (!esPrimo(mayor)) {
            mayor++;
        }

        return mayor;
    }
    //*********************FIN GPT PRUEBAS**************************

    int fHash1(int id, int tama) {
        return id % tama;
    }

    int fHash2(int id, int tama) {
        return 1 + (id % (tama - 1));
    }

    void insertarRehash(nodoHash* nuevaTabla, int id, string titulo, char estado, int bucket, int tama){
        if(nuevaTabla[bucket].estado == 'V'){
            nodoHash nuevo;
            nuevo.id = id;
            nuevo.titulo = titulo;
            nuevo.estado = estado;
            nuevaTabla[bucket] = nuevo;
        }
        else {
            int nuevoBucket = proximoVacio(nuevaTabla, id, bucket, tama);
            insertarRehash(nuevaTabla, id, titulo, estado, nuevoBucket, tama);
        }
    }

    void rehash(){
        int nuevoTam = primoMayorCercano(tam); //proxPrimo?
        nodoHash* nuevaTabla = new nodoHash[nuevoTam]();
        for (int i = 0; i < tam; i++) {
            if(tabla[i].estado != 'V'){
                int bucket = fHash1(tabla[i].id, nuevoTam);
                insertarRehash(nuevaTabla, tabla[i].id, tabla[i].titulo, tabla[i].estado, bucket, nuevoTam);
            }
        }
        delete[] tabla;
        tabla = nuevaTabla;
        tam = nuevoTam;
    }

    void destruir(){
        delete[] tabla;
        tabla = NULL;
    }

    int proximoVacio(nodoHash* tab, int bucket, int id, int tama) {
        int i = 1;
        int nuevoBucket = bucket;
        bool sigo = true;
        while (tab[nuevoBucket].estado != 'V' && tab[nuevoBucket].id != id && i < tama && sigo) {
            nuevoBucket = (bucket + i * fHash2(id, tama)) % tama;
            i++;
            if(i == tama) sigo = false;
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
            if(tabla[bucket].estado == 'D'){
                tabla[bucket].estado = 'H';
                habilitados++;
            }
        }
        else if(tabla[bucket].id != id){
            int nuevoBucket = proximoVacio(tabla, bucket, id, tam);
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
        else if(tabla[bucket].id != id) toggleAux(id, proximoVacio(tabla, bucket, id, tam));  
    }

    void findAux(int id, int bucket){
        if(tabla[bucket].estado == 'V') cout << "libro_no_encontrado" << endl;
        else if(tabla[bucket].id == id) cout << tabla[bucket].titulo << " " << tabla[bucket].estado << endl;
        else if(tabla[bucket].id != id) findAux(id, proximoVacio(tabla, bucket, id, tam));
    }

    public:
    THash(int largo){
        tabla = new nodoHash[largo]();
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
        if(fc > 0.7) rehash();
        int bucket = fHash1(id, tam);
        insertarAux(id, titulo, bucket);
	}

    void find(int id){
        int bucket = fHash1(id, tam);
        findAux(id, bucket);
    }

    void toggle(int id){
        int bucket = fHash1(id, tam);
        toggleAux(id, bucket);
    }

    void count(){
        cout << totales << " " << habilitados << " " << totales-habilitados << endl;
    }
};
