#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
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

float zoom = 90.0;
#define ZOOM_FACTOR 1.0
Camara cam = Camara((tamY+2)/10*6,-tamX+2/3, (-tamY+2)/20);
GLfloat camUpDown = 0.0;
GLfloat rot = 0.0;
GLfloat avanceX = 0.0;
GLfloat avanceY = 0.0;
float maximoX = 0.0;
float maximoY = 0.0;
float maximoZ = 0.0;
float despJugX = 0.0;
float despJugY = 0.0;
double cX;
double cY;
double cZ;

/* INICIO Variables para display list */
GLuint base;
GLuint paredes;
GLuint tablero;
GLuint meshJugador;
/* FIN Variables para display list */

/* INICIO Variables para las luces*/
GLfloat ambiente[4];
GLfloat difusa[4];
GLfloat especular[4];
GLfloat posicion[4];
/* FIN Variables para las luces*/

/* INICIO Variables para las texturas*/
static GLuint texName;
int iheight, iwidth;
unsigned char* image = NULL;

static GLuint texName2;
int iheight2, iwidth2;
unsigned char* image2 = NULL;
/* FIN Variables para las texturas*/

/* INICIO Variables para el manejo de la Ventana y Sub-Ventanas*/
int camara = 0;
int venMain;
int subVen1 = 0;
int subVen2 = 0;
int subVen3 = 0;
int subVen4 = 0;
int GAP = 10;
int venMain_w = 1200;
int venMain_h = 800;
int subVen_w = (venMain_w - (3*GAP))/2;
int subVen_h = (venMain_h - (3*GAP))/2;
/* FIN Variables para el manejo de la Ventana y Sub-Ventanas*/

/* FIN Variables globales */
void cambioCamara() {
  Punto salida = (j.listaNiveles[nivelActual]).salida;
  if (camara == 1) {
    // Vista en primera perona
    gluLookAt(posActualJugador.getX(),posActualJugador.getY(),2,
	      posActualJugador.getX(),posActualJugador.getY()+2,0,
	      0.0,1.0,0.0);
  }
  else if (camara == 2) {
    // Vista desde arriba cerca del jugador
    gluLookAt(posActualJugador.getX(),posActualJugador.getY(),2,
	      posActualJugador.getX(),posActualJugador.getY()+2,0,
	      0.0,1.0,0.0);
  } else if (camara == 3) {
    //Vista desde la salida
    gluLookAt(salida.getX(),salida.getY(),2,
	      salida.getY(),salida.getY(),0,
	      0.0,1.0,0.0);
  } else if (camara == 4) {
    // Vista de las 3+1
  } else {
    // Vista desde Arriba de TODO el tablero
    if (tamX == tamY) {
      gluLookAt (0.0, (-(tamX+2))/10, (tamY+2)/10*6,
                 0.0+giroH, giroV+(-(tamY+2))/20, (-(tamX+2))/3,
                 0.0,1, 0.0);
    } else if (tamX < tamY) {
      gluLookAt (0.0, (-(tamY+2))/10, (tamY+2)/10*6,
                 0.0+giroH, giroV+(-(tamY+2))/20, (-(tamY+2))/3,
                 0.0,1, 0.0);
    } else {
      gluLookAt (0.0, (-(tamX+2))/10, (tamX+2)/10*6,
                 0.0+giroH, giroV+(-(tamX+2))/20, (-(tamX+2))/3,
                 0.0,1, 0.0);
    }
    glTranslatef((-tamX)/2 + avanceX,(-tamY)/2 + avanceY,camUpDown);
  }
}

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  /* Coordenadas del sistema */

  glLoadIdentity();
  cambioCamara();
  /* La CAMARA Como dato
  cam.go(tamX, tamY, giroH, giroV, avanceX, avanceY, camUpDown);
  */
  glRotatef(rot, 0.0,0.0,1.0);

  /* Tablero */

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D, texName);
  glCallList(base);

  glBindTexture(GL_TEXTURE_2D, texName2);
  glCallList(paredes);
  glDisable(GL_TEXTURE_2D);

  glLightfv(GL_LIGHT0, GL_POSITION, ambiente);
  glLightfv(GL_LIGHT0, GL_POSITION, difusa);
  glLightfv(GL_LIGHT0, GL_POSITION, especular);
  glLightfv(GL_LIGHT0, GL_POSITION, posicion);
  glCallList(tablero);

  glPushMatrix();
  glScalef(0.2,0.2,0.2);
  glTranslatef(despJugX, despJugY,0.0);
  //  glCallList(meshJugador);
  glPopMatrix();

  // Dibujar trayectoria de Jugador
  //  j.listaNiveles[nivelActual].j.dibujarTrayectoriaJ();
  j.listaNiveles[nivelActual].dibujarTrayectoriaContrincantes();
  // Dibujar jugadores en posición inicial
  j.listaNiveles[nivelActual].dibujarJugadores();
  // Dibujar obstáculos
  //  j.listaNiveles[nivelActual].dibujarObstaculos();
  // Dibujar discos
  //  j.listaNiveles[nivelActual].dibujarDiscos();
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
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  cout << w << endl;
  cout << h << endl;
  venMain_w = w;
  venMain_h = h;
  subVen_w = (venMain_w - (3*GAP))/2;
  subVen_h = (venMain_h - (3*GAP))/2;
  if (camara == 4) {
    glutSetWindow (subVen1);
    glutPositionWindow (GAP, GAP);
    glutReshapeWindow (subVen_w, subVen_h);

    glutSetWindow (subVen2);
    glutPositionWindow (GAP+subVen_w+GAP,GAP);
    glutReshapeWindow (subVen_w, subVen_h);

    glutSetWindow (subVen3);
    glutPositionWindow (GAP,GAP+subVen_h+GAP);
    glutReshapeWindow (subVen_w, subVen_h);

    glutSetWindow (subVen4);
    glutPositionWindow (GAP+subVen_w+GAP,GAP+subVen_h+GAP);
    glutReshapeWindow (subVen_w, subVen_h);
  }
}

void subReshape (int w, int h) {
  float aspectratio;
  aspectratio = (float) w / (float) h;
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(zoom, 1, 0.5, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void flechas(int key, int x, int y) {
  // float tmpMax = 10.0;
  // float tmpMov = 1.0;
  // switch(key) {
  // case GLUT_KEY_LEFT:
  //   if (giroH > (-tmpMax))
  //     giroH -= tmpMov;
  //   break;
  // case GLUT_KEY_RIGHT:
  //   if (giroH < (tmpMax))
  //     giroH += tmpMov;
  //   break;
  // case GLUT_KEY_DOWN:
  //   if (giroV > (-tmpMax))
  //     giroV -= tmpMov;
  //   break;
  // case GLUT_KEY_UP:
  //   //    if (giroV < (tmpMax))
  //     giroV += tmpMov;
  //   break;
  // }
  //  mouVenX = x;
  //  mouVenY = y;
  //  proyectarMouse(x,y);

  switch(key) {
  case GLUT_KEY_LEFT:
      despJugX -= 2.0;
    break;
  case GLUT_KEY_RIGHT:
      despJugX += 2.0;
    break;
  case GLUT_KEY_DOWN:
      despJugY -= 2.0;
    break;
  case GLUT_KEY_UP:
      despJugY += 2.0;
    break;
  }
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

void endSD() {
  glRotatef(rot, 0.0,0.0,1.0);
  /* Tablero */
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texName);
  glCallList(base);
  glBindTexture(GL_TEXTURE_2D, texName2);
  glCallList(paredes);
  glDisable(GL_TEXTURE_2D);
  glLightfv(GL_LIGHT0, GL_POSITION, ambiente);
  glLightfv(GL_LIGHT0, GL_POSITION, difusa);
  glLightfv(GL_LIGHT0, GL_POSITION, especular);
  glLightfv(GL_LIGHT0, GL_POSITION, posicion);
  glCallList(tablero);
  glPushMatrix();
  glScalef(0.2,0.2,0.2);
  glTranslatef(despJugX, despJugY,0.0);
  //  glCallList(meshJugador);
  glPopMatrix();
  // Dibujar trayectoria de Jugador
  //  j.listaNiveles[nivelActual].j.dibujarTrayectoriaJ();
  j.listaNiveles[nivelActual].dibujarTrayectoriaContrincantes();
  // Dibujar jugadores en posición inicial
  j.listaNiveles[nivelActual].dibujarJugadores();
  // Dibujar obstáculos
  //  j.listaNiveles[nivelActual].dibujarObstaculos();
  // Dibujar discos
  //  j.listaNiveles[nivelActual].dibujarDiscos();
  glutPostRedisplay();
  glutSwapBuffers();
  glFlush ();
}

void subDisplay1() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 // Vista desde arriba cerca del jugador
  gluLookAt(posActualJugador.getX(),posActualJugador.getY(),2,
	    posActualJugador.getX(),posActualJugador.getY()+2,0,
	    0.0,1.0,0.0);
  endSD();
  return;
}

void subDisplay2() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // Vista en primera perona
  gluLookAt(posActualJugador.getX(),posActualJugador.getY(),2,
	    posActualJugador.getX(),posActualJugador.getY()+2,0,
	    0.0,1.0,0.0);
  endSD();
  return;
}

void subDisplay3() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Punto salida = (j.listaNiveles[nivelActual]).salida;
  //Vista desde la salida
  gluLookAt(salida.getX(),salida.getY(),2,
	    salida.getY(),salida.getY(),0,
	    0.0,1.0,0.0);
  endSD();
  return;
}

void subDisplay4() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // Vista desde Arriba de TODO el tablero
  if (tamX == tamY) {
    gluLookAt (0.0, (-(tamX+2))/10, (tamY+2)/10*6,
	       0.0+giroH, giroV+(-(tamY+2))/20, (-(tamX+2))/3,
	       0.0,1, 0.0);
  } else if (tamX < tamY) {
    gluLookAt (0.0, (-(tamY+2))/10, (tamY+2)/10*6,
	       0.0+giroH, giroV+(-(tamY+2))/20, (-(tamY+2))/3,
	       0.0,1, 0.0);
  } else {
    gluLookAt (0.0, (-(tamX+2))/10, (tamX+2)/10*6,
	       0.0+giroH, giroV+(-(tamX+2))/20, (-(tamX+2))/3,
	       0.0,1, 0.0);
  }
  glTranslatef((-tamX)/2 + avanceX,(-tamY)/2 + avanceY,camUpDown);
  endSD();
  return;
}

/*
******
Funcion de Keyboard Utilizada para la entrega anterior
MUCHO mejor que la nueva
*******
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
      teclaTrasXDer();
      break;
    case 'B': case 'b': 
      teclaTrasYIzq();
      break;
    case 'N': case 'n':
      teclaTrasYDer();
      break;
    default:
      printf("Didnt match\n");
      break;
    }
  return;
}
*/
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
  // // mesh Jugador
  // meshJugador = glGenLists(1);
  // glNewList(meshJugador, GL_COMPILE);
  // //  drawMesh();
  // glEndList();
  return;
}

void initLuz() {
  ambiente = {0.01*tamX, 0.02*tamY, 0.02, 1.0};
  difusa = {0.05*tamX, 0.05*tamY, 0.05, 1.0 };
  especular = {0.08*tamX, 0.08*tamY, 0.08, 1.0};
}

void initPosicion() {
  posicion = {0.5*tamX,0.5*tamY,0.3,0.0};
  // despJugX = j.listaNiveles[nivelActual].j.posInicial.getX()*5.0;
  // despJugY = j.listaNiveles[nivelActual].j.posInicial.getY() + 6;
}

void initTamTablero(Juego j, int nivelActual) {
  tamX = j.listaNiveles[nivelActual].t.getAncho();
  tamY = j.listaNiveles[nivelActual].t.getLargo();
  tamZ = 2.0;
}

void initVentana(){
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glClearDepth (1.0f);
  glClearColor(1.0,1.0,1.0,1.0f);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  textureInit();
  init();
}

void offVentanas(){
  if (subVen1 != 0 && subVen2 != 0 && subVen3 != 0 && subVen4 != 0) {
    glutDestroyWindow(subVen1);
    glutDestroyWindow(subVen2);
    glutDestroyWindow(subVen3);
    glutDestroyWindow(subVen4);
    subVen1 = 0;
    subVen2 = 0;
    subVen3 = 0;
    subVen4 = 0;
  }
}

void keyboard (unsigned char key, int x, int y)  {
  switch (key)
    {
    case 'T': case 't':
      if (camara != 4) {
	camara = 4;
	subVen1 = glutCreateSubWindow(venMain,GAP,GAP,subVen_w,subVen_h);
	initVentana();
	glutDisplayFunc(subDisplay1);
	glutReshapeFunc(subReshape);
	glutSpecialFunc(flechas);
	glutKeyboardFunc(keyboard);

	subVen2 = glutCreateSubWindow(venMain,GAP+subVen_w+GAP,GAP,subVen_w,subVen_h);
	initVentana();
	glutDisplayFunc(subDisplay2);
	glutReshapeFunc(subReshape);
	glutSpecialFunc(flechas);
	glutKeyboardFunc(keyboard);

	subVen3 = glutCreateSubWindow(venMain,GAP,GAP+subVen_h+GAP,subVen_w,subVen_h);
	initVentana();
	glutDisplayFunc(subDisplay3);
	glutReshapeFunc(subReshape);
	glutSpecialFunc(flechas);
	glutKeyboardFunc(keyboard);

	subVen4 = glutCreateSubWindow(venMain,GAP+subVen_w+GAP,GAP+subVen_h+GAP,subVen_w,subVen_h);
	initVentana();
	glutDisplayFunc(subDisplay4);
	glutReshapeFunc(subReshape);
	glutSpecialFunc(flechas);
	glutKeyboardFunc(keyboard);
      }
      break;
    case 'P': case 'p':
      teclaPausa();
      glutPostRedisplay();
      break;
    case 'F': case 'f':
      camara = 0;
      offVentanas();
      break;
    case 'D': case 'd':
      camara = 1;
      offVentanas();
      break;
    case 'R': case 'r':
      camara = 2;
      offVentanas();
      break;
    case 'E': case 'e':
      camara = 3;
      offVentanas();
      break;
    }
}

int main (int argc, char **argv) {
  /* Abrir archivo e inicializar estructuras de juego */
  char *archivo = argv[1];
  j = parse(archivo);
  initTamTablero(j, nivelActual);
  initLuz();
  initPosicion();

  /* InicializaciÃ³n de ventana */
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
  glutInitWindowSize (venMain_w,venMain_h);
  glutInitWindowPosition (100, 150);

  venMain = glutCreateWindow (argv[0]);
 /* Propiedades de openGL */
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glClearDepth (1.0f);
  glClearColor(1.0,1.0,1.0,1.0f);
  glShadeModel(GL_SMOOTH);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  textureInit();

  /* Directivas para graficar */
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutSpecialFunc(flechas);
  glutKeyboardFunc(keyboard);

  glutMainLoop();

  exit (EXIT_SUCCESS);
}
