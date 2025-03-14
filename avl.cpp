#include <iostream>
using namespace std;

class AVL
{
private:
	struct NodoAVL
	{
		int id;
		string titulo;
		char estado;
		int altura;
		NodoAVL *izq;
		NodoAVL *der;
		NodoAVL(int i, string t) : id(i), titulo(t), estado('H'), izq(NULL), der(NULL), altura(1) {}
	};

	NodoAVL *raiz;
	int totales;
	int habilitados;

	int max(int a, int b)
	{
		return a > b ? a : b;
	}

	// Devuelve la altura del nodo (si es nulo da 0)
	int altura(NodoAVL *n)
	{
		if (!n)
			return 0;
		return n->altura;
	}

	// Calcula el balance del nodo
	int balance(NodoAVL *n)
	{
		if (!n)
			return 0;
		return altura(n->izq) - altura(n->der);
	}

	// Rotaciones
	NodoAVL *rotacionHoraria(NodoAVL *A)
	{
		NodoAVL *B = A->izq;
		NodoAVL *T2 = B->der;
		B->der = A;
		A->izq = T2;
		A->altura = 1 + max(altura(A->izq), altura(A->der));
		B->altura = 1 + max(altura(B->izq), altura(B->der));
		return B;
	}

	NodoAVL *rotacionAntiHoraria(NodoAVL *B)
	{
		NodoAVL *A = B->der;
		NodoAVL *T2 = A->izq;
		A->izq = B;
		B->der = T2;
		B->altura = 1 + max(altura(B->izq), altura(B->der));
		A->altura = 1 + max(altura(A->izq), altura(A->der));
		return A;
	}

	void destruir(NodoAVL *nodo)
	{
		if (!nodo)
			return;
		destruir(nodo->izq);
		destruir(nodo->der);
		delete nodo;
		nodo = NULL;
	}

	NodoAVL *insertarAux(NodoAVL *nodo, int id, string titulo)
	{
		if (!nodo)
		{
			totales++;
			habilitados++;
			return new NodoAVL(id, titulo);
		}
		if (id < nodo->id)
			nodo->izq = insertarAux(nodo->izq, id, titulo);
		else if (id > nodo->id)
			nodo->der = insertarAux(nodo->der, id, titulo);
		else
		{
			nodo->titulo = titulo;
			if (nodo->estado == 'D')
			{
				nodo->estado = 'H';
				habilitados++;
			}
			return nodo;
		}

		// A la vuelta de la recursión, actualizo la altura del nodo en el que estoy
		nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));

		// Verifico si el nodo está desbalanceado
		int balanceo = balance(nodo);

		bool desbalanceoIzq = balanceo > 1;
		bool desbalanceoDer = balanceo < -1;

		// Caso izquierda izquierda
		if (desbalanceoIzq && id < nodo->izq->id)
			return rotacionHoraria(nodo);

		// Caso derecha derecha
		if (desbalanceoDer && id > nodo->der->id)
			return rotacionAntiHoraria(nodo);

		// Caso izquierda derecha
		if (desbalanceoIzq && id > nodo->izq->id)
		{
			nodo->izq = rotacionAntiHoraria(nodo->izq);
			return rotacionHoraria(nodo);
		}

		// Caso derecha izquierda
		if (desbalanceoDer && id < nodo->der->id)
		{
			nodo->der = rotacionHoraria(nodo->der);
			return rotacionAntiHoraria(nodo);
		}

		return nodo;
	}

	void toggleAux(NodoAVL *nodo, int id)
	{
		if (!nodo)
			cout << "libro_no_encontrado" << endl;
		else if (id < nodo->id)
			toggleAux(nodo->izq, id);
		else if (id > nodo->id)
			toggleAux(nodo->der, id);
		else
		{
			if (nodo->estado == 'D')
			{
				nodo->estado = 'H';
				habilitados++;
			}
			else
			{
				nodo->estado = 'D';
				habilitados--;
			}
		}
	}

	void findAux(NodoAVL *nodo, int id)
	{
		if (!nodo)
			cout << "libro_no_encontrado" << endl;
		else if (id < nodo->id)
			findAux(nodo->izq, id);
		else if (id > nodo->id)
			findAux(nodo->der, id);
		else
			cout << nodo->titulo << " " << nodo->estado << endl;
	}

public:
	AVL()
	{
		raiz = NULL;
		totales = 0;
		habilitados = 0;
	}

	~AVL()
	{
		destruir(raiz);
	}

	void add(int id, string titulo)
	{
		raiz = insertarAux(raiz, id, titulo);
	}

	void find(int id)
	{
		findAux(raiz, id);
	}

	void toggle(int id)
	{
		toggleAux(raiz, id);
	}

	void count()
	{
		cout << totales << " " << habilitados << " " << totales - habilitados << endl;
	}
};