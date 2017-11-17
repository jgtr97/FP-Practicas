/*
·Juan Gomez Tarraga
*/

#ifndef USUARIO_H
#define USUARIO_H

#include "Fecha.h"
#include <string>

using namespace std;

const int MAX_USUARIOS = 5;
const int NRN = 3;

enum tNivel {Principiante, Medio, Experto};
enum tResultado{Gana,Pierde,Empata};

typedef struct {
	string nombreJugador; 
	string contraseña; 
	tNivel nivel; 
	int racha; 
	bool rival;
	int ganadas, perdidas, empatadas;
	tFecha fecha;
	string avisos; 
}tUsuario;

void iniciar(tUsuario &usuario, const string &id, const string &pas);
void guardar(const tUsuario &usuario, ofstream &archivo);
bool cargar(tUsuario &usuario, ifstream &archivo);
bool iniciarSesion(tUsuario &usuario, const string &clave);
void aplicarFinPartida(tUsuario  &usuario, tResultado resultado);
void actualizarAvisos(tUsuario &usu, const string &aviso);
void limpiarAvisos(tUsuario & usu);

tNivel intToTNivel(int x);
int tNivelToInt(tNivel x);
bool intToBool(int x);

#endif
