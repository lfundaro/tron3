#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "include/ModeloDatos.h"
#include "include/Camara.h"
#include "include/Parser.h"
#include "include/Elementos.h"
#include "include/TriMesh.h"
#include "include/XForm.h"
#include "include/ICP.h"
#include "include/GLCamera.h"
#include "include/glm.h"

using namespace std;

/* INICIO Variables globales */
Juego j;
float tamX;
float tamY;
float tamZ;
float giroH = 0;
float giroV = 0;
int nivelActual = 0;
Punto posActualJugador;
vector<Punto> posContrincante;
double incr = 0.001;
GLfloat ambiente[4];
GLfloat difusa[4];
GLfloat especular[4];
GLfloat posicion[4];

static GLuint texName;
int iheight, iwidth;
unsigned char* image = NULL;

static GLuint texName2;
int iheight2, iwidth2;
unsigned char* image2 = NULL;
float zoom = 90.0;
#define ZOOM_FACTOR 1.0
Camara cam = Camara((tamY+2)/10*6,-tamX+2/3, (-tamY+2)/20);
GLfloat camUpDown = 0.0;
GLfloat rot = 0.0;
GLfloat avanceX = 0.0;
GLfloat avanceY = 0.0;

/* INICIO Variables para display list */
GLuint base;
GLuint paredes;
GLuint tablero;
GLuint meshJugador;
/* FIN Variables para display list */

/* FIN Variables globales */

/* Alarma para controlar Turbo */

void  SIGALRM_control (int signum) {
  j.listaNiveles[nivelActual].j.desactivarTurbo();
}

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  /* Coordenad=as del sistema */
  glLoadIdentity();
  cam.go(tamX, tamY, giroH, giroV, avanceX, avanceY, camUpDown);

  //gluLookAt (0.0, (-x)/10.0,(-x)/10*6, 0.0,(-y)/20.0,(-x)/3.0, 0.0, 0.0,-1.0);
  glRotatef(rot, 0.0,0.0,1.0);

  /* Tablero */

  //  glEnable(GL_TEXTURE_2D);
  //  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  //  glBindTexture(GL_TEXTURE_2D, texName);
  glCallList(base);

  //  glBindTexture(GL_TEXTURE_2D, texName2);
  glCallList(paredes);
  //  glDisable(GL_TEXTURE_2D);

  // glLightfv(GL_LIGHT0, GL_POSITION, ambiente);
  // glLightfv(GL_LIGHT0, GL_POSITION, difusa);
  // glLightfv(GL_LIGHT0, GL_POSITION, especular);
  // glLightfv(GL_LIGHT0, GL_POSITION, posicion);
  glCallList(tablero);

  // Dibujar trayectoria de Jugador
  //  j.listaNiveles[nivelActual].j.dibujarTrayectoriaJ();
  j.listaNiveles[nivelActual].dibujarTrayectoriaContrincantes();
  // Dibujar jugadores en posición inicial
  j.listaNiveles[nivelActual].dibujarJugadores();
  // Dibujar obstáculos
  j.listaNiveles[nivelActual].dibujarObstaculos();
  glutPostRedisplay();
  glutSwapBuffers();
  glFlush ();
  return;
}

void reshape (int w, int h) {
  float aspectratio;
  aspectratio = (float) w / (float) h;
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  //  gluPerspective(90.0f, 1, 0.5, 100.0);
  gluPerspective(zoom, 1, 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
  //  glEnable(GL_LIGHTING);
  //  glEnable(GL_LIGHT0);
}

void flechas(int key, int x, int y) {
  switch(key) {
  case GLUT_KEY_LEFT:
    j.listaNiveles[nivelActual].j.dirIzquierda();
    break;
  case GLUT_KEY_RIGHT:
    j.listaNiveles[nivelActual].j.dirDerecha();
    break;
  case GLUT_KEY_DOWN:
    j.listaNiveles[nivelActual].j.dirAbajo();
    //    giroV -= 2.0;
    break;
  case GLUT_KEY_UP:
    j.listaNiveles[nivelActual].j.dirArriba();
    //    giroV += 2.0;
    break;
  }
  j.listaNiveles[nivelActual].j.cambiarDireccion();
}

void teclaPausa() {
  sleep(5);
}

void teclaZoomIn() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  zoom -= ZOOM_FACTOR;
  gluPerspective(zoom, 1, 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void teclaZoomOut() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  zoom += ZOOM_FACTOR;
  gluPerspective(zoom, 1, 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void teclaCamUp() {
  camUpDown -= 0.5;
}

void teclaCamDown() {
  camUpDown += 0.5;
}

void teclaRotIzq() {
  rot -= 3.0;
}

void teclaRotDer() {
  rot += 3.0;
}

void teclaTrasXIzq() {
  avanceX += 0.5; 
}

void teclaTrasXDer() {
  avanceX -= 0.5;
}

void teclaTrasYDer() {
  avanceY -= 0.5;
}

void teclaTrasYIzq() {
  avanceY += 0.5;
}

void keyboard (unsigned char key, int x, int y)  {
  switch (key)
    {
    case 'P': case 'p':
      teclaPausa();
      glutPostRedisplay();
      break;
    case 'I': case 'i':
      teclaZoomIn();
      break;
    case 'O': case 'o':
      teclaZoomOut();
      break;
    case 'U': case 'u': 
      teclaCamUp();
      break;
    case 'D': case 'd':
      teclaCamDown();
      break;
    case 'A': case 'a':
      teclaRotIzq();
      break;
    case 'S': case 's':
      teclaRotDer();
      break;
    case 'F': case 'f': 
      teclaTrasXIzq();
      break;
    case 'G': case 'g':
      j.listaNiveles[nivelActual].j.setGo();
      j.listaNiveles[nivelActual].setGo();
      //      teclaTrasXDer();
      break;
    case 'B': case 'b': 
      teclaTrasYIzq();
      break;
    case 'N': case 'n':
      teclaTrasYDer();
      break;
    case 32: // TURBO !
      if (j.listaNiveles[nivelActual].j.getNumTurbo() > 0) {
        j.listaNiveles[nivelActual].j.activarTurbo();
        alarm(1);
      }
    default:
      printf("Didnt match\n");
      break;
    }
  return;
}

void textureInit(){
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);


   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   image = glmReadPPM("texturas/imagen1.ppm", &iwidth, &iheight);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

   glGenTextures(1, &texName2);
   glBindTexture(GL_TEXTURE_2D, texName2);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   image2 = glmReadPPM("texturas/imagen2.ppm", &iwidth2, &iheight2);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth2, iheight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);

}

static void init(void) {
  // Base
  base = glGenLists(1);
  glNewList(base, GL_COMPILE);
  dibujarBase(tamX, tamY);
  glEndList();
  // Paredes
  paredes = glGenLists(1);
  glNewList(paredes, GL_COMPILE);
  dibujarParedes(tamX,tamY,tamZ);
  glEndList();
  // tablero 
  tablero = glGenLists(1);
  glNewList(paredes, GL_COMPILE);
  dibujarTablero(tamX,tamY,tamZ);
  glEndList();
  return;
}

void initLuz() {
  ambiente = {0.01*tamX, 0.02*tamY, 0.02, 1.0};
  difusa = {0.05*tamX, 0.05*tamY, 0.05, 1.0 };
  especular = {0.08*tamX, 0.08*tamY, 0.08, 1.0};
}

void initPosicion() {
  posicion = {0.5*tamX,0.5*tamY,0.3,0.0};
}

void initTamTablero(Juego j, int nivelActual) {
  tamX = j.listaNiveles[nivelActual].t.getAncho();
  tamY = j.listaNiveles[nivelActual].t.getLargo();
  tamZ = 2.0;
}

int main (int argc, char **argv) {
  /* Abrir archivo e inicializar estructuras de juego */
  char *archivo = argv[1];
  j = parse(archivo);
  
  initTamTablero(j, nivelActual);
  //  initLuz();
  initPosicion();

  /* Inicialización de ventana */
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
  glutInitWindowSize (1280,800);
  glutInitWindowPosition (100, 150);

  glutCreateWindow (argv[0]);
  /* Propiedades de openGL */
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glClearDepth (1.0f);
  glClearColor(1.0,1.0,1.0,1.0f);
  glShadeModel(GL_SMOOTH);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //  textureInit();

  /* Directivas para graficar */
  init();
  // Binding de alarma
  signal (SIGALRM, SIGALRM_control);
  
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutSpecialFunc(flechas);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  exit (EXIT_SUCCESS);
}
