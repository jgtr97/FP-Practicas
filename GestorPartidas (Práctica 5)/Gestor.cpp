/*
·Juan Gomez Tarraga
*/

#include "Gestor.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

bool arrancar(tGestor & gestor) {
	/*Inicializa el gestor e intenta arrancarlo cargando la información del sistema. Para ello abre los
	archivos y carga la lista de usuarios y de partidas.*/
	
	bool cargado;
	ifstream archivo;
	string archivoUsuarios = "usuarios.txt";
	string archivoPartidas = "partidas.txt";

	// Carga los datos del usuario.
	archivo.open(archivoUsuarios);
	cargado = cargar(gestor.listaUsuarios, archivo);
	archivo.close();
	
	if (!cargado)
		cout << "No se ha cargado el archivo de usuarios" << endl;

	// Carga los datos de las partidas.
	archivo.open(archivoPartidas);
	cargado = cargar(gestor.listaPartidas, archivo);
	archivo.close();
	
	if (!cargado)
		cout << "No se ha cargado el archivo de partidas" << endl;

	return cargado;
}

void generarAccesos(tGestor & gestor) {
	/*Recorre la lista de partidas, y para las que todavía están en curso, añade el par {identificador, índice} 
	a la lista de partidas en curso de cada uno de los adversarios de la partida. Además calcula el numero de 
	partidas abiertas del usuario.*/
	
	int posEnCurso = 0;

	for (int i = 0; i < MAX_PARTIDAS; i++) {
	
		if ((gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador == gestor.listaPartidas.partida[i]->adversario1) ||
			(gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador == gestor.listaPartidas.partida[i]->adversario2)) {

			if (gestor.listaPartidas.partida[i]->estadoPartida == EnCurso) {

				// Almacena el acceso de las partidas del usuario.
				gestor.listaAccesoPartidas.lista[posEnCurso].enlace = gestor.listaPartidas.partida[i];

				gestor.listaAccesoPartidas.lista[posEnCurso].identificador = gestor.listaPartidas.partida[i]->identificador;
				
				posEnCurso++;
			}
		}
	}
	// Guarda el numero de partidas en curso del usuario.
	gestor.listaAccesoPartidas.numPartidasUsu = posEnCurso;
}

void apagar(tGestor & gestor) {
	/* Esta operación apaga el gestor, abriendo los ficheros del sistema. */
	
	ofstream archivo;
	ofstream archivo2;
	string nombreArchivoUsuarios = "usuarios.txt";
	string nombreArchivoPartidas = "partidas.txt";
	string nombrePartidasHist = "historico.txt";
	int numPartidasAbiertas = 0;

	// Guarda los datos del usuario.
	archivo.open(nombreArchivoUsuarios);
	guardar(gestor.listaUsuarios, archivo);
	archivo.close();

	// Cierra las partidas que tengan mas de un mes de actividad.
	cerrarPartida(gestor, numPartidasAbiertas);

	// Guarda los datos de las partidas.
	archivo.open(nombreArchivoPartidas);
	archivo2.open(nombrePartidasHist, ios::app);

	archivo << numPartidasAbiertas << endl;

	guardar(gestor.listaPartidas, archivo, archivo2);
	archivo.close();

	//Borra el array dinamico de usuarios.
	delete[] gestor.listaUsuarios.usuario;

	//Borra la memoria de lista de partidas.
	for (int i = 0; i < MAX_PARTIDAS; i++)
		delete gestor.listaPartidas.partida[i]; 
}

void cerrarPartida(tGestor &gestor, int &numParAbiertas) {

	tFecha fechaPar;
	tFecha mes = 2592000; // 30 dias.
	numParAbiertas = 0;
	int posUsu1;
	int posUsu2;
	string adversario1;
	string adversario2;

	for (int i = 0; i < gestor.listaPartidas.numPartidas; i++) {
		fechaPar = fechaActual() - gestor.listaPartidas.partida[i]->fechaActualizada;

		// Si la fecha de partida tiene mas de un mes:
		if (fechaPar > mes) {

			adversario1 = gestor.listaPartidas.partida[i]->adversario1;
			adversario2 = gestor.listaPartidas.partida[i]->adversario2;

			gestor.listaPartidas.partida[i]->estadoJuego.turno = Nadie;
			gestor.listaPartidas.partida[i]->estadoPartida = Terminada;

			//Buscamos la posicion de los adversarios.
			if (buscar(gestor.listaUsuarios, adversario1, 0, gestor.listaUsuarios.numUsuarios - 1, posUsu1))
				if (buscar(gestor.listaUsuarios, adversario2, 0, gestor.listaUsuarios.numUsuarios - 1, posUsu2)) {
					//Quien tenga el turno pierde la partida.
					if (gestor.listaPartidas.partida[i]->estadoJuego.turno == Jugador1) {
						//Jugador1 pierde.
						aplicarFinPartida(gestor.listaUsuarios.usuario[posUsu1], Pierde);
						//Jugador2 gana.
						aplicarFinPartida(gestor.listaUsuarios.usuario[posUsu2], Gana);
					}
					else {
						//Jugador1 gana.
						aplicarFinPartida(gestor.listaUsuarios.usuario[posUsu1], Gana);
						//Jugador2 pierde.
						aplicarFinPartida(gestor.listaUsuarios.usuario[posUsu2], Pierde);
					}
				}
				else
					cout << "Se ha producido un error interno al cerrar una partida: No existe el jugador " << adversario2
					<< " en la lista de usuarios. " << endl;
			else
				cout << "Se ha producido un error interno al cerrar una partida: No existe el jugador " << adversario1
				<< " en la lista de usuarios. " << endl;
		}
			
		else if (fechaPar < mes && gestor.listaPartidas.partida[i]->estadoPartida == Jugando)
			numParAbiertas++;
	}
}

bool iniciarSesion(tGestor & gestor, const string & idUsu, const string &clave) {
	/*Recibe los datos de usuario necesarios para validar la cuenta (id y contraseña) y comprueba  si  el  usuario existe
	 y la contraseña coincide. Devuelve cierto y actualiza  el usuario de la sesión si la operación tuvo éxito.*/
	
	bool buscado, iniciado;
	int pos = 0;
	tFecha fecha;

	// Primero busca el usuario dentro de la lista de usuarios.
	buscado = buscar(gestor.listaUsuarios, idUsu, 0, gestor.listaUsuarios.numUsuarios-1, pos);

	if (buscado) {
		gestor.posUsuario = pos;
		
		// Inicia sesion comparando la clave introducia con la de la posicion.
		iniciado = iniciarSesion(gestor.listaUsuarios.usuario[pos], clave);
		
		// Actualiza la fecha de inicio.
		fecha = fechaActual();
		
		gestor.listaUsuarios.usuario[gestor.posUsuario].fecha = fecha;
	}
	else 
		iniciado = false;
	
	return iniciado;
}

bool crearCuenta(tGestor & gestor, const string & idUsu, const string &clave) {
	/*Recibe los datos de usuario necesarios para crear una cuenta (id y contraseña) y si el id de usuario no existe y
	hay espacio en la lista de usuarios, crea la cuenta del usuario. Devuelve cierto y actualiza el usuario de la sesión
	si la operación tuvo éxito. */
	
	bool cuentaCreada = true;
	bool busqueda = false;
	int i = 0;

	// Comprueba que no exista el usuario.
	while (!busqueda && i < gestor.listaUsuarios.numUsuarios) {
		if (idUsu == gestor.listaUsuarios.usuario[i].nombreJugador) {
			busqueda = true;
			cout << "ERROR: ese nombre ya existe " << endl << endl;
		}
		else 
			i++;
	}

	if (!busqueda) 
		insertar(gestor.listaUsuarios, idUsu, gestor.posUsuario);
	
	else 
		cuentaCreada = false;
	
	if (cuentaCreada) {
		
		// Desplazar datos usuarios.
		for (int i = gestor.listaUsuarios.numUsuarios-1; i > gestor.posUsuario; i--) 
			gestor.listaUsuarios.usuario[i] = gestor.listaUsuarios.usuario[i-1];

		// Inicializar cuenta del usuario nuevo.
		iniciar(gestor.listaUsuarios.usuario[gestor.posUsuario], idUsu, clave);
	}

	return cuentaCreada;
}

bool tieneAvisos(const tGestor & gestor, string &aviso) {
	/*Comprueba  si  el usuario de la sesión tiene avisos, y en caso afirmativo los devuelve en aviso.*/
	
	stringstream avisoUsu;
	bool hayAviso = true;

	if (gestor.listaUsuarios.usuario[gestor.posUsuario].avisos.empty()) 
		hayAviso = false;
	
	else {
		avisoUsu << "Tiene avisos:" << endl;
		avisoUsu << gestor.listaUsuarios.usuario[gestor.posUsuario].avisos << endl << endl;
	}

	aviso = avisoUsu.str();

	return hayAviso;
}

void limpiarAvisos(tGestor & gestor) {
	/*Actualiza los avisos del usuario de la sesión a la cadena vacía. */
	
	limpiarAvisos(gestor.listaUsuarios.usuario[gestor.posUsuario]);
}

string resumenActividad(const  tGestor  &  gestor) {
	/* Devuelve  un  string  con  los datos de la actividad del usuario de la sesión,las partidas ganadas, perdidas y
	 empatadas, el nivel, la racha y si está esperando para una nueva partida */
	
	stringstream actividad;
	unsigned int rachaUsu;
	
	actividad << "Resumen de actividad" << endl;
	actividad << "  " << "Partidas ganadas: " << gestor.listaUsuarios.usuario[gestor.posUsuario].ganadas << endl;
	actividad << "  " << "Partidas perdidas: " << gestor.listaUsuarios.usuario[gestor.posUsuario].perdidas << endl;
	actividad << "  " << "Partidas empatadas: " << gestor.listaUsuarios.usuario[gestor.posUsuario].empatadas << endl;
	
	switch (gestor.listaUsuarios.usuario[gestor.posUsuario].nivel) {
	case 0:
		actividad << "  " << "Nivel actual: Principiante" << endl;
		break;
	case 1: 
		actividad << "  " << "Nivel actual: Medio" << endl;
		break;
	case 2:
		actividad << "  " << "Nivel actual: Experto" << endl;
	}

	if (gestor.listaUsuarios.usuario[gestor.posUsuario].racha > 0)
		actividad << "  " << "Racha actual: " << gestor.listaUsuarios.usuario[gestor.posUsuario].racha << " victorias." << endl << endl;
	
	else if (gestor.listaUsuarios.usuario[gestor.posUsuario].racha < 0){
		// Elimina signo.
		rachaUsu = abs(gestor.listaUsuarios.usuario[gestor.posUsuario].racha);
		actividad << "  " << "Racha actual: " << rachaUsu << " derrotas" << endl << endl;
	}
	return actividad.str();
}

//********************** MENU USUARIO MAIN ************************************************
bool tieneTurno(const tGestor & gestor) {
	bool turno;

	if ((gestor.ptrPartida->estadoJuego.turno == Jugador1) &&
		(gestor.ptrPartida->adversario1 == gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador))

		turno = true;

	else if ((gestor.ptrPartida->estadoJuego.turno == Jugador2) &&
		(gestor.ptrPartida->adversario2 == gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador))

		turno = true;

	else
		turno = false;

	return turno;
}

void ordenar_Fecha(tGestor & gestor) {
	/* Ordena por el metodo de la burbuja la fecha de la última actualización de la lista de 
	partidas en curso del usuario de la sesión. */
	
	for (int i = 0; i < gestor.listaAccesoPartidas.numPartidasUsu - 1; i++) 
		for (int j = gestor.listaAccesoPartidas.numPartidasUsu - 1; j > i; j--)
			aplicarOrdenarFecha(gestor, j);
}

void aplicarOrdenarFecha(tGestor & gestor, int pos) {
	
	tParIdEn tmp;

	if (gestor.listaAccesoPartidas.lista[pos].enlace->fechaActualizada > gestor.listaAccesoPartidas.lista[pos - 1].enlace->fechaActualizada) {

		tmp = gestor.listaAccesoPartidas.lista[pos];

		gestor.listaAccesoPartidas.lista[pos] = gestor.listaAccesoPartidas.lista[pos - 1];

		gestor.listaAccesoPartidas.lista[pos - 1] = tmp;
	}
}

void ordenar_Turno(tGestor & gestor) {
	/*  Ordena por  turno  (primero  aquellas  en  las que le toca jugar al usuario de la sesión) la lista  de  partidas
	en curso del  usuariO de la sesión. Utiliza la ordenación por inserción. */
	
	int j;
	tParIdEn tmp;
	bool turno, turnoAnt;

	for (int i = 1; i < gestor.listaAccesoPartidas.numPartidasUsu; i++) {
		tmp = gestor.listaAccesoPartidas.lista[i];
		j = i;
		
		gestor.ptrPartida = gestor.listaAccesoPartidas.lista[j].enlace;
		turno = tieneTurno(gestor);
		
		gestor.ptrPartida = gestor.listaAccesoPartidas.lista[j-1].enlace;
		turnoAnt = tieneTurno(gestor);

		if ((!turno) && (!turnoAnt))
			aplicarOrdenarFecha(gestor, j);

		while ((j>0) && (turno) && (!turnoAnt)) {
			gestor.listaAccesoPartidas.lista[j] = gestor.listaAccesoPartidas.lista[j-1];
			gestor.ptrPartida = gestor.listaAccesoPartidas.lista[j - 1].enlace;
			turnoAnt = tieneTurno(gestor);
			j--;
		}
		gestor.listaAccesoPartidas.lista[j] = tmp;
	}
}

string cabecera(const tGestor & gestor, int posPartida) {
	/*Dada  una  posición de la lista de partidas en curso del usuario de la sesión, devuelve un string que contiene
	la información que se mostrará en la lista de partidas en curso: marca ('*') si tiene el turno, nombres de los jugadores
	(jugador1,  jugador2) y  fecha  de  la  última  actualización */
	
	stringstream datosUsu;
	string printDatos, fecha;
	bool hora = true;
	bool turno;

	fecha = stringFecha(gestor.ptrPartida->fechaActualizada, hora);
	turno = tieneTurno(gestor);

	datosUsu << posPartida + 1 << "  ";
	
	if (turno)
		datosUsu << "*";
	else
		datosUsu << " ";

	datosUsu << "  " << setw(20) << left << gestor.ptrPartida->adversario1 
		<< setw(20) << gestor.ptrPartida->adversario2
		<< setw(20) << fecha;
	
	printDatos = datosUsu.str();

	return printDatos;
}

bool nuevaPartida(tGestor & gestor) {
	/* Si el usuario de la sesión no ha alcanzado el máximo  número  de  partidas  en  curso  y  no  está ya en espera,
	busca un usuario que esté esperando para una nueva partida, que sea del mismo nivel que el usuario de la sesión. Si
	lo encuentra crea la nueva partida e inserta en ambos adversarios los datos de acceso. En otro caso deja al usuario
	de la sesión en estado de espera para una nueva partida.*/

	bool rival;
	bool encuentraRival = false;
	bool insertarPartida;
	bool nuevaPar;
	string adversario;
	int posRival;
	tPartida partida;
	tParIdEn parIdEn;

	if (gestor.listaPartidas.numPartidas < MAX_PARTIDAS) {

		if (gestor.listaAccesoPartidas.numPartidasUsu < MAX_PAR_US) {

			if (!gestor.listaUsuarios.usuario[gestor.posUsuario].rival) {

				rival = buscarUsuarioEsperando(gestor.listaUsuarios, gestor.listaUsuarios.usuario[gestor.posUsuario].nivel, posRival);

				if (rival) {

					adversario = gestor.listaUsuarios.usuario[posRival].nombreJugador;

					nueva(partida, gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador, adversario);

					insertarPartida = insertar(gestor.listaPartidas, partida);

					if (insertarPartida) {

						gestor.listaPartidas.numPartidas++;

						//Actualiza las partidas del usuario.
						generarAccesos(gestor);

						// Insertamos la lista de acceso.

						parIdEn.enlace = gestor.listaPartidas.partida[gestor.listaPartidas.numPartidas - 1];
						parIdEn.identificador = parIdEn.enlace->identificador;

						// Modo espera en usuario -> false
						gestor.listaUsuarios.usuario[gestor.posUsuario].rival = false;

						nuevaPar = insertar(gestor.listaAccesoPartidas, parIdEn);

						// Quitamos al adversario del modo de espera
						gestor.listaUsuarios.usuario[posRival].rival = false;
						encuentraRival = true;
					}
				}
				else {
					gestor.listaUsuarios.usuario[gestor.posUsuario].rival = true;
					cout << "Quedas en espera de adversario" << endl << endl;
				}
			}
			else
				cout << "Ya estas en lista de espera." << endl << endl;
		}
		else
			cout << "Has alcanzado el maximo de partidas abiertas." << endl << endl;
	}
	else
		cout << "Has alcanzado el limite de partidas totales" << endl << endl;

	return encuentraRival;
}

void apuntaPartida(tGestor & gestor, int posParEnCurso) {
	/* Dada  una  posición de la lista de partidas en curso del usuario de la sesión, guarda en partida seleccionada, el
	índice de dicha partida en la lista de partidas.*/
	
	gestor.ptrPartida = gestor.listaAccesoPartidas.lista[posParEnCurso - 1].enlace;
}

//************************** MENU PARTIDA DEL MAIN **************************************************

void mostrarPartida(const tGestor & gestor) {
	/* Muestra el estado de la partida en curso*/
	
	mostrar(gestor.ptrPartida->estadoJuego,
		gestor.ptrPartida->adversario1,
		gestor.ptrPartida->adversario2);
}

bool esSuTurno(const tGestor & gestor) {
	/* Comprueba si es el turno del usuario de la sesión en la partida seleccionada. */
	
	bool turno;
	
	if ((gestor.ptrPartida->estadoJuego.turno == Jugador1) &&
		(gestor.ptrPartida->adversario1 == gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador))
		turno = true;
	
	else if ((gestor.ptrPartida->estadoJuego.turno == Jugador2) &&
		(gestor.ptrPartida->adversario2 == gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador))
		turno = true;
	
	else
		turno = false;

	return turno;
}

void jugarPartida(tGestor & gestor, int col) {
	/*Aplica la jugada col a la partida, y en caso de que la partida termine, actualiza a los adversarios, según termine el
	juego con Ganador o Bloqueo. Además  envía  un  aviso  al  contrincante  indicando  que  ha  perdido  o empatado y la
	fecha */
	
	int rival;
	stringstream avisos;
	tFecha fecha;
	bool jugadaAplicada;

	fecha = fechaActual();
	jugadaAplicada = aplicar_Jugada(*gestor.ptrPartida, col);

	if (jugadaAplicada) {

		rival = posRival(gestor);

		switch (gestor.ptrPartida->estadoJuego.estado) {
		
		case Jugando:
			// Intercambiar turno.
			if (gestor.ptrPartida->adversario1 == gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador)
				gestor.ptrPartida->estadoJuego.turno = Jugador2;
			else
				gestor.ptrPartida->estadoJuego.turno = Jugador1;

			gestor.ptrPartida->fechaActualizada = fecha;
			break;
		
		case Ganador:
			//Actualiza los avisos.
			avisos << "Ha perdido una partida contra " << gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador << " el " << stringFecha(fecha, false) << ".";
			actualizarAvisos(gestor.listaUsuarios.usuario[rival], avisos.str());
			avisos.str("");
			//Aplica fin partida.
			finPartida(gestor);
			break;
		
		case Bloqueo:
			//Actualizar los avisos.
			avisos << "Ha empatado una partida contra " << gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador << " el " << stringFecha(fecha, false) << ".";
			actualizarAvisos(gestor.listaUsuarios.usuario[rival], avisos.str());
			avisos.str("");
			//Aplica fin partida.
			finPartida(gestor);
			break;
		}
	}
}

void  abandonarPartida(tGestor  &  gestor) {
	/*Abandona  la  partida  seleccionada  del usuario, y actualiza la información de los adversarios teniendo en cuenta que
	el  que abandona  pierde y el otro gana. Además avisa al contrincante indicando que ha ganado con la fecha*/
	
	int rival;
	stringstream avisos;
	tFecha fecha;
	tResultado;
	bool eliminado;
	
	fecha = fechaActual();
	abandonar(*gestor.ptrPartida);

	rival = posRival(gestor);

	//Eliminar de la lista de accesos la partida terminada.
	eliminado = eliminar(gestor.listaAccesoPartidas,
		gestor.ptrPartida->identificador);

	if (eliminado)
		//Restar partida en curso.
		gestor.listaAccesoPartidas.numPartidasUsu--;
	
	gestor.listaUsuarios.usuario[gestor.posUsuario].perdidas++;
	gestor.listaUsuarios.usuario[rival].ganadas++;

	//Actualizar avisos.
	avisos << "Ha ganado por abandono una partida contra " << gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador << " el " 
		<< stringFecha(fecha, false) << ".";
	
	actualizarAvisos(gestor.listaUsuarios.usuario[rival], avisos.str());

	// Rachas Usuario
	if (gestor.listaUsuarios.usuario[gestor.posUsuario].racha > 0)
		gestor.listaUsuarios.usuario[gestor.posUsuario].racha = -1;
	else
		gestor.listaUsuarios.usuario[gestor.posUsuario].racha--;
	
	// Rachas rival
	if (gestor.listaUsuarios.usuario[rival].racha > 0)
		gestor.listaUsuarios.usuario[rival].racha++;
	else
		gestor.listaUsuarios.usuario[rival].racha = 1;
}

int posRival(tGestor & gestor) {
	// Devuelve la posicion del rival.
	
	int i = 0;
	bool buscar = false;

	if (gestor.ptrPartida->adversario1 == gestor.listaUsuarios.usuario[gestor.posUsuario].nombreJugador)
		while (!buscar) {
			if (gestor.ptrPartida->adversario2 == gestor.listaUsuarios.usuario[i].nombreJugador)
				buscar = true;
			else {
				buscar = false;
				i++;
			}
		}
	else
		while (!buscar) {
			if (gestor.ptrPartida->adversario1 == gestor.listaUsuarios.usuario[i].nombreJugador)
				buscar = true;
			else {
				buscar = false;
				i++;
			}
		}
	return i;
}

void finPartida(tGestor & gestor) {
	/*Aplica el final de partida actualizando los datos del usuario.*/
	
	tResultado resultado;
	bool eliminado;
	int rival;

	//Devuelve posicion rival.
	rival = posRival(gestor);
	//Fecha de la partida finalizada.
	gestor.ptrPartida->fechaActualizada = fechaActual();

	//Eliminar de la lista de accesos la partida terminada.
	eliminado = eliminar(gestor.listaAccesoPartidas,
		gestor.ptrPartida->identificador);

	if (eliminado)
		//Restar partida en curso.
		gestor.listaAccesoPartidas.numPartidasUsu--;

	switch (gestor.ptrPartida->estadoJuego.estado) {
	
	case Ganador:
		gestor.ptrPartida->estadoJuego.turno = Nadie;
		gestor.ptrPartida->estadoPartida = Terminada;

		//Para el jugador que gana.
		resultado = Gana;
		aplicarFinPartida(gestor.listaUsuarios.usuario[gestor.posUsuario], resultado);

		//Para el jugador que pierde.
		resultado = Pierde;
		aplicarFinPartida(gestor.listaUsuarios.usuario[rival], resultado);
		break;
	
	case Bloqueo:
		gestor.ptrPartida->estadoJuego.turno = Nadie;
		gestor.ptrPartida->estadoPartida = Terminada;

		// Para el jugador usuario.
		resultado = Empata;
		aplicarFinPartida(gestor.listaUsuarios.usuario[gestor.posUsuario], resultado);
		//Para el jugador rival.
		aplicarFinPartida(gestor.listaUsuarios.usuario[rival], resultado);
		break;
	}
}
