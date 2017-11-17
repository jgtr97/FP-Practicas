/*
·Juan Gomez Tarraga
*/

#include "Gestor.h"
#include "checkML.h"
#include <iostream>
#include <string>

using namespace std;

void menuRegistro(tGestor &gestor);
int mostrarMenuRegistro();
int mostrarMenuUsuario(const tGestor &gestor);
int mostrarMenuPartida(bool turno);
void mostrarDatosUsu(tGestor &gestor);
void menuUsuario(tGestor &gestor);
void mostrarPartidasEnCurso(tGestor &gestor);
bool elegirPartida(tGestor &gestor);
void menuPartida(tGestor &gestor);
void leerJugada(int &col);

int main() {
	tGestor gestor;
	bool arrancado;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	arrancado = arrancar(gestor);

	if (arrancado) {
		menuRegistro(gestor);
		apagar(gestor);
	}

	system("pause");
	return 0;
}

int mostrarMenuRegistro() {
	// Muestra el menu y devuelve la eleccion del usuario.
	
	int opcion;

	do {
		cout << "MENU REGISTRO" << endl;
		cout << "    " << "1. Acceder a mi cuenta" << endl;
		cout << "    " << "2. Registrarse" << endl;
		cout << "    " << "0. Salir" << endl;
		cout << "Elija una opcion: ";
		cin >> opcion;
		cout << endl;
	} while ((opcion < 0) || (opcion > 2));

	return opcion;
}

void menuRegistro(tGestor & gestor) {
	// Muestra y gestiona el menú inicial de la aplicación:
	
	int opcion;
	string idUsu, clave;
	bool sesionIniciada, cuentaCreada, salir = false;
	
	while (!salir) {
		opcion = mostrarMenuRegistro();
		switch (opcion) {
		case 1:
			cout << "ACCESO" << endl << "    " << "Nombre de usuario: ";
			cin >> idUsu;
			cout << "    " << "Clave: ";
			cin >> clave;
			cout << endl;

			sesionIniciada = iniciarSesion(gestor, idUsu, clave);
			
			if (sesionIniciada) {
				mostrarDatosUsu(gestor);
				menuUsuario(gestor);
			}
			else
				cout << "Nombre o clave INCORRECTOS" << endl << endl;
			break;
		case 2:
			cout << "REGISTRO" << endl << "    " << "Nombre de usuario: ";
			cin >> idUsu;
			cout << "    " << "Clave: ";
			cin >> clave;
			cout << endl;
			cuentaCreada = crearCuenta(gestor, idUsu, clave);
			if (cuentaCreada) {
				mostrarDatosUsu(gestor); 
				menuUsuario(gestor);
			}
			break;
		case 0:
			cout << "Saliendo..." << endl;
			salir = true;
		}
	}

}

void mostrarDatosUsu(tGestor & gestor) {
	/*Si el usuario de la sesión tiene avisos, los muestra y pregunta si quiere eliminarlos.
	Y a continuación muestra su resumen de actividad*/
	
	string aviso, actividad;
	bool hayAvisos;
	char borrarAviso;
	
	cout << "Ha abierto correctamente una nueva sesion." << endl << endl;
	hayAvisos = tieneAvisos(gestor, aviso);
	
	if (hayAvisos) {
		cout << aviso;
		cout << "Quiere borrar los avisos (S/N)?";
		cin >> borrarAviso;
		
		if ((borrarAviso == 's') || (borrarAviso == 'S'))
			limpiarAvisos(gestor);
	}
	
	actividad = resumenActividad(gestor);
	cout << actividad;
}

int mostrarMenuUsuario(const tGestor &gestor) {
	int opcionMenu;
	
	do {
		cout << "   " << "1. Ver partida" << endl;
		
		if (!gestor.listaUsuarios.usuario[gestor.posUsuario].rival)
			cout << "   " << "2. Nueva partida" << endl;
		
		cout << "   " << "3. Partidas por actualizacion" << endl;
		cout << "   " << "4. Partidas por turno y fecha" << endl;
		cout << "   " << "0. Salir" << endl;
		cout << "Elija una opcion: ";
		cin >> opcionMenu;
		cout << endl;
	} while ((opcionMenu > 4) || (opcionMenu < 0));

	return opcionMenu;
}

void menuUsuario(tGestor & gestor) {
	/*Muestra  y  gestiona  el  menú  principal  de  una sesión de usuario*/
	
	int opcionMenu;
	bool rival;
	bool partidaElegida = true;
	bool salir = false;

	//Inicializar lista de acceso a partida.
	iniciar(gestor.listaAccesoPartidas);
	//Detecta partidas en curso y lo guarda en la lista.
	generarAccesos(gestor);
	//Ordena inicialmente por fecha.
	ordenar_Fecha(gestor);
	//Muestra las partidas en curso.
	cout << "MENU USUARIO" << endl << endl << endl;
	mostrarPartidasEnCurso(gestor);

	while (!salir) {

		opcionMenu = mostrarMenuUsuario(gestor);
		switch (opcionMenu) {
		
		case 1:
			partidaElegida = elegirPartida(gestor);
			if (partidaElegida) 
				menuPartida(gestor);
			cout << "MENU USUARIO" << endl << endl << endl;
			mostrarPartidasEnCurso(gestor);
			break;
		
		case 2:
			//Impide el acceso al usuario que este en lista de espera.
			if (!gestor.listaUsuarios.usuario[gestor.posUsuario].rival) {
				
				rival = nuevaPartida(gestor);
				
				if (rival) {
					//Vuelve a ordenar por fecha con la partida nueva.
					ordenar_Fecha(gestor);
					cout << "MENU USUARIO" << endl << endl << endl;
					mostrarPartidasEnCurso(gestor);
				}
			}
			else
				cout << "Ya estabas en lista de espera. " << endl;

			break;
		
		case 3: 
			ordenar_Fecha(gestor);
			cout << "MENU USUARIO" << endl << endl << endl;
			mostrarPartidasEnCurso(gestor);
			break;
		
		case 4:
			ordenar_Turno(gestor);
			cout << "MENU USUARIO" << endl << endl << endl;
			mostrarPartidasEnCurso(gestor);
			break;
		
		case 0:
			salir = true;
		}
	} 
}

void mostrarPartidasEnCurso(tGestor & gestor) {
	/*  Muestra  las  cabeceras de cada una de las partidas en curso del usuario de la sesión. */
	
	string datosUsu;

	cout << "#" << "  " << "T" << "  " << "Jugador1" << "            " << "Jugador2" << "            " << "Fecha" << endl;
	cout << "------------------------------------------------------------------------" << endl;

	for (int i = 0; i < gestor.listaAccesoPartidas.numPartidasUsu; i++) {
		gestor.ptrPartida = gestor.listaAccesoPartidas.lista[i].enlace;
		datosUsu = cabecera(gestor, i);
		cout << datosUsu << endl;
	}
	cout << endl;
}

bool  elegirPartida(tGestor  &  gestor) {
	/*  Solicita un número válido de partida  (de entre la lista mostrada), y lo apunta como partida seleccionada
	del usuario de la sesión. */
	
	int opcionPartida, numPartidas;
	bool rangoPartida;

	if (gestor.listaAccesoPartidas.numPartidasUsu == 0) {
		cout << "Lo sentimos. No tienes partidas abiertas." << endl << endl;
		rangoPartida = false;
	}
	
	else {
		numPartidas = gestor.listaAccesoPartidas.numPartidasUsu + 1;
		cout << "Elija una partida (0 para salir): ";
		cin >> opcionPartida;

		if (opcionPartida > 0 && opcionPartida < numPartidas) {
			rangoPartida = true;
			apuntaPartida(gestor, opcionPartida);
		}
		else
			rangoPartida = false;
	}
	return rangoPartida;
}

int mostrarMenuPartida(bool turno) {
	int opcion;
	
	if (turno)
		do {
			cout << endl << "  " << "1. Jugar" << endl;
			cout << "  " << "2. Abandonar" << endl;
			cout << "  " << "0. Salir" << endl;
			cout << "Elija una opcion:";
			cin >> opcion;
		} while ((opcion > 2) || (opcion < 0));

	else 
		do {
			cout << endl << "  " << "2. Abandonar" << endl;
			cout << "  " << "0. Salir" << endl;
			cout << "Elija una opcion:";
			cin >> opcion;
		} while ((opcion > 2) || (opcion < 0) || (opcion == 1));
	
	return opcion;
}

void menuPartida(tGestor & gestor) {
	/* Muestra y gestiona el menú de una partida */
	
	bool turno;
	int opcion, col;

	mostrarPartida(gestor);
	turno = esSuTurno(gestor);
	opcion = mostrarMenuPartida(turno);
	
	if (opcion == 1) {
		do {
			leerJugada(col);
		} while ((col < 1) || (col > 7));

		jugarPartida(gestor, col);
		mostrarPartida(gestor);
	}
	if (opcion == 2) 
		abandonarPartida(gestor);
}

void leerJugada(int & col) {
	/*solicita y devuelve una columna válida. */
	cout << "Elija una columna (1 .. 7): ";
	cin >> col;
}

