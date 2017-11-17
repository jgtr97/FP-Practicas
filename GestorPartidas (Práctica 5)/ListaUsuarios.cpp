/*
·Juan Gomez Tarraga
*/

#include "ListaUsuarios.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool cargar(tListaUsuarios & usuarios, ifstream & archivo) {
	/*Carga la lista de usuarios desde el flujo archivo*/
	
	bool cargado = true;

	if (archivo.is_open()) {
		// Crea array dinamico para lista de usuarios.
		crearListaUsu(usuarios);

		archivo >> usuarios.numUsuarios;

		// Aumenta la lista para que puedan entrar todos los usuarios.
		while (usuarios.numUsuarios > usuarios.dimLista) 
			redimensionarListaUsu(usuarios);

		for (int i = 0; i < usuarios.numUsuarios; i++) 
			cargado = cargar(usuarios.usuario[i], archivo);
	}
	else 
		cargado = false;
	
	return cargado;
}

void guardar(const tListaUsuarios & usuarios, ofstream & archivo) {
	/*Guarda la lista de usuarios en el flujo(ya abierto) archivo.*/
	
	archivo << usuarios.numUsuarios << endl;

	for (int i = 0; i < usuarios.numUsuarios; i++) 
		guardar(usuarios.usuario[i], archivo);

}

bool buscar(const tListaUsuarios & usuarios, const string & idUser, int ini, int fin, int &pos) {
	/*Dado un identificador de usuario y la lista, devuelve, si dicho identificador existe en la lista,
	su  posición y el valor true, y si no existe en la lista, la  posición que le
	correspondería y el valor false.*/
	
	int mitad;
	bool encontrado;

	if (ini <= fin) {
		mitad = (ini + fin) / 2;

		if (usuarios.usuario[mitad].nombreJugador < idUser)
			encontrado = buscar(usuarios, idUser, mitad + 1, fin, pos);
		else if (usuarios.usuario[mitad].nombreJugador > idUser)
			encontrado = buscar(usuarios, idUser, ini, mitad - 1, pos);
		else if (usuarios.usuario[mitad].nombreJugador == idUser) {
			encontrado = true;
			pos = mitad;
		}
	}

	else {
		pos = ini;
		encontrado = false;
	}

	return encontrado;
}

void insertar(tListaUsuarios & usuarios, string  idUsu, int &pos) {
	/*Añade un usuario en la posición de la lista que le corresponde, si hay sitio para ello y no hay otro
	usuario con el mismo identificador.*/
	
	bool buscado = false;
	int i = 0;
	
	while (i < usuarios.numUsuarios && !buscado) {
		if (idUsu > usuarios.usuario[i].nombreJugador) 
			i++;
		else
			buscado = true;
	}
	
	usuarios.numUsuarios++;

	if (usuarios.numUsuarios > usuarios.dimLista)
		redimensionarListaUsu(usuarios);

	pos = i;
	
}

bool buscarUsuarioEsperando(const tListaUsuarios & usuarios, tNivel nivel, int & pos) {
	/*Devuelve true y la posición del primer usuario del nivel dado que está esperando para iniciar una partida.
	Si no existe devuelve false.*/
	
	bool rival = false;
	pos = 0;

	while (pos < usuarios.numUsuarios && !rival) {
		if ((usuarios.usuario[pos].rival == true) && (usuarios.usuario[pos].nivel == nivel))
			rival = true;
		else 
			pos++;
	}

	return rival;
}

void crearListaUsu(tListaUsuarios & usuarios) {

	usuarios.dimLista = MAX_USUARIOS;

	usuarios.usuario = new tUsuario[usuarios.dimLista];

}

void redimensionarListaUsu(tListaUsuarios & usuarios) {

	tUsuario *swap = new tUsuario[usuarios.dimLista];

	for (int i = 0; i < usuarios.dimLista; i++) 
		swap[i] = usuarios.usuario[i];
	
	int viejaCapacidad = usuarios.dimLista;
	int nuevaCapacidad = ((viejaCapacidad * 3) / 2) + 1;
	
	usuarios.dimLista = nuevaCapacidad;

	delete[] usuarios.usuario;

	usuarios.usuario = new tUsuario[usuarios.dimLista];

	for (int i = 0; i < viejaCapacidad; i++) 
		usuarios.usuario[i] = swap[i];
	
	delete[] swap;

}
