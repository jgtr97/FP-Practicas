/*
·Juan Gomez Tarraga
*/

#ifndef LISTAACCESOPARTIDAS_H
#define LISTAACCESOPARTIDAS_H

#include "Partida.h"
#include <string>
using namespace std;

const int MAX_PAR_US = 3;

typedef struct {
	string identificador;
	tPartida *enlace;
}tParIdEn;

typedef struct {
	tParIdEn lista[MAX_PAR_US];
	int numPartidasUsu;
}tListaAccesoPartidas;

void iniciar(tListaAccesoPartidas &lista);
bool llena(const tListaAccesoPartidas & lista);
bool insertar(tListaAccesoPartidas &lista, tParIdEn par);
bool eliminar(tListaAccesoPartidas &lista, const string & id);
bool buscar(tListaAccesoPartidas const &lista, string const &id, int &pos);

#endif
