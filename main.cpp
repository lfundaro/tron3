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
vector<Punto> posContrincante;
double incr = 0.001;

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

/* INICIO Variables para las luces*/
GLfloat ambiente[4];
GLfloat difusa[4];
GLfloat especular[4];
GLfloat posicion[4];
/* FIN Variables para las luces*/

/* INICIO Variables para las texturas*/
static GLuint texName;
unsigned char* image = NULL;

static GLuint texName2;
unsigned char* image2 = NULL;

static GLuint texVida0;
unsigned char* vida0 = NULL;
static GLuint texVida1;
unsigned char* vida1 = NULL;
static GLuint texVida2;
unsigned char* vida2 = NULL;
static GLuint texVida3;
unsigned char* vida3 = NULL;

static GLuint texAcel;
unsigned char* acel = NULL;

static GLuint texMapHERE;
unsigned char* mapHERE = NULL;
static GLuint texMapN;
unsigned char* mapN = NULL;
static GLuint texMapS;
unsigned char* mapS = NULL;
static GLuint texMapE;
unsigned char* mapE = NULL;
static GLuint texMapO;
unsigned char* mapO = NULL;
static GLuint texMapNE;
unsigned char* mapNE = NULL;
static GLuint texMapNO;
unsigned char* mapNO = NULL;
static GLuint texMapSO;
unsigned char* mapSO = NULL;
static GLuint texMapSE;
unsigned char* mapSE = NULL;

static GLuint texPrueba;
unsigned char* prueba = NULL;
/* FIN Variables para las texturas*/

/* INICIO Variables para el manejo de la Ventana y Sub-Ventanas*/
int camara = 0;
int venMain;
int subDato = 0;
int subDat2 = 0;
int subMain = 0;
int subVen1 = 0;
int subVen2 = 0;
int subVen3 = 0;
int subVen4 = 0;
int GAP = 10;
int venMain_w = 1200;
int venMain_h = 800;
int venDato_w = 460;
int subVen_w = (venMain_w - (3*GAP))/2;
int subVen_h = (venMain_h - (3*GAP))/2;
/* FIN Variables para el manejo de la Ventana y Sub-Ventanas*/

/* FIN Variables globales */
float varX = 0;
float varY = 0;
float varZ = 0;
//Punto inicio =  (j.listaNiveles[nivelActual]).j.ubicacionActual;
float angulo = 1;
float dtr( float d )
{
        return d*3.141592/180;
}

void camara1(){
  Punto posActualJugador = (j.listaNiveles[nivelActual]).j.ubicacionActual;
  float desX;
  float desY;
  //  Vista en 1ra Persona
  if (angulo == 1)
    // Vista al Frente
    gluLookAt(posActualJugador.getX(),posActualJugador.getY()+1.4,1,
              posActualJugador.getX(),posActualJugador.getY()+3.5,0,
              0.0,0.0,1.0);
  else if (angulo == -1) {
    // Vista hacia atras
    glRotatef(180.0f,0.0f,1.0f,0.0f);
    gluLookAt(posActualJugador.getX(),posActualJugador.getY()+3,1.5,
              posActualJugador.getX(),posActualJugador.getY()+8,0.5,
              0.0,0.0,1.0);
  }
  else if (angulo == 2) {
    // Vista a la derecha
    gluLookAt(posActualJugador.getX()+0.1,posActualJugador.getY()+1.25,1,
              posActualJugador.getX()+5,posActualJugador.getY()+1.25,0,
              0.0,0.0,1.0);
  }
  else if (angulo == -2) {
    // Vista a la izquierda
    gluLookAt(posActualJugador.getX()-0.1,posActualJugador.getY()+1.15,1,
              posActualJugador.getX()-5,posActualJugador.getY()+1.15,0,
              0.0,0.0,1.0);
  }
}

void camara2() {
  // Vista desde arriba cerca del jugador
  Punto posActualJugador = (j.listaNiveles[nivelActual]).j.ubicacionActual;
  gluLookAt (posActualJugador.getX(),posActualJugador.getY(),5,
             posActualJugador.getX(),posActualJugador.getY()+1,0,
             0.0,0.0,1.0);
}
void camara3(){
  Punto salida = (j.listaNiveles[nivelActual]).salida;
  Punto jugador = (j.listaNiveles[nivelActual]).j.ubicacionActual;
  //Vista desde la salida
  gluLookAt (salida.getX(),salida.getY(),5,
             jugador.getX(),jugador.getY(),0,
             0,0,1.0);
}

void camaraA(){
  // Vista desde Arriba de TODO el tablero
  if (tamX == tamY) {
    gluLookAt (0.0, (-(tamX+2))/10, (tamY+2)/10*6,
               0.0,(-(tamY+2))/20, (-(tamX+2))/3,
               0.0,0.0,1.0);
  } else if (tamX < tamY) {
    gluLookAt (0.0, (-(tamY+2))/10, (tamY+2)/10*6,
               0.0,(-(tamY+2))/20, (-(tamY+2))/3,
               0.0,0.0,1.0);
  } else {
    gluLookAt (0.0, (-(tamX+2))/10, (tamX+2)/10*6,
               0.0,(-(tamX+2))/20, (-(tamX+2))/3,
               0.0,0.0,1.0);
  }
  glTranslatef((-tamX)/2 + avanceX,(-tamY)/2 + avanceY,camUpDown);
}

void cambioCamara() {
  Punto salida = (j.listaNiveles[nivelActual]).salida;
  if (camara == 1) {
    camara1();
  }
  else if (camara == 2) {
    camara2();
  } else if (camara == 3) {
    camara3();
  } else if (camara == 4) {
    // Vista de las 3+1
  } else {
    camaraA();
  }
  glRotatef(rot, 0.0,0.0,1.0);
}

void
datos_reshape(int w, int h)
{
  float aspectratio;
  if (h==0)
    h=1;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspectratio = (float) w / (float) h;
  if (w<=h) {
    glOrtho(-10, 10, -10/aspectratio, 10/aspectratio, 1.0, -1.0);
  }else{
    glOrtho(-10*aspectratio, 10*aspectratio, -10, 10, 1.0, -1.0);
  }

  /*
  float aspectratio;
  aspectratio = (float) w / (float) h;
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0f, 1, 1, 100.0);
  //gluPerspective(zoom, 1, 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
  */
  /*
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glDisable(GL_DEPTH_TEST);
  */
}

void decidirMap(){
  int salidaX = (int)(j.listaNiveles[nivelActual]).salida.getX();
  int jugadorX = (int)(j.listaNiveles[nivelActual]).j.ubicacionActual.getX();
  int salidaY = (int)(j.listaNiveles[nivelActual]).salida.getY();
  int jugadorY = (int)(j.listaNiveles[nivelActual]).j.ubicacionActual.getY();
  if (salidaX < jugadorX) {
    if (salidaY < jugadorY)
      glBindTexture(GL_TEXTURE_2D, texMapSO);
    else if (salidaY > jugadorY)
      glBindTexture(GL_TEXTURE_2D, texMapNO);
    else
      glBindTexture(GL_TEXTURE_2D, texMapO);
  } else if (salidaX > jugadorX){
    if (salidaY < jugadorY)
      glBindTexture(GL_TEXTURE_2D, texMapSE);
    else if (salidaY > jugadorY)
      glBindTexture(GL_TEXTURE_2D, texMapNE);
    else
      glBindTexture(GL_TEXTURE_2D, texMapE);
  } else {
    if (salidaY < jugadorY)
      glBindTexture(GL_TEXTURE_2D, texMapS);
    else if (salidaY > jugadorY)
      glBindTexture(GL_TEXTURE_2D, texMapN);
    else
      glBindTexture(GL_TEXTURE_2D, texMapHERE);
  }
}

void decidirVida() {
  int vidas = (j.listaNiveles[nivelActual]).j.vidas;
  if (vidas == 3) {
    glBindTexture(GL_TEXTURE_2D, texVida3);
  } else if (vidas == 2) {
    glBindTexture(GL_TEXTURE_2D, texVida2);
  } else if (vidas == 1) {
    glBindTexture(GL_TEXTURE_2D, texVida1);
  } else
    glBindTexture(GL_TEXTURE_2D, texVida0);
}

void
datos_display(void)
{
  float base = -45;
  glClearColor(0.0f, 0.0f, 0.0f ,1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  decidirMap();
  glPushMatrix();
  glTranslatef(base+45,-10,0);
  glBegin(GL_QUADS) ;
  glColor3f(0.0,1.0,1.0);glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0,0.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(1.0f, 0.0f);glVertex3f(45,0.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(1.0f, 1.0f);glVertex3f(45,10.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(0.0f, 1.0f);glVertex3f(0.0,10.0,0.0);
  glEnd();
  glPopMatrix();

  decidirVida();
  glPushMatrix();
  glTranslatef(base+45,0,0);  
  glBegin(GL_QUADS) ;
  glColor3f(0.0,1.0,1.0);glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0,0.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(1.0f, 0.0f);glVertex3f(45,0.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(1.0f, 1.0f);glVertex3f(45,10.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(0.0f, 1.0f);glVertex3f(0.0,10.0,0.0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texAcel);
  glTranslatef(base,-10,0);
  glBegin(GL_QUADS) ;
  glColor3f(0.0,1.0,1.0);glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0,0.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(1.0f, 0.0f);glVertex3f(45,0.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(1.0f, 1.0f);glVertex3f(45,20.0,0.0);
  glColor3f(0.0,1.0,1.0);glTexCoord2f(0.0f, 1.0f);glVertex3f(0.0,20.0,0.0);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(base+11,-10,0);
  glBegin(GL_TRIANGLES) ;
  glColor3f(1.0,0.0,0.0);glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0,0.0,-1.0);
  glColor3f(1.0,0.0,0.0);glTexCoord2f(1.0f, 0.0f);glVertex3f(3.0,0.0,-1.0);
  glColor3f(1.0,0.0,0.0);glTexCoord2f(1.0f, 1.0f);glVertex3f(1.5,5.0,-1.0);
  glEnd();
  glPopMatrix();



  glutPostRedisplay();
  glutSwapBuffers();
  glFlush ();
  return;
  /*
    float pos[4];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3ub(255, 255, 255);
    polygon_display();
    setfont("helvetica", 18);
    drawstr(10, 30, "GLfloat border_color[ ] = { ");
    drawstr(10, 100, "GLfloat env_color[ ] = { ");
    glColor3ub(0, 255, 128);
    setfont("helvetica", 12);
    drawstr(10, 90, "glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,"
        " border_color);");
    drawstr(10, 110, "glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, "
            "env_color);");

    glutSwapBuffers();
  */
}

/* Alarma para controlar Turbo */
void  SIGALRM_control (int signum) {
  j.listaNiveles[nivelActual].j.desactivarTurbo();
}

void endSD() {
  /* Tablero */
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texName);
  glCallList(base);
  glBindTexture(GL_TEXTURE_2D, texName2);
  glCallList(paredes);
  glDisable(GL_TEXTURE_2D);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
  //  glLightfv(GL_LIGHT0, GL_POSITION, difusa);
  //  glLightfv(GL_LIGHT0, GL_POSITION, especular);
  posicion[0] = (j.listaNiveles[nivelActual]).j.ubicacionActual.getX();
  posicion[1] = (j.listaNiveles[nivelActual]).j.ubicacionActual.getY();
  posicion[2] = 0.3;
  posicion[3] = 1;
  glLightfv(GL_LIGHT0, GL_POSITION, posicion);
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

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  /* Coordenadas del sistema */

  glLoadIdentity();
  cambioCamara();
  endSD();
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
  cout << w << endl;
  cout << h << endl;
  venMain_w = w;
  venMain_h = h;
  subVen_w = (venMain_w - (3*GAP))/2;
  subVen_h = (venMain_h - (3*GAP))/2;
  glutSetWindow (subMain);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glutSetWindow (subDato);
  glutPositionWindow ((venMain_w-venDato_w)/2,venMain_h-100);
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

    glutSetWindow (subDat2);
    glutPositionWindow ((venMain_w-venDato_w)/2,venMain_h-100);
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
  switch(key) {
  case GLUT_KEY_LEFT:
    angulo = -2;
    j.listaNiveles[nivelActual].j.dirIzquierda();
    break;
  case GLUT_KEY_RIGHT:
    angulo = 2;
    j.listaNiveles[nivelActual].j.dirDerecha();
    break;
  case GLUT_KEY_DOWN:
    angulo = -1;
    j.listaNiveles[nivelActual].j.dirAbajo();
    break;
  case GLUT_KEY_UP:
    angulo = 1;
    j.listaNiveles[nivelActual].j.dirArriba();
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

void subDisplay1() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camara1();
  glRotatef(rot, 0.0,0.0,1.0);
  endSD();
  return;
}

void subDisplay2() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camara2();
  glRotatef(rot, 0.0,0.0,1.0);
  endSD();
  return;
}

void subDisplay3() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camara3();
  glRotatef(rot, 0.0,0.0,1.0);
  endSD();
  return;
}

void subDisplay4() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camaraA();
  glRotatef(rot, 0.0,0.0,1.0);
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
  int iheight, iwidth;
  int iheight2, iwidth2;
  int pheight, pwidth;
  int gheight, gwidth;
  int pruebaH, pruebaW;
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  // Piso
  glGenTextures(1, &texName);
  glBindTexture(GL_TEXTURE_2D, texName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  image = glmReadPPM("texturas/imagen1.ppm", &iwidth, &iheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  // Pared
   glGenTextures(1, &texName2);
   glBindTexture(GL_TEXTURE_2D, texName2);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   image2 = glmReadPPM("texturas/imagen2.ppm", &iwidth2, &iheight2);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth2, iheight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
   
  // Aceleracion
   glGenTextures(1, &texAcel);
   glBindTexture(GL_TEXTURE_2D, texAcel);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   acel = glmReadPPM("texturas/acel.ppm", &gheight, &gwidth);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gheight, gwidth, 0, GL_RGB, GL_UNSIGNED_BYTE, acel);
   
  // Vida 0
  glGenTextures(1, &texVida0);
  glBindTexture(GL_TEXTURE_2D, texVida0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  vida1 = glmReadPPM("texturas/vida0.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, vida0);

  // Vida 1
  glGenTextures(1, &texVida1);
  glBindTexture(GL_TEXTURE_2D, texVida1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  vida1 = glmReadPPM("texturas/vida1.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, vida1);

  // Vida 2
  glGenTextures(1, &texVida2);
  glBindTexture(GL_TEXTURE_2D, texVida2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  vida2 = glmReadPPM("texturas/vida2.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, vida2);

  // Vida 3
  glGenTextures(1, &texVida3);
  glBindTexture(GL_TEXTURE_2D, texVida3);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  vida3 = glmReadPPM("texturas/vida3.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, vida3);

  // Mapa Here
  glGenTextures(1, &texMapHERE);
  glBindTexture(GL_TEXTURE_2D, texMapHERE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapHERE = glmReadPPM("texturas/metaHERE.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapHERE);

  // Mapa Hacia N
  glGenTextures(1, &texMapN);
  glBindTexture(GL_TEXTURE_2D, texMapN);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapN = glmReadPPM("texturas/metaN.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapN);

  // Mapa Hacia NO
  glGenTextures(1, &texMapNO);
  glBindTexture(GL_TEXTURE_2D, texMapNO);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapNO = glmReadPPM("texturas/metaNO.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapNO);

  // Mapa Hacia O
  glGenTextures(1, &texMapO);
  glBindTexture(GL_TEXTURE_2D, texMapO);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapO = glmReadPPM("texturas/metaO.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapO);
 
  // Mapa Hacia SO
  glGenTextures(1, &texMapSO);
  glBindTexture(GL_TEXTURE_2D, texMapSO);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapSO = glmReadPPM("texturas/metaSO.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapSO);

  // Mapa Hacia S
  glGenTextures(1, &texMapS);
  glBindTexture(GL_TEXTURE_2D, texMapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapS = glmReadPPM("texturas/metaS.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapS);

  // Mapa Hacia SE
  glGenTextures(1, &texMapSE);
  glBindTexture(GL_TEXTURE_2D, texMapSE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapSE = glmReadPPM("texturas/metaSE.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapSE);

  // Mapa Hacia E
  glGenTextures(1, &texMapE);
  glBindTexture(GL_TEXTURE_2D, texMapE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapE = glmReadPPM("texturas/metaE.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapE);

  // Mapa Hacia NE
  glGenTextures(1, &texMapNE);
  glBindTexture(GL_TEXTURE_2D, texMapNE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  mapNE = glmReadPPM("texturas/metaNE.ppm", &pwidth, &pheight);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGB, GL_UNSIGNED_BYTE, mapNE);


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
  ambiente = {0,0.7,0.7,1};
  difusa = {0.05*tamX, 0.05*tamY, 0.05, 1.0 };
  especular = {0.08*tamX, 0.08*tamY, 0.08, 1.0};
}

void initPosicion() {
  posicion = {0.5*tamX,0.5*tamY,0.3,0.0};
}

void initTamTablero(Juego j, int nivelActual) {
  tamX = j.listaNiveles[nivelActual].t.getAncho();
  tamY = j.listaNiveles[nivelActual].t.getLargo();
  tamZ = 3.0;
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
  if (subVen1 != 0 && subVen2 != 0 && subVen3 != 0 && subVen4 != 0 && subDat2 != 0) {
    glutDestroyWindow(subVen1);
    glutDestroyWindow(subVen2);
    glutDestroyWindow(subVen3);
    glutDestroyWindow(subVen4);
    glutDestroyWindow(subDat2);
    subVen1 = 0;
    subVen2 = 0;
    subVen3 = 0;
    subVen4 = 0;
    subDat2 = 0;
  }
}

void keyboard (unsigned char key, int x, int y)  {
  switch (key)
    {
    case 'G': case 'g':
      j.listaNiveles[nivelActual].j.setGo();
      j.listaNiveles[nivelActual].setGo();
      //      teclaTrasXDer();
      break;
    case 'N': case 'n':
      varX += 0.1;
      break;
    case 'M': case 'm':
      varX -= 0.1;
      break;
    case 'J': case 'j':
      varY += 0.1;
      break;
    case 'K': case 'k':
      varY -= 0.1;
      break;
    case 'I': case 'i':
      varZ += 0.1;
      break;
    case 'O': case 'o':
      varZ -= 0.1;
      break;
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

	subDat2 = glutCreateSubWindow(venMain,(venMain_w-venDato_w)/2,venMain_h-100,venDato_w,100);
	glutReshapeFunc(datos_reshape);
	glutDisplayFunc(datos_display);
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
    case 32: // TURBO !
      if (j.listaNiveles[nivelActual].j.getNumTurbo() > 0) {
        j.listaNiveles[nivelActual].j.activarTurbo();
        alarm(1);
      }
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
  glutSpecialFunc(flechas);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  subMain = glutCreateSubWindow(venMain,0,0,venMain_w,venMain_h);
  initVentana();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutSpecialFunc(flechas);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  /*Propiedades de openGL */
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
  // Binding de alarma
  signal (SIGALRM, SIGALRM_control);
  glutKeyboardFunc(keyboard);
  subDato = glutCreateSubWindow(venMain,0,venMain_h-100,venDato_w,100);
  initVentana();
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glClearDepth (1.0f);
  glClearColor(1.0,1.0,1.0,1.0f);
  glShadeModel(GL_SMOOTH);
  glutDisplayFunc(datos_display);
  glutReshapeFunc(datos_reshape);

  glutSpecialFunc(flechas);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  
  exit (EXIT_SUCCESS);
}
