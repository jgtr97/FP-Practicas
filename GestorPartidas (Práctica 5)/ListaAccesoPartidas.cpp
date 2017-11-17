/*
·Juan Gomez Tarraga
*/

#include "ListaAccesoPartidas.h"
#include <iostream>

using namespace std;

void iniciar(tListaAccesoPartidas & lista) {
	/*Inicia la lista vacia*/
	
	for (int i = 0; i < MAX_PAR_US; i++) {
		lista.lista[i].enlace = 0;
		lista.lista[i].identificador.empty();
	}
}

bool llena(const tListaAccesoPartidas & lista) {
	//Comprueba si la lista está llena.
	
	bool llena;
	
	if (lista.numPartidasUsu < MAX_PAR_US) 
		llena = false;
	else 
		llena = true;

	return llena;
}

bool insertar(tListaAccesoPartidas & lista, tParIdEn par) {
	/*Dado un elemento, si la lista no está llena lo inserta al final de la lista y devuelve true. En otro caso
	devuelve false.*/
	
	bool listaLlena;
	bool insertar = false;

	listaLlena = llena(lista);

	if (!listaLlena) {
		insertar = true;
		//Añade a la lista el acceso de la nueva partida para el usuario.
		lista.lista[lista.numPartidasUsu] = par;
	}
	return insertar;
}

bool eliminar(tListaAccesoPartidas & lista, const string & id) {
	/*Dado un identificador de partida, lo busca en la lista y  si lo encuentra lo elimina de la lista.
	Si no lo encuentra, devuelve false, en otro caso devuelve true. */
	
	bool encontrado = false;
	bool eliminar = false;
	int pos = 0;

	encontrado = buscar(lista, id, pos);

	if (encontrado) {
		for (int z = pos+1; z < MAX_PAR_US; z++) 
			lista.lista[z-1] = lista.lista[z];
		eliminar = true;
	}

	return eliminar;
}

bool buscar(tListaAccesoPartidas const &lista, string const &id, int &pos) {
	// Busca la posicion del acceso a una partida.

	bool encontrado;

	if (pos < lista.numPartidasUsu) {

		if (lista.lista[pos].identificador == id)
			encontrado = true;
		else {
			pos++;
			encontrado = buscar(lista, id, pos);
		}
	}
	else
		encontrado = false;

	return encontrado;
}
