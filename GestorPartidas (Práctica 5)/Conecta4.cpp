/*
·Juan Gomez Tarraga
*/

#include "Conecta4.h"
#include "Fecha.h"
#include <iostream>
using namespace std;

const int incF[] = { 1, 1, 0, -1 };
const int incC[] = { 0, 1, 1,  1 };

bool conecta4(tConecta4 const& juego, int fil, int col);
int enLinea(tConecta4 const& juego, int fil, int col, int dir);
//-------------------------------------------------------------------------------
inline tTurno cambiar(tTurno t) {
   return (t == Jugador1) ? Jugador2 : Jugador1;
}
//-------------------------------------------------------------------------------
inline bool enRango(int f, int c) {
   return 0 <= c && c < COLS && 0 <= f && f < FILAS;
}
//-------------------------------------------------------------------------------
inline bool sePuede(tConecta4 const& juego, int col) {
   return 0 <= col && col < COLS && juego.tablero[FILAS - 1][col] == Nadie;
}
//-------------------------------------------------------------------------------
void iniciar(tConecta4 & juego) { 
   juego.estado = Jugando;
   juego.turno = Jugador1;
   juego.fichas = 0;
   for (int i = 0; i < FILAS; i++) {
      for (int j = 0; j < COLS; j++) {
         juego.tablero[i][j] = Nadie;
      }
   }
}
//-------------------------------------------------------------------------------
bool aplicarJugada(tConecta4 & juego, int col) {
   if (juego.estado == Jugando && sePuede(juego, col)) {
      int pos = 0;
      while (juego.tablero[pos][col] != Nadie) {
        ++pos; 
      }
      juego.tablero[pos][col] = juego.turno;
      ++juego.fichas;
      if (conecta4(juego, pos, col)) juego.estado = Ganador;
      else if (juego.fichas == FILAS*COLS) juego.estado = Bloqueo;
      else juego.turno = cambiar(juego.turno);
      return true;
   } 
   else return false;
}
//-------------------------------------------------------------------------------
bool conecta4(tConecta4 const& juego, int f, int c) {
   bool conectados = false;
   int d = 0;
   while (d < 4 && !conectados) {          // 4 direcciones
     if (enLinea(juego, f, c, d) >= 4)     // 4 en línea
        conectados = true;
     else ++d;
   }
   return conectados;
}
//-------------------------------------------------------------------------------
int enLinea(tConecta4 const& juego, int f, int c, int dir) {
   int seguidos = 1;
   int i = f + incF[dir];
   int j = c + incC[dir];
   while (enRango(i, j) && juego.tablero[f][c] == juego.tablero[i][j]) {
      ++seguidos;
      i += incF[dir];
      j += incC[dir];
   }
   i = f - incF[dir];
   j = c - incC[dir];
   while (enRango(i, j) && juego.tablero[f][c] == juego.tablero[i][j]) {
      ++seguidos;
      i -= incF[dir];
      j -= incC[dir];
   }
   return seguidos;
}
//-------------------------------------------------------------------------------
void guardar(tConecta4 const& juego, ofstream & flujo) {
   flujo << juego.estado << ' '
         << juego.turno << ' ' 
         << juego.fichas << '\n';
   for (size_t i = 0; i < FILAS; ++i) {
      flujo << juego.tablero[i][0];
      for (size_t j = 1; j < COLS; ++j) {
         flujo << ' ' << juego.tablero[i][j];
      }
      flujo << '\n';
   } 
}
//-------------------------------------------------------------------------------
bool cargar(tConecta4 & juego, ifstream & flujo) {
   int num;
   flujo >> num; juego.estado = tEstado(num);
   flujo >> num; juego.turno = tTurno(num);
   flujo >> juego.fichas;
   for (size_t i = 0; i < FILAS; ++i) {
      for (size_t j = 0; j < COLS; ++j) {
         flujo >> num; juego.tablero[i][j] = tFicha(num);
      }
   }
   return !flujo.fail();
}
//----------------------------------------------------------------------------------------

#include <windows.h>

void colorFondo(int color) {
   HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(handle, 15 | (color << 4));
}

inline void colorFondo() { colorFondo(0); };
//----------------------------------------------------------------------------------------
inline void mostrarTurno(tConecta4 const& juego) {
   colorFondo(juego.turno); cout << ' '; colorFondo();
}
//----------------------------------------------------------------------------------------
inline void mostrarJugador1() {
   colorFondo(Jugador1); cout << ' '; colorFondo();
}
//----------------------------------------------------------------------------------------
inline void mostrarJugador2() {
   colorFondo(Jugador2); cout << ' '; colorFondo();
}
//----------------------------------------------------------------------------------------
void mostrar(tConecta4 const& juego) {
   for (int f = FILAS - 1; f >= 0; --f) {
      cout << '\t';
      for (size_t c = 0; c < COLS; ++c) {

		  colorFondo(juego.tablero[f][c]); cout << ' ';
         colorFondo(); cout << ' ';
      }
      cout << "\n\n";
   }
   colorFondo();
   cout << '\t';
   for (size_t c = 0; c < COLS; ++c) {
       cout << c+1 << ' ';
   }
   cout << '\n';
   if (juego.estado == Ganador) {
      cout << "\nGanador: ";
      mostrarTurno(juego);
      cout << "\n\n";
   }
}
//----------------------------------------------------------------------------------------
void mostrar(tConecta4 const& juego, string const& jugador1, string const& jugador2) {
   cout << '\n';
   mostrar(juego);
   cout << "    " << jugador1 << "("; mostrarJugador1(); cout << ")";
   if (juego.turno == Jugador1) cout << '*';
   cout << "    " << jugador2 << "("; mostrarJugador2(); cout << ")";
   if (juego.turno == Jugador2) cout << '*';
   cout << "\n\n";
}
//----------------------------------------------------------------------------------------

