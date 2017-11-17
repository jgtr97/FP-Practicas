/*
# |----------------------|
# | Juan Gomez Tarraga   |
# |Fernando Quijada Diaz |
# |----------------------|
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

const int DIM = 6;
const int NUM_DIRS = 4;
const int MAX_MOV = 3;

enum tCelda { NULA = 0, VACIA = 2, FICHA = 10 };
enum tEstado { BLOQUEO, GANA, JUGANDO, ABANDONO };
enum tPosicion { ARRIBA, ABAJO, IZQUIERDA, DERECHA, INCORRECTA };

typedef bool tPosibles[NUM_DIRS];

typedef struct {
	int fila, col; 
	tPosicion posicion; 
} tMovimiento;

typedef struct {
	tCelda tablero[DIM][DIM];
	int filaMeta, colMeta;
	int bolasIniciales;
	int numMovimientos;
	tEstado estado;
	tMovimiento movimientos[MAX_MOV];
	int posMov; //Contador en la lista de movimientos.
} tJuego;

/*------------------------------------------------*/
bool menuPrincipal();
int menuPartida();
/*------------------------------------------------*/
void colorFondo(int color);
void mostrar(const tJuego &juego);
/*------------------------------------------------*/
bool cargar(tJuego &juego, string nombre);
void guardar(const tJuego &juego, string nombre);
tCelda intToTCelda(int x);
/*------------------------------------------------*/
void partida(tJuego &juego);
bool selecFicha(const tJuego &juego, tMovimiento &mov, bool &valida, bool &deshacer);
bool leerMovimiento(const tJuego juego, tMovimiento &mov, bool &deshacer); 
int mostrarMovimiento(const tPosibles &posible);
bool elegirMovimiento(tMovimiento &mov, const tPosibles &posible, int opcion);
bool posiblesMovimientos(const tJuego &juego, tMovimiento mov, tPosibles &posibles);
void ejecutarMovimiento(tJuego &juego, tMovimiento mov); 
void escribirListaMovimientos(tJuego &juego, const tMovimiento &mov, int pos);
/*-------------------------------------------------*/
void deshacerMovimientos(tJuego &juego);
void ejecutarDeshacerMovimiento(tJuego &juego, const tMovimiento &mov);
/*-------------------------------------------------*/
void calcularEstado(tJuego &juego);
bool jugando(tJuego juego);
bool gana(const tJuego &juego);
/*--------------------------------------------------*/

//Para el movimiento inverso:
void iniciarTablero(tJuego &juego); 
void fijarMeta(tJuego &juego);
void generarTablero(tJuego &juego, int movimientos);
bool posiblesMovimientosInversos(const tJuego &juego, tMovimiento mov, tPosibles &posibles); 
void movimientoInversoAleatorio(const tJuego &juego, const tPosibles &posible, tMovimiento &mov);
void ejecutarMovimientoInverso(tJuego &juego, tMovimiento mov); 
bool movimientoInverso(tJuego &juego); 

