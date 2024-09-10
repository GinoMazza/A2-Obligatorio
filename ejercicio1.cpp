#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <avl.cpp>

using namespace std;

int main(){
    AVL* arbol = new AVL();
    int cant = 0;
    cin >> cant;
    for (int i = 0; i < cant; i++){
        string op;
        int id;
        string titulo;
        cin >> op;
        if(op._Equal("ADD")){
            cin >> id >> titulo;
            arbol->add(id, titulo); 
        } 
        else if(op._Equal("FIND")){
            cin >> id;
            arbol->find(id);
        }
        else if(op._Equal("TOGGLE")){
            cin >> id;
            arbol->toggle(id);
        }
        else if(op._Equal("COUNT")) arbol->count();
    }
}