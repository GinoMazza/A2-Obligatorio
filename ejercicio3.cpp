#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "heap.cpp"

using namespace std;

int main(){
    int N = 0;
    int K = 0;
    cin >> N;
    MinHeap* heap = new MinHeap(N);
    for (int i = 0; i < N; i++){
        int id;
        int precio;
        cin >> id >> precio;
        heap->insertar(id, precio);
    }
    cin >> K;
    heap->imprimirK(K);
}