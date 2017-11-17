/*
·Juan Gomez Tarraga
*/

#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include "Usuario.h"
#include <string>

using namespace std;

typedef struct {
	tUsuario *usuario;
	int numUsuarios;
	int dimLista;
}tListaUsuarios;

bool cargar(tListaUsuarios & usuarios, ifstream & archivo);
void guardar(const tListaUsuarios & usuarios, ofstream & archivo);
bool buscar(const tListaUsuarios & usuarios, const string & idUser, int ini, int fin, int &pos);
void insertar(tListaUsuarios & usuarios, string idUsu, int &pos);
bool buscarUsuarioEsperando(const tListaUsuarios & usuarios, tNivel nivel, int & pos);

void crearListaUsu(tListaUsuarios & usuarios);
void redimensionarListaUsu(tListaUsuarios & usuarios);

#endif
