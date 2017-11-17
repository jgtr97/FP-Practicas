/*
# |----------------------|
# | Juan Gomez Tarraga   |
# |Fernando Quijada Diaz |
# |----------------------|
*/

#include "Unosolo.h"
using namespace std;

void colorFondo(int color) { 
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

bool cargar(tJuego &juego, string nombre) {
	//Carga el juego.
	
	ifstream fichero;
	bool carga = true;
	int dimTxt;
	int casilla;
	fichero.open(nombre);

	if (fichero.is_open()) {
		fichero >> dimTxt;
		
		//Tiene que coincidir la dimension en archivo y programa.
		if (dimTxt == DIM) { 
			fichero >> juego.bolasIniciales;
			fichero >> juego.filaMeta;
			fichero >> juego.colMeta;
			
			//Carga el tablero.
			for (int fila = 0; fila < DIM; fila++) 
				for (int col = 0; col < DIM; col++) {
					fichero >> casilla;
					juego.tablero[fila][col] = intToTCelda(casilla);
				}
            fichero >> juego.numMovimientos;
		}
		else 
			carga = false;
	}
	else 
		carga = false;
	
	fichero.close();
	
	return carga;
}

void guardar(const tJuego &juego, string nombre) {
	//Guarda el juego.
	
	ofstream fichero;
	fichero.open(nombre);

	fichero << DIM << endl;
	fichero << juego.bolasIniciales << endl;
	fichero << juego.filaMeta << endl;
	fichero << juego.colMeta << endl;

	for (int fila = 0; fila < DIM; fila++) {
		for (int col = 0; col < DIM; col++) 
			fichero << juego.tablero[fila][col] << char(9);
		fichero << endl;
	}
	fichero << juego.numMovimientos << endl;
	fichero.close();
}

tCelda intToTCelda(int x) {
	return tCelda(x);
}

bool menuPrincipal(){
	// Menu principal del programa. 
	
	int eleccion;
	bool jugar;

	cout << "**** UNO SOLO ****" << endl << endl;

	do {
		cout << "0 - Salir" << endl;
		cout << "1 - Empezar una partida nueva" << endl;
		cin >> eleccion;
	} while (eleccion != 0 && eleccion != 1);

    if (eleccion == 0)
        jugar = false;
    else 
        jugar = true;
  
    return jugar;
}

int menuPartida() {
    // Menu de una partida, devuelve la opcion del usuario.

	int opcion;
    
	do {
        cout << "1 - Escoger tablero" << endl;
        cout << "2 - Generacion aleatoria de tablero" << endl;
        cin >> opcion;
    } while (opcion != 1 && opcion !=2);
  
	return opcion;
}

void partida(tJuego &juego){
	// Controla la partida.
	
	tMovimiento mov;
	bool leer; 
	bool deshacer;

	juego.posMov = 0; // Posicion en la lista de movimientos.

	do {
		cout << char(9) << "Movimientos: " << juego.numMovimientos << endl;

		mostrar(juego); // Mostrar tablero.
		leer = leerMovimiento(juego, mov, deshacer);
		
		if (leer) { // Si el movimiento se ha leido correctamente.
		
			if (!deshacer) {
				ejecutarMovimiento(juego, mov);
				juego.numMovimientos++; //Aumento el numero de movimientos.
				juego.posMov++; // Aumento la posicion en la lista.
				calcularEstado(juego); // Comprueba el estado.
				
				// Guarda los movimientos en una lista.
				escribirListaMovimientos(juego, mov, juego.numMovimientos-1);
			}
			
			else {
				deshacerMovimientos(juego);
				calcularEstado(juego); 
			}
		}
		
		else //El usuario ha decidido salir.
			juego.estado = ABANDONO;

		if (juego.estado == BLOQUEO) {
			mostrar(juego);
			cout << "Ha perdido. No hay mas movimientos posibles." << endl;
		}
		
		else if (juego.estado == GANA) {
			mostrar(juego);
			cout << "Enhorabuena, has ganado la partida!" << endl;
		}

    } while (juego.estado == JUGANDO);
}

void mostrar(const tJuego &juego) {
	// Muestra por pantalla el tablero.

	cout << char(9);

	// Numera las columnas.
	for (int i = 0; i < DIM; i++) {
		if (i < DIM) 
			cout << i+1 << setw(4);
		else 
			cout << i+1;
	}
	cout << endl;

	//Numera las filas y muestra el tablero por pantalla.
	for (int fila = 0; fila < DIM; fila++) {

		cout << setw(6) << fila+1 << " ";

		for (int col = 0; col < DIM; col++) {
			colorFondo(juego.tablero[fila][col]); // Para mostar un color dependiendo del valor de la casilla.
			if (juego.filaMeta == fila && juego.colMeta == col) {
				cout << " " << char(254); // Mostrar meta.
				colorFondo(0); // Fondo negro de separacion entre fichas.
				cout << "  ";
			}
			else {
				cout << "  ";
				colorFondo(0); // Fondo negro de separacion entre fichas.
				cout << "  ";
			}
		}
		cout << endl << endl; // Hago dos saltos de linea para pasar a la siguiente fila.
	}
}

bool selecFicha(const tJuego &juego, tMovimiento &mov, bool &valida, bool &deshacer) {
	// Pregunta al usuario la ficha a seleccionar, o si quiere deshacer movimientos.
	
	bool salir = false;
	
	cout << "FICHA (fila y columna), 0 para salir, o -1 para deshacer: ";
	cin >> mov.fila;
	
	if (mov.fila != 0 && mov.fila != -1) {
		cin >> mov.col;
		
		//Restar 1 fila y col para seleccionar correctamente la ficha en programa.
		mov.fila = mov.fila - 1;
		mov.col = mov.col - 1;
	
		if (juego.tablero[mov.fila][mov.col] == FICHA)
			valida = true;
		else
			valida = false;
		deshacer = false;
	}
	
	else if (mov.fila == -1)
		deshacer = true;
	else
		salir = true;

	return salir;
}

bool leerMovimiento(const tJuego juego, tMovimiento &mov, bool &deshacer){
	tPosibles posible;
	int opcion;
	bool salir = false;
	bool puedeMover;
	bool valida = true;
	bool leer = false;

	do {
		// Se selecciona una ficha, se sale o se deshace movimientos.
		salir = selecFicha(juego, mov, valida, deshacer); 
		if (!salir && !deshacer) {
			// Si la casilla seleccionada es una ficha.
			if (valida) { 
				puedeMover = posiblesMovimientos(juego, mov, posible);
				//La ficha tiene al menos un movimiento disponible.
				if (puedeMover) { 
					opcion = mostrarMovimiento(posible);
					if (opcion != 0) {
						// Si todo va bien, se habra leido correctamente el movimiento.
						leer = elegirMovimiento(mov, posible, opcion);
						// La direccion escogida no existe por las siguientes razones:
						if (!leer) { 
							if (opcion == 1)
								cout << "No se puede mover arriba." << endl;
							else if (opcion == 2)
								cout << "No se puede mover abajo." << endl;
							else if (opcion == 3)
								cout << "No se puede mover a la izquierda." << endl;
							else if (opcion == 4)
								cout << "No se puede mover a la derecha." << endl;
						}
					}
				}
				// La ficha no se puede mover.
				else 
					cout << "La ficha seleccionada no se puede mover. Intentalo de nuevo" << endl;
			}
			// Si la casilla seleccionada no es ficha, quedan tres opciones:
			else { 
				if (mov.fila < 0 || mov.fila > DIM - 1 || mov.col < 0 || mov.col > DIM - 1)
					cout << "La casilla seleccionada esta fuera de rango. Intentalo de nuevo" << endl;
				else if (juego.tablero[mov.fila][mov.col] == NULA)
					cout << "La casilla seleccionada es nula. Intentalo de nuevo." << endl;
				else if (juego.tablero[mov.fila][mov.col] == VACIA)
					cout << "La casilla seleccionada esta vacia. Intentalo de nuevo" << endl;
			}
		}
		else if (deshacer) {
			leer = true;
		}
		else 
			salir = true;

	} while (!leer && !salir);

	if (salir)
		leer = false;
	
	return leer;
}

bool posiblesMovimientos(const tJuego &juego, tMovimiento mov, tPosibles &posible){
	
	bool mover = false;

	for (int i = 0; i < NUM_DIRS; i++) {
		if (i == 0) {
			//Para no salirse del rango arriba.
			if (mov.fila - 2 >= 0) { 
				if ((juego.tablero[mov.fila - 2][mov.col] == VACIA) && (juego.tablero[mov.fila - 1][mov.col] == FICHA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
		else if (i == 1) {
			if (mov.fila + 2 < DIM) {//Para no salirse del rango abajo.
				if ((juego.tablero[mov.fila + 2][mov.col] == VACIA) && (juego.tablero[mov.fila + 1][mov.col] == FICHA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
		else if (i == 2) {
			if (mov.col - 2 >= 0) {//Para no salirse del rango a la izquierda.
				if ((juego.tablero[mov.fila][mov.col - 2] == VACIA) && (juego.tablero[mov.fila][mov.col - 1] == FICHA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
		else if (i == 3) {
			if (mov.col + 2 < DIM) { //Para no salirse del rango a la derecha.
				if ((juego.tablero[mov.fila][mov.col + 2] == VACIA) && (juego.tablero[mov.fila][mov.col + 1] == FICHA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
	}
    return mover;
}

int mostrarMovimiento(const tPosibles &posible) {
	// Lista los movimientos posibles, y devuelve la opcion del usuario.
	
	int opcion;
	
	cout << "Selecciona direccion: " << endl;

	for (int i = 0; i < NUM_DIRS; i++)
		//Solo se mostrara los movimientos posibles.
		if (posible[i]) {  
			if (i == 0)
				cout << char(9) << "1 - Arriba" << endl;
			else if (i == 1)
				cout << char(9) << "2 - Abajo" << endl;
			else if (i == 2)
				cout << char(9) << "3 - Izquierda" << endl;
			else if (i == 3)
				cout << char(9) << "4 - Derecha" << endl;
		}

	cout << char(9) << "0 - Seleccionar otra" << endl;
	
	do {
		cin >> opcion;
		
		if (opcion > 4 || opcion < 0)
			cout << "La direccion no existe. Intentalo de nuevo." << endl;
	}while (opcion > 4 || opcion < 0);
	
	return opcion;
}

bool elegirMovimiento(tMovimiento &mov, const tPosibles &posible, int opcion) {
	
	bool mover = true;
	
	if (opcion == 1) {
		if (posible[0]) // 
			mov.posicion = ARRIBA;
		else
			mover = false;
	}
	else if (opcion == 2) {
		if (posible[1])
			mov.posicion = ABAJO;
		else
			mover = false;
	}
	else if (opcion == 3) {
		if (posible[2])
			mov.posicion = IZQUIERDA;
		else
			mover = false;
	}
	else if (opcion == 4) {
		if (posible[3])
			mov.posicion = DERECHA;
		else
			mover = false;
	}
	return mover;
}

void ejecutarMovimiento(tJuego &juego, tMovimiento mov) {
	// Ejecuta el movimiento y actualiza el valor de las fichas en el tablero.
	
	if (mov.posicion == ARRIBA) {
		juego.tablero[mov.fila - 2][mov.col] = FICHA;
		juego.tablero[mov.fila - 1][mov.col] = VACIA;
		juego.tablero[mov.fila][mov.col] = VACIA;
	}

	else if (mov.posicion == ABAJO) {
		juego.tablero[mov.fila + 2][mov.col] = FICHA;
		juego.tablero[mov.fila + 1][mov.col] = VACIA;
		juego.tablero[mov.fila][mov.col] = VACIA;
	}

	else if (mov.posicion == IZQUIERDA) {
		juego.tablero[mov.fila][mov.col - 2] = FICHA;
		juego.tablero[mov.fila][mov.col - 1] = VACIA;
		juego.tablero[mov.fila][mov.col] = VACIA;
	}

	else if (mov.posicion == DERECHA) {
		juego.tablero[mov.fila][mov.col + 2] = FICHA;
		juego.tablero[mov.fila][mov.col + 1] = VACIA;
		juego.tablero[mov.fila][mov.col] = VACIA;
	}
}

void ejecutarDeshacerMovimiento(tJuego &juego, const tMovimiento &mov) {
	// Deshace un movimiento.
	
	if (mov.posicion == ARRIBA) {
		juego.tablero[mov.fila - 2][mov.col] = VACIA;
		juego.tablero[mov.fila - 1][mov.col] = FICHA;
		juego.tablero[mov.fila][mov.col] = FICHA;
	}
	else if (mov.posicion == ABAJO) {
		juego.tablero[mov.fila + 2][mov.col] = VACIA;
		juego.tablero[mov.fila + 1][mov.col] = FICHA;
		juego.tablero[mov.fila][mov.col] = FICHA;
	}
	else if (mov.posicion == IZQUIERDA) {
		juego.tablero[mov.fila][mov.col - 2] = VACIA;
		juego.tablero[mov.fila][mov.col - 1] = FICHA;
		juego.tablero[mov.fila][mov.col] = FICHA;
	}
	else if (mov.posicion == DERECHA) {
		juego.tablero[mov.fila][mov.col + 2] = VACIA;
		juego.tablero[mov.fila][mov.col + 1] = FICHA;
		juego.tablero[mov.fila][mov.col] = FICHA;
	}
}

void calcularEstado(tJuego &juego) {
	//Calcula el estado de la partida despues de mover una ficha.
	
	bool jugar;
	bool ganar;

	jugar = jugando(juego);
	if (!jugar) {
		ganar = gana(juego);
		if (ganar)
			juego.estado = GANA;
		else
			juego.estado = BLOQUEO;
	}
	else
		juego.estado = JUGANDO;
}

void escribirListaMovimientos(tJuego &juego, const tMovimiento &mov, int pos) {
	//Guarda los movimientos que realiza el usuario con el fin de poder deshacerlos.

	if (pos < MAX_MOV) {
		juego.movimientos[pos].fila = mov.fila;
		juego.movimientos[pos].col = mov.col;
		juego.movimientos[pos].posicion = mov.posicion;
	}
	else {
		for (int i = 1; i < MAX_MOV; i++) 
			juego.movimientos[i - 1] = juego.movimientos[i];
		
		juego.movimientos[MAX_MOV -1].fila = mov.fila;
		juego.movimientos[MAX_MOV -1].col = mov.col;
		juego.movimientos[MAX_MOV -1].posicion = mov.posicion;
	}
}

void deshacerMovimientos(tJuego &juego) {
	//Deshace el numero de movimientos señalados por el usuario.
	
	int movimientos;
	tMovimiento mov;
	
	cout << "Introduce el numero de movimientos a deshacer: (MAX " << MAX_MOV << "): "; 
	cin >> movimientos;

	if (movimientos <= juego.numMovimientos && movimientos <= MAX_MOV && juego.posMov - movimientos >= 0) {
		if (juego.numMovimientos - 1 < MAX_MOV) {
			for (juego.posMov = juego.numMovimientos - 1; juego.posMov >= juego.numMovimientos - movimientos; juego.posMov--) {
				mov.fila = juego.movimientos[juego.posMov].fila;
				mov.col = juego.movimientos[juego.posMov].col;
				mov.posicion = juego.movimientos[juego.posMov].posicion;
				ejecutarDeshacerMovimiento(juego, mov);
			}
			juego.numMovimientos -= movimientos;
		}
		else  {
			for (juego.posMov = MAX_MOV - 1; juego.posMov >= MAX_MOV - movimientos; juego.posMov--) {
				mov.fila = juego.movimientos[juego.posMov].fila;
				mov.col = juego.movimientos[juego.posMov].col;
				mov.posicion = juego.movimientos[juego.posMov].posicion;
				ejecutarDeshacerMovimiento(juego, mov);
			}
			juego.numMovimientos -= movimientos;
		}

	}
	else if (movimientos > MAX_MOV) 
		cout << "El numero de movimientos a deshacer es mayor al maximo permitido. " << endl;
	else
		cout << "No se puede deshacer mas movimientos." << endl;
	
	// Lleva a la posicion actual de lista movimientos.
	juego.posMov++; 
}

bool jugando(tJuego juego) {
	//Comprueba que haya algun movimiento disponible para continuar jugando.
	
	bool jugar = false;
	bool mover;
	tPosibles posible; //En este caso no es relativo, pero necesito declarlo para llamar a posiblesMovimientos.
	tMovimiento mov;
	mov.fila = 0;

	while (!jugar && mov.fila < DIM) {
		mov.col = 0;
		while (!jugar && mov.col < DIM) {
			if (juego.tablero[mov.fila][mov.col] == FICHA) {
				mover = posiblesMovimientos(juego, mov, posible);
				if (mover)
					jugar = true;
			}
			mov.col++;
		}
		mov.fila++;
	}
	return jugar;
}

bool gana(const tJuego &juego) {
	//Comprueba si hay una unica ficha, y coincide con la meta.
	
	bool ganar = true;
	int fila = 0;
	int col;

	// Si encuentra al menos una ficha fuera de la meta. Se devolvera false.
	while (ganar && fila < DIM) {
		col = 0;
		while (ganar && col < DIM) {
			if (juego.tablero[fila][col] == FICHA) {
				if ((fila != juego.filaMeta && col != juego.colMeta) || (fila == juego.filaMeta && col != juego.colMeta)
					|| (fila != juego.filaMeta && col == juego.colMeta)) 
					ganar = false;
			}
			col++;
		}
		fila++;
	}
	return ganar;
}

bool posiblesMovimientosInversos(const tJuego &juego, tMovimiento mov, tPosibles &posible){
	// Comprueba los movimientos inversos disponibles.

	bool mover = false;
	
	for (int i = 0; i < NUM_DIRS; i++) {
		if (i == 0) {
			//Para no salirse del rango arriba.
			if (mov.fila - 2 >= 0) { 
				if ((juego.tablero[mov.fila - 2][mov.col] == VACIA || juego.tablero[mov.fila - 2][mov.col] == NULA)
				&& (juego.tablero[mov.fila - 1][mov.col] == VACIA || juego.tablero[mov.fila - 1][mov.col] == NULA)) {
					posible[i] = true; 
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
		else if (i == 1) {
			//Para no salirse del rango abajo.
			if (mov.fila + 2 < DIM) {
				if ((juego.tablero[mov.fila + 2][mov.col] == VACIA || juego.tablero[mov.fila + 2][mov.col] == NULA) 
				&& (juego.tablero[mov.fila + 1][mov.col] == VACIA || juego.tablero[mov.fila + 1][mov.col] == NULA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
		else if (i == 2) {
			//Para no salirse del rango a la izquierda.
			if (mov.col - 2 >= 0) {
				if ((juego.tablero[mov.fila][mov.col - 2] == VACIA || juego.tablero[mov.fila][mov.col - 2] == NULA)
				&& (juego.tablero[mov.fila][mov.col - 1] == VACIA || juego.tablero[mov.fila][mov.col - 1] == NULA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
		else if (i == 3) {
			//Para no salirse del rango a la derecha.
			if (mov.col + 2 < DIM) { 
				if ((juego.tablero[mov.fila][mov.col + 2] == VACIA || juego.tablero[mov.fila][mov.col + 2] == NULA) 
				&& (juego.tablero[mov.fila][mov.col + 1] == VACIA || juego.tablero[mov.fila][mov.col + 1] == NULA)) {
					posible[i] = true;
					mover = true;
				}
				else
					posible[i] = false;
			}
			else
				posible[i] = false; //Se sale del rango.
		}
	}

	return mover;
}

void ejecutarMovimientoInverso(tJuego &juego, tMovimiento mov){
	if (mov.posicion == ARRIBA) {
		juego.tablero[mov.fila][mov.col] = VACIA;
		juego.tablero[mov.fila - 1][mov.col] = FICHA;
		juego.tablero[mov.fila - 2][mov.col] = FICHA;
	}
	else if (mov.posicion == ABAJO) {
		juego.tablero[mov.fila][mov.col] = VACIA;
		juego.tablero[mov.fila + 1][mov.col] = FICHA;
		juego.tablero[mov.fila + 2][mov.col] = FICHA;
	}
	else if (mov.posicion == DERECHA) {
		juego.tablero[mov.fila][mov.col] = VACIA;
		juego.tablero[mov.fila][mov.col + 1] = FICHA;
		juego.tablero[mov.fila][mov.col + 2] = FICHA;
	}
	else if (mov.posicion == IZQUIERDA) {
		juego.tablero[mov.fila][mov.col] = VACIA;
		juego.tablero[mov.fila][mov.col - 1] = FICHA;
		juego.tablero[mov.fila][mov.col - 2] = FICHA;
	}
}

void generarTablero(tJuego &juego, int movimientos){
    bool mover = true;
	int i = 0;

	while (i <= movimientos && mover) {
        cout << char(9) << "Movimientos: " << i << endl;
        mostrar(juego);
        mover = movimientoInverso(juego);
		system("pause");
		if (!mover)
			cout << "No se han detectado mas movimientos para la ficha aleatoria. " << endl;
		i++;
    }
}

void iniciarTablero(tJuego &juego){
	//Iniciar el tablero en nulo.

	for (int fila = 0; fila < DIM; fila++)
		for (int col = 0; col < DIM; col++)
            juego.tablero[fila][col] = NULA;
	juego.bolasIniciales = 0;
	juego.numMovimientos = 0;
}

void fijarMeta(tJuego &juego){
	// Fija una meta aleatoriamente.
	srand(clock());
	juego.filaMeta = rand() % DIM;
	juego.colMeta = rand() % DIM;
	juego.tablero[juego.filaMeta][juego.colMeta] = FICHA;
}

bool movimientoInverso(tJuego &juego){
	bool puedeMover;
    tPosibles posibles;
    tMovimiento mov;
    srand(clock());

	do { // Coge una ficha aleatoriamente para mover.
		mov.fila = rand() % DIM;
		mov.col = rand() % DIM;
	} while (juego.tablero[mov.fila][mov.col] != FICHA);
	
	//Calcula los movimientos permitidos.
	puedeMover = posiblesMovimientosInversos(juego, mov, posibles);

	if (puedeMover) {
		movimientoInversoAleatorio(juego, posibles, mov);
		ejecutarMovimientoInverso(juego, mov);
	}	
    return puedeMover;
}

void movimientoInversoAleatorio(const tJuego &juego, const tPosibles &posible, tMovimiento &mov) {
	int pos;
	
	do { // Movimiento aleatorio.
		pos = rand() % NUM_DIRS;
	} while (!posible[pos]);

	if (pos == 0)
		mov.posicion = ARRIBA;
	else if (pos == 1)
		mov.posicion = ABAJO;
	else if (pos == 2)
		mov.posicion = IZQUIERDA;
	else if (pos == 3)
		mov.posicion = DERECHA;
}