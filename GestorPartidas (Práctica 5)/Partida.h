/*
·Juan Gomez Tarraga
*/

#ifndef PARTIDA_H
#define PARTIDA_H

#include "Conecta4.h"
#include <string>
using namespace std;

const int MAX_PARTIDAS = 5;

typedef time_t tFecha;
enum tEstadoPartida {EnCurso, Terminada};

typedef struct {
	string identificador;
	string adversario1;
	string adversario2;
	tFecha fechaInicio;
	tFecha fechaActualizada;
	tEstadoPartida estadoPartida;
	tConecta4 estadoJuego; 
}tPartida;

void nueva(tPartida & partida, const string & jugador, const string & adversario);
bool aplicar_Jugada(tPartida & partida, int col);
void abandonar(tPartida & partida);
void guardar(const tPartida & partida, ofstream & archivo);
void cargar(tPartida & partida, ifstream & archivo);
void cerrarPartida(tPartida &partida);

tEstadoPartida intToTestadoPartida(int a);

#endif
