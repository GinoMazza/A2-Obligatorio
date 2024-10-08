#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "cpe.cpp"

using namespace std;

int main(){
    int N = 0;
    int O = 0;
    cin >> N;
    cin >> O;
    CPE* cpe = new CPE(N);
    for (int i = 0; i < O; i++){
        string operacion;
        int id;
        int prioridad;
        string paraLlevarStr;
        string items;
        cin >> operacion;
        if(operacion == "I"){
            cin >> id >> prioridad >> paraLlevarStr >> items;
            bool paraLlevar = (paraLlevarStr == "true");
            cpe->insertar(id, prioridad, paraLlevar, items);
        }
        else if(operacion == "E"){
            cin >> id;
            cpe->entregar(id);
        }
        else if(operacion == "C"){
            cin >> id;
            cpe->cambiar(id);
        }
    }
    cpe->imprimir();
}