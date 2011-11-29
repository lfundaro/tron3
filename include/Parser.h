#ifndef PARSER_H
#define PARSER_H

#include "ModeloDatos.h"
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <iostream>
#include "tinyxml.h"
#define MAX_CHAR_SIZE 256

using namespace std;

Punto parsePunto(TiXmlHandle rootPunto);

Trayectoria parseTrayectoria(TiXmlHandle rootTrayectoria);

Jugador parseJugador(TiXmlHandle rootJugador, int vidas);

Contrincante parseContrincante(TiXmlHandle rootContrincante);

vector<Objeto> parseObjetos(TiXmlHandle rootObjetos);

Tablero parseTablero(TiXmlHandle rootTablero);

Nivel parseNivel(TiXmlHandle rootNivel);

Juego parse (char *archivo);

#endif 
