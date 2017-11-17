/*
·Juan Gomez Tarraga
*/

#ifndef LISTA_PARTIDAS
#define LISTA_PARTIDAS

#include "Partida.h"
#include <fstream>
using namespace std;

typedef struct {
	tPartida *partida[MAX_PARTIDAS]; 
	int numPartidas; 
}tListaPartidas;

void guardar(const tListaPartidas &partidas, ofstream & archivo, ofstream & archivo2);
bool cargar(tListaPartidas & partidas, ifstream & archivo);
bool insertar(tListaPartidas &partidas, const tPartida &partida); 

#endif
