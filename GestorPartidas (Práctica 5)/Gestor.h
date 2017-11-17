/*
·Juan Gomez Tarraga
*/

#ifndef GESTOR_H
#define GESTOR_H

#include "ListaAccesoPartidas.h"
#include "ListaUsuarios.h"
#include "ListaPartidas.h"

using namespace std;

typedef struct {
	tListaPartidas listaPartidas;
	tListaUsuarios listaUsuarios;
	tListaAccesoPartidas listaAccesoPartidas;
	int posUsuario; // Indice con la posicion del usuario que ha iniciado sesion.
	tPartida *ptrPartida; 
}tGestor;

/*------------------MAIN-------------------*/
bool arrancar(tGestor & gestor);
void apagar(tGestor & gestor);
void generarAccesos(tGestor & gestor);
void cerrarPartida(tGestor &gestor, int &numParAbiertas);
bool buscarUsu(const tGestor &gestor, string id, int ini, int fin, int &pos);

/*-----------------REGISTRO----------------*/
bool iniciarSesion(tGestor & gestor, const string & idUsu, const string &clave);
bool crearCuenta(tGestor & gestor, const string & idUsu, const string &clave);
bool tieneAvisos(const tGestor & gestor, string &aviso);
void limpiarAvisos(tGestor & gestor);
string resumenActividad(const tGestor  &  gestor);

/*----------------MENUUSUARIO--------------*/
void ordenar_Fecha(tGestor & gestor);
void ordenar_Turno(tGestor & gestor);
string cabecera(const tGestor & gestor, int posPartida);
bool nuevaPartida(tGestor & gestor);
void apuntaPartida(tGestor & gestor, int posParEnCurso);
bool tieneTurno(const tGestor & gestor);
void aplicarOrdenarFecha(tGestor & gestor, int pos);

/*----------------MENUPARTIDA--------------*/
void mostrarPartida(const tGestor & gestor);
bool esSuTurno(const tGestor & gestor);
void jugarPartida(tGestor & gestor, int col);
void abandonarPartida(tGestor & gestor);
int posRival(tGestor & gestor);
void finPartida(tGestor & gestor);

#endif
