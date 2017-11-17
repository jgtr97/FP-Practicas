/*
·Juan Gomez Tarraga
*/

#include "ListaPartidas.h"
#include <iostream>
#include <fstream>

using namespace std;

void guardar(const tListaPartidas &partidas, ofstream & archivo, ofstream &archivo2) {
	/*Guarda la lista de partidas en el flujo(ya abierto) archivo*/

	for (int i = 0; i < partidas.numPartidas; i++) {
		if (partidas.partida[i]->estadoPartida == EnCurso) 
			guardar(*partidas.partida[i], archivo);
		
		else if (partidas.partida[i]->estadoPartida == Terminada)
			guardar(*partidas.partida[i], archivo2);
	}
}

bool cargar(tListaPartidas & partidas, ifstream & archivo) {
	/*Carga la lista de partidas  desde  el  flujo  (ya  abierto) archivo. Devuelve false si la lista no se ha podido
	cargar correctamente. */
	
	bool cargado = true;
	string cadena;
	
	if (archivo.is_open()) {
		for (int i = 0; i < MAX_PARTIDAS; i++)
			partidas.partida[i] = new tPartida;

		archivo >> partidas.numPartidas;
		
		for (int i = 0; i < partidas.numPartidas; i++) 
			cargar(*partidas.partida[i], archivo);
		
	}
	else 
		cargado = false; 

	return cargado;
}

bool insertar(tListaPartidas &partidas, const tPartida &partida) {
	/*Dada una partida, si hay espacio en la lista, la añade al final de la lista y devuelve true. 
	Si no se ha podido insertar devuelve false.*/

	bool insertar = true;

	if (partidas.numPartidas < MAX_PARTIDAS)
		*partidas.partida[partidas.numPartidas] = partida;
	else
		insertar = false;

	return insertar;
}
