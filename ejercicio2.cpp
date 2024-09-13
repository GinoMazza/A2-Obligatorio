#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "hash.cpp"

using namespace std;

int main(){
    THash* tabla = new THash(11);
    int cant = 0;
    cin >> cant;
    for (int i = 0; i < cant; i++){
        string op;
        int id;
        string titulo;
        cin >> op;
        if(op == "ADD"){
            cin >> id >> titulo;
            tabla->add(id, titulo); 
        } 
        else if(op == "FIND"){
            cin >> id;
            tabla->find(id);
        }
        else if(op == "TOGGLE"){
            cin >> id;
            tabla->toggle(id);
        }
        else if(op == "COUNT") tabla->count();
    }
}