/*
·Juan Gomez Tarraga
*/

#include "Partida.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void nueva(tPartida & partida, const string & jugador, const string & adversario) {
	/* Recibe  los  identificadores  de  los  adversarios  y  devuelve  una  partida  con todos sus datos rellenos.*/
	
	stringstream identificador;
	tFecha fecha;

	fecha = fechaActual();

	identificador << fecha << "_" << jugador << "_" << adversario;
	
	partida.identificador = identificador.str();
	partida.adversario1 = jugador;
	partida.adversario2 = adversario;
	partida.fechaInicio = fecha;
	partida.fechaActualizada = fecha;
	partida.estadoPartida = EnCurso; 
	iniciar(partida.estadoJuego);
}

bool aplicar_Jugada(tPartida & partida, int col) {
	/*Aplica la jugada col al juego, y si se pudo realizar, devuelve true y actualiza los demás campos de la partida. */
	
	bool aplicada;

	aplicada = aplicarJugada(partida.estadoJuego, col - 1);

	return aplicada;
}

void abandonar(tPartida & partida) {
	/*Modifica el estado de la partida a Terminada, y la fecha de la última actualización. */

	partida.estadoPartida = Terminada;
	partida.estadoJuego.estado = Ganador;
	partida.fechaActualizada = fechaActual();
}

void guardar(const tPartida & partida, ofstream & archivo) {
	/*Dado un flujo de archivo  de  salida  (ya  abierto),  escribe  en  el flujo  los  datos  de  la  partida. */
	
	archivo << partida.identificador << endl << partida.adversario1 << endl
		<< partida.adversario2 << endl << partida.fechaInicio << " " << partida.fechaActualizada
		<< endl << partida.estadoPartida << endl;
	
	guardar(partida.estadoJuego, archivo);
}

void cargar(tPartida & partida, ifstream & archivo) {
	/*Dado un flujo de archivo de  entrada(ya  abierto), lee  los  datos  que  corresponden  a  una  partida  en
	partida.*/
	
	int estadoPartida;

	archivo >> partida.identificador >> partida.adversario1 >> partida.adversario2 >> partida.fechaInicio 
		>> partida.fechaActualizada >> estadoPartida;
		
	cargar(partida.estadoJuego , archivo);
	
	partida.estadoPartida = intToTestadoPartida(estadoPartida); // Conversor
}

void cerrarPartida(tPartida &partida) {

	partida.estadoPartida = Terminada;


}

tEstadoPartida intToTestadoPartida(int a) {
	return tEstadoPartida(a);
}

