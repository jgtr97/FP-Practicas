/*
·Juan Gomez Tarraga
*/

#include "Usuario.h"
#include <iostream>
#include <fstream>

using namespace std;

void iniciar(tUsuario & usuario, const string & id, const string & pas) {
	/*Recibe un identificador de usuario y una contraseña e inicia todos los campos del usuario*/
	
	tFecha fecha;
	fecha = fechaActual();

	usuario.nombreJugador = id;
	usuario.contraseña = pas;
	usuario.avisos = "";
	usuario.empatadas = 0;
	usuario.fecha = fecha;
	usuario.ganadas = 0;
	usuario.nivel = Principiante;
	usuario.perdidas = 0;
	usuario.racha = 0;
	usuario.rival = 0;
}

void guardar(const tUsuario & usuario, ofstream & archivo) {
	/*Dado un flujo de archivo  (ya  abierto),  escribe  en  el  flujo  los  datos  del  usuario */
	
	string centinela = "_X_X_X_";

	archivo << usuario.nombreJugador << endl << usuario.contraseña << endl << usuario.nivel
		<< " " << usuario.racha << " " << usuario.rival << endl << usuario.ganadas << " "
		<< usuario.perdidas << " " << usuario.empatadas << endl << usuario.fecha << endl
		<< usuario.avisos << endl << centinela << endl;
}

bool cargar(tUsuario & usuario, ifstream & archivo) {
	/*Dado un flujo de archivo (ya abierto), lee los datos que corresponde a un usuario en usuario.
	Importante: el fichero no contiene los datos de las partidas en curso (hay que iniciarla a la lista vacía).*/
	
	stringstream avisosTotales;
	string tmp, aviso;
	int nivel, rival;

	archivo >> usuario.nombreJugador >> usuario.contraseña >> nivel >> usuario.racha >> rival
		>> usuario.ganadas >> usuario.empatadas >> usuario.perdidas >> usuario.fecha;
	
	getline(archivo, tmp); //Salto de linea
	getline(archivo, aviso); // Coge el primer aviso
	avisosTotales << aviso << endl;

	// Si el aviso esta vacio, coge el centinela de la linea siguiente.
	if (aviso.empty()) 
		getline(archivo, tmp); 

	// Si no esta vacio, recorre los avisos hasta que haya un centinela.
	else {
		getline(archivo, aviso);

		while (aviso != "_X_X_X_") {
			avisosTotales << aviso;
			getline(archivo, aviso);
			
			if (aviso != "_X_X_X_") {
				avisosTotales << endl;
			}
		}
		usuario.avisos = avisosTotales.str();
		avisosTotales.str("");
	}

	// Conversores.
	usuario.nivel = intToTNivel(nivel);
	usuario.rival = intToBool(rival);
	
	return true;
}

bool iniciarSesion(tUsuario & usuario, const string & clave) {
	/*Recibe  una contraseña  (clave)  y  un  usuario  y  devuelve  si la  contraseña  es  correcta  o  no.  Además,  en
	caso de ser correcta, actualiza la fecha de la última conexión. */
	
	bool iniciado;
	
	if (clave == usuario.contraseña) 
		iniciado = true;
	else 
		iniciado = false;
		
	return iniciado;
}

void actualizarAvisos(tUsuario & usu, const string & aviso) {
	/*Actualiza el campo avisos añadiendo al final del string una nueva línea con el nuevo aviso.*/
	
	stringstream avisos;

	if (usu.avisos == "") 
		avisos << aviso; 
	else 
		avisos << usu.avisos << aviso;

	usu.avisos = avisos.str();
}

void aplicarFinPartida(tUsuario & usuario, tResultado resultado) {
	switch (resultado) {
	case Gana:
		usuario.ganadas++;
		if (usuario.racha > 0)
			usuario.racha++;
		else
			usuario.racha = 1;
		if (usuario.racha == NRN)
			usuario.nivel = intToTNivel(tNivelToInt(usuario.nivel) + 1);
		break;
	case Pierde:
		usuario.perdidas++;
		if (usuario.racha > 0)
			usuario.racha = -1;
		else
			usuario.racha--;
		if (usuario.racha == -NRN)
			usuario.nivel = intToTNivel(tNivelToInt(usuario.nivel)-1);
		break;
	case Empata:
		usuario.empatadas++;
		usuario.racha = 0;
		break;
	}
}

void limpiarAvisos(tUsuario & usu) {
	/*Actualiza el campo avisos dejando el string vacío.*/
	
	usu.avisos = "";
}

tNivel intToTNivel(int x) {
	return tNivel(x);
}
int tNivelToInt(tNivel x) {
	return int(x);
}
bool intToBool(int x) {
	return bool(x);
}
