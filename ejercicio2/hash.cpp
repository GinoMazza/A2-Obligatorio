#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <limits>

using namespace std;

class THash{
    private:
    struct nodoHash {
		int id;
        string titulo;
        char estado;
		nodoHash(int i, string t) : id(i), titulo(t), estado('H') {}
        nodoHash(int i, string t, char e) : id(i), titulo(t), estado(e) {}
	};

    nodoHash** tabla;
    int tam;
    int totales;
    int habilitados;

    int proximoParaRevisar(int bucket, int id){
        int i = 1;
        int nuevoBucket = bucket;
        bool sigo = true;
        while (tabla[nuevoBucket] != NULL && tabla[nuevoBucket]->id != id && i < tam && sigo) {
            nuevoBucket = (bucket + i * fHash2(id, tam)) % tam;
            i++;
            if(i == tam) sigo = false;
        }
        return nuevoBucket;
    }

    // Función para verificar si un número es primo
    bool esPrimo(int n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

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
        while (!esPrimo(mayor)) {
            mayor++;
        }
        return mayor;
    }

    // Función que retorna el número primo más grande menor que el tamaño de la tabla
    int obtenerPrimoMenor(int tama) {
        for (int i = tama - 1; i >= 2; i--) {
            if (esPrimo(i)) {
                return i;
            }
        }
        return -1;
    }

    int fHash1(int id, int tama) {
        return id % tama;
    }

    int fHash2(int id, int tama) {
        int primoMenor = obtenerPrimoMenor(tama);
        return primoMenor - (id % primoMenor);
    }

    int proximoVacio(int id, int i, int tama) {
        int hashValue = fHash1(id, tama);
        int hashValue2 = fHash2(id, tama);
        int pos = abs(hashValue + i * hashValue2) % tama;
        return pos;
    }

    void insRehash(nodoHash** nuevaTabla, int id, string titulo, char estado, int tama) {
        int i = 0;
        bool inserte = false;

        while (!inserte) {
            int bucket = proximoVacio(id, i, tama); 
            if (!nuevaTabla[bucket]) {
                nodoHash* nuevo = new nodoHash(id, titulo, estado);
                nuevaTabla[bucket] = nuevo;
                inserte = true;
            } else {
                i++;
            }
        }
    }

    void rehash(){
        int nuevoTam = primoMayorCercano(tam * 2);
        nodoHash** nuevaTabla = new nodoHash*[nuevoTam]();
        for (int i = 0; i < tam; i++) {
            if(tabla[i]){
                insRehash(nuevaTabla, tabla[i]->id, tabla[i]->titulo, tabla[i]->estado, nuevoTam);
                delete tabla[i];
                tabla[i] = NULL;
            }
        }
        delete[] tabla;
        tabla = nuevaTabla;
        tam = nuevoTam;
    }

    void destruir(){
        for(int i = 0; i<tam; i++){
            delete tabla[i];
            tabla[i] = NULL;
        }
        delete[] tabla;
        tabla = NULL;
    }

    void insertarAux(nodoHash** tab, int id, string titulo, int tama){
        int i = 0;
		int inserte = false;

		while (!inserte){
			int bucket = proximoVacio(id, i, tama);
			if (!tab[bucket]){ 
                nodoHash* nuevo = new nodoHash(id, titulo);
                tab[bucket] = nuevo;
                totales++;
                habilitados++;
                inserte = true;
			}
			else if (tab[bucket]->id == id){ 
				tab[bucket]->titulo = titulo;
                if(tab[bucket]->estado == 'D'){
                    tab[bucket]->estado = 'H';
                    habilitados++;
                }
                inserte = true;
			} 
            else i++;
		} 
        
    }

    void findAux(int id, int bucket){
        if(!tabla[bucket]) cout << "libro_no_encontrado" << endl;
        else if(tabla[bucket]->id == id) cout << tabla[bucket]->titulo << " " << tabla[bucket]->estado << endl;
        else if(tabla[bucket]->id != id) findAux(id, proximoParaRevisar(bucket, id));
    }

    void toggleAux(int id, int bucket){
        if(!tabla[bucket]) cout << "libro_no_encontrado" << endl;
        else if(tabla[bucket]->id == id){
            if(tabla[bucket]->estado == 'D'){
                tabla[bucket]->estado = 'H';
                habilitados++;
            }
            else{
                tabla[bucket]->estado = 'D';
                habilitados--;
            }
        }
        else if(tabla[bucket]->id != id) toggleAux(id, proximoParaRevisar(bucket, id));  
    }

    public:
    THash(int largo){
        tabla = new nodoHash*[largo]();
        tam = largo;
		totales = 0;
		habilitados = 0;
    }    

    ~THash(){
		destruir();
	}

    void add(int id, string titulo) {
        float fc = (float)totales / (float)tam;
        if(fc > 0.7) rehash();
        insertarAux(tabla, id, titulo, tam);
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