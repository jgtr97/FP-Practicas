/*
# |----------------------|
# | Juan Gomez Tarraga   |
# |Fernando Quijada Diaz |
# |----------------------|
*/

#include "unosolo.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	int opcion, numMovimientos;
	tJuego juego;
	bool carga, jugar;
	string nombre;
	char opcionGuardar;

	jugar = menuPrincipal();

	while (jugar) {
        opcion = menuPartida();
        
		// Para escoger tablero.
		if (opcion == 1) { 
			cout << "Escriba el nombre del tablero: ";
			cin >> nombre;

			// Carga el archivo nombrado.
			carga = cargar(juego, nombre); 
			
			if (carga) {
				// Funcion que lleva el control de la partida.
				partida(juego); 
				
				// Evita que el jugador guarde una partida si esta ya esta terminada.
				if (juego.estado == JUGANDO || juego.estado == ABANDONO) {

					cout << "Desea guardar la partida? (S/N): ";
					cin >> opcionGuardar;

					if (opcionGuardar == 'S' || opcionGuardar == 's') {
						cout << "Introduzca nombre de archivo para guardar partida: ";
						cin >> nombre;

						guardar(juego, nombre);
					}
				}
			}
			
			else
				cout << "No se pudo abrir el archivo :( " << endl;
        }
		
		// Tablero aleatorio.
		else { 
			iniciarTablero(juego);
			fijarMeta(juego);
			
			cout << "Introduzca el numero de movimientos: ";
			cin >> numMovimientos;
			
			generarTablero(juego, numMovimientos);
			
			cout << "Desea guardar la partida? (S/N): ";
			cin >> opcionGuardar;
			
			if (opcionGuardar == 'S' || opcionGuardar == 's') {
				cout << "Introduzca nombre de archivo para guardar partida: ";
				cin >> nombre;

				guardar(juego, nombre);
			}
		}
        jugar = menuPrincipal();
	}
    return 0;
}
