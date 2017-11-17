/*
·Juan Gomez Tarraga
*/

#ifndef FECHA_H
#define FECHA_H

#include <ctime>
#include <sstream>
using namespace std;

typedef time_t tFecha;

tFecha fechaActual();
string stringFecha(tFecha fecha, bool hora);

#endif
