#ifndef MODELODATOS_H
#define MODELODATOS_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include "TriMesh.h"

#define RAD_INT_TORUS 1.0
#define RAD_EXT_TORUS 2.0

using namespace std;

class Punto
{
 public:
  double x;
  double y;

  // Constructor por defecto
  Punto() 
    {
      x = 0;
      y = 0;
    }

  // Constructor
  Punto(double a, double b)
    {
      x = a;
      y = b;
    }
  
  double getX();
  double getY();
  void setX(double val);
  void setY(double val);
  void Print();
};

class Coeficientes {
 public:
  double cX;
  double cY;
  double cZ; 

  Coeficientes() {
    cX = 0.0;
    cY = 0.0;
    cZ = 0.0;
  }

  Coeficientes(double a, double b, double c) {
    cX = a;
    cY = b;
    cZ = c;
  }
};

Coeficientes coeficientesMaya(TriMesh *themesh);

class Tablero {
 public:
  double ancho;
  double largo;

  Tablero() {
    ancho = 0.0;
    largo = 0.0;
  }

  Tablero(double a, double l) {
    ancho = a;
    largo = l;
  }

  // getters
  double getAncho() {return ancho;}
  double getLargo() {return largo;}

  // setters
  void setAncho(double a) {ancho = a;}
  void setLargo(double l) {largo = l;}
  void Print();
};

class Trayectoria
{
 public:
  double velocidad;
  int numPuntos;
  vector<Punto> listaPuntos;
  int origen;
  double lambdaX;
  double lambdaY;
  time_t timeStamp;

  // Constructor por defecto
  Trayectoria()
    {
      velocidad = 0.0;
      numPuntos = 0;
      listaPuntos = vector<Punto>();
      origen = 0;
      lambdaX = 0.0;
      lambdaY = 0.0;
      time(&timeStamp);
    }

  Trayectoria(double vel, int npuntos, vector<Punto> lstPuntos)
    {
      velocidad = vel;
      numPuntos = npuntos;
      listaPuntos = lstPuntos;
      origen = 0;
      lambdaX = 0.0;
      lambdaY = 0.0;
      time(&timeStamp);
    }

  void actLambdaX(double val);
  void actLambdaY(double val);
  void lambdaReset();
  int cambiarOrigen();
  void calcularNuevaPosicion(Punto *posActual);
  double normalizarD(double desplazamiento);
  void ecuacionRecta(Punto *posActual);
  double getVelocidad();
  int getNumPuntos();
  void calcularTrayectoria(Punto *p);
  double calcDesp(double velocidad);
  void Print();
};

//void coeficientesMaya(TriMesh *themesh);

class Objeto
{
 public:
  Punto ubicacion;
  char *archivoMaya;
  TriMesh *themesh;

  Objeto()
    {
      ubicacion = Punto();
      archivoMaya = NULL;
    }

  Objeto(Punto v, char *am) 
    {
      ubicacion = v;
      archivoMaya = am;
      
      // Cargar maya
      const char *filename = archivoMaya;
      themesh = TriMesh::read(filename);
      themesh->need_faces();
    }

  void Print();
  void dibujarObjeto();
};

enum Direccion { IZQUIERDA, DERECHA, ARRIBA, ABAJO};

class Jugador
{
 public:
  int vidas;
  double velocidad;
  enum Direccion dir;
  int turbo;
  double velocidadTurbo;
  char * archivoMaya;
  TriMesh *themesh;
  Coeficientes cf;
  Punto ubicacionActual;

  Jugador() 
    {
      vidas = 3;
      velocidad = 0.0;
      dir = ARRIBA;
      turbo = 0;
      velocidadTurbo = 0.0;
      archivoMaya = NULL;
      ubicacionActual = Punto();
      cf = Coeficientes();
    }

  Jugador(int v, double vl, int t, double vlt, char *am, Punto pI)
    {
      vidas = v;
      velocidad = vl;
      dir = ARRIBA;
      turbo = t;
      velocidadTurbo = vlt;
      archivoMaya = am;
      ubicacionActual = pI;

      // Cargar maya
      const char *filename = archivoMaya;
      themesh = TriMesh::read(filename);
      themesh->need_faces();
    }

  void Print();
  void dibujarJugador();
  void dirIzquierda();
  void dirDerecha();
  void dirArriba();
  void dirAbajo();
};

class Contrincante {
 public:
  Trayectoria t;
  Punto ubicacionActual;
  char* maya;
  TriMesh *themesh;
  Coeficientes cf;

  Contrincante() {
    t = Trayectoria();
    ubicacionActual = Punto();
    maya = NULL;
    cf = Coeficientes();
  }

  Contrincante(Trayectoria tr, char* mya) {
    t = tr;
    maya = mya;
    ubicacionActual = Punto(t.listaPuntos[0].getX(), 
                            t.listaPuntos[0].getY());
    
    // Cargar maya
    const char *filename = maya;
    themesh = TriMesh::read(filename);
    themesh->need_faces();
    cout << "Maya Contrincante = " << maya << endl;
  }
  void Print();
  void dibujarTrayectoriaC();
  void dibujarContrincante();
};

class Nivel
{
 public:
  int id;
  int vidas;
  Tablero t;
  Punto salida;
  char *terrenoBN;
  Jugador j;
  int numContrincantes;
  vector<Contrincante> listaContrincantes;
  int numObjetos;
  vector<Objeto> listaObjetos;

  Nivel() 
    {
      id = 0;
      vidas = 0;
      t = Tablero();
      salida = Punto();
      terrenoBN = NULL;
      j = Jugador();
      numContrincantes = 0;
      listaContrincantes = vector<Contrincante>();
      numObjetos = 0;
      listaObjetos = vector<Objeto>();
    }
  
  Nivel(int ident, int vd, Tablero tab, Punto sal, char * tBN, 
        Jugador jug, int nContr, vector<Contrincante> listContr, 
        int nObjetos, vector<Objeto> listObj)
    {
      id = ident;
      vidas = vd;
      t = tab;
      salida = sal;
      terrenoBN = tBN;
      j = jug;
      numContrincantes = nContr;
      listaContrincantes = listContr;
      numObjetos = nObjetos;
      listaObjetos = listObj;
    }

  void dibujarTrayectoriaContrincantes();
  void dibujarJugadores();
  void dibujarObstaculos();
  void Print();
};

class Juego
{
 public:
  int numNiveles;
  vector<Nivel> listaNiveles;

  Juego()
    {
      numNiveles = 0;
      listaNiveles = vector<Nivel>();
    }

  Juego(int nNiveles, vector<Nivel> lstNivel)
    {
      numNiveles = nNiveles;
      listaNiveles = lstNivel;
    }
  void Print();
};

#endif
