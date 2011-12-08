#include <cstdlib>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include "include/TriMesh.h"
#include "include/TriMesh_algo.h"
#include "include/ModeloDatos.h"
#include "include/FreeImagePlus.h"

using namespace std;

TriMesh *themesh;
float maximo;
Coeficientes cf;

#define ANCHOF 5
#define LARGOF 5
#define ALTOF 5

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(-2.0,-2.0,-1.0,-2.0,0.0,0.0,0.0,1.0,0.0);
  
  glColor4f(1.0,0.0,0.0,0.5);
  
  //  glColor3f(0.0f, 0.0f, 0.0f); // sets color to black.
  glBegin(GL_QUAD_STRIP);

  glVertex2f(-0.5f, 0.25f); // left corner of the roof
  glVertex2f(0.5f, 0.25f); // right corner of the roof
  glVertex2f(-0.5f, -0.5f); // bottom left corner of the house
  glVertex2f(0.5f, -0.5f); //bottom right corner of the house
  glVertex3f(0.0f, 0.75f,-0.5); // top of the roof
  glVertex3f(1.0f, 0.75f,-0.5);
  glEnd();
  
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
  gluPerspective(100.0f, aspectratio, 0.5, 200.0);
  glMatrixMode(GL_MODELVIEW);
}

// double sumatoria(int columna, int n, int ancho, fipImage fp) {
//   double sum = 0.0;
//   RGBQUAD *palette = fp.getPalette();
//   for(int i = 0; i < ancho; i++) {
//     sum += palette[columna + ancho*i];  
//   }
//   return sum;
// }

// double *ajustarPaleta(fipImage fp) {
//   int nuevoAncho = fp.getWidth()/ANCHOF + fp.getWidth()%ANCHOF;
//   int nuevoLargo = fp.getHeight()/LARGOF + fp.getHeight()%LARGOF;
//   RGBQUAD *nuevaPaleta = (RGBQUAD *) malloc(sizeof(RGBQUAD)*
//                                             (nuevoAncho)*(nuevoLargo));

//   double *transformadaI = (double *) malloc(sizeof(double)*
//                                             fp.getWidth()*nuevoLargo);
//   // Transformación en i 
//   for(int j = 0; j < fp.getWidth(); j++) {
//     for(int i = 0; i < fp.getHeight()/LARGOF; i++) {
//       transformadaI[i + j*nuevoAncho] = 
//         sumatoria(j, fp.getHeight()/LARGOF, fp.getWidth())
//         / (double) (fp.getHeight()/LARGOF);
//     }
//   }
  
// }

int main(int argc, char *argv[]) {
  fipImage fp = fipImage();
  if (fp.load("./texturas/fractal.bmp"))
    cout << "Cargó" << endl;
  cout << "PaletteSize = " << fp.getPaletteSize()/8 << endl;
  cout << "Bitmap line = " << fp.getLine()/8 << endl;
  cout << "ImageWidth = " << fp.getWidth() << endl;
  cout << "ImageHeight = " << fp.getHeight() << endl;
  cout << "bitsPerPixel = " << fp.getBitsPerPixel() << endl;
  RGBQUAD * p = fp.getPalette();
  //  cout << "Red = " << (int) p[0].rgbRed << endl;
  
  // for(int i = 0; i < fp.getHeight(); i++) {
  //   for(int j = 0; j < fp.getWidth(); j++) {
  //     int r,g,b;
  //     r = (int) p[j + i*fp.getWidth()].rgbRed;
  //     g = (int) p[j + i*fp.getWidth()].rgbGreen;
  //     b = (int) p[j + i*fp.getWidth()].rgbBlue;
  //     // cout << "Red = " << r << endl;
  //     // cout << "Green = " << g << endl;
  //     // cout << "Blue = " << b << endl;
  //   }
  // }

  // /* Inicialización de ventana */
  // glutInit(&argc, argv);
  // glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
  // glutInitWindowSize (1280,800);
  // glutInitWindowPosition (100, 150);

  // glutCreateWindow (argv[0]);
  // /* Propiedades de openGL */
  // glEnable(GL_DEPTH_TEST);
  // glEnable( GL_LINE_SMOOTH );
  // glEnable( GL_POLYGON_SMOOTH );
  // glClearDepth (1.0f);
  // glClearColor(0.0,0.0,0.0,0.0f);
  // glShadeModel(GL_SMOOTH);

  // glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable( GL_BLEND );

  // /* Directivas para graficar */
  // glutReshapeFunc(reshape);
  // glutDisplayFunc(display);
  // glutMainLoop();

  return 0;
}

// Coeficientes coeficientesMaya(TriMesh *themesh) {
//   int i1;
//   int i2;
//   int i3;
//   float maximoX, maximoY, maximoZ;
//   maximoX = maximoY = maximoZ = 0.0; 
//   for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
//       it != themesh->faces.end();
//       ++it) {
//     i1 = (*it)[0];
//     i2 = (*it)[1];
//     i3 = (*it)[2];
//     maximoX = max(max(max(themesh->vertices[i1][0], themesh->vertices[i2][0]),
//                       themesh->vertices[i3][0]), maximoX);
//     maximoY = max(max(max(themesh->vertices[i1][1], themesh->vertices[i2][1]),
//                       themesh->vertices[i3][1]), maximoZ);
//     maximoZ = max(max(max(themesh->vertices[i1][2], themesh->vertices[i2][2]),
//                       themesh->vertices[i3][2]), maximoZ);
//   }
//   // double cX = 2.0 / maximoX;
//   // double cY = 2.0 / maximoY;
//   // double cZ = 2.0 / maximoZ;
//   // double cX = maximoX / 2.0;
//   // double cY = maximoY / 2.0;
//   // double cZ = maximoZ / 2.0;

//   double cX = 2.0 / maximoX;
//   double cY = 2.0 / maximoY;
//   double cZ = 2.0 / maximoZ;
  
//   // double cX = 20.0 / maximoX;
//   // double cY = 1.0 / maximoY;
//   // double cZ = 1.0 / maximoZ;
  
  
//     cout << "MaximoX = " << maximoX << endl;
//     cout << "MaximoY = " << maximoY << endl;
//     cout << "MaximoZ = " << maximoZ << endl;
//   Coeficientes c = Coeficientes(cX, cY, cZ);
//   return c;
// }

// void drawMesh() {
//   int i1;
//   int i2;
//   int i3;
//   glColor3f(1.0,0.0,0.0);
//   glBegin(GL_TRIANGLES);
//   for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
//       it != themesh->faces.end();
//       ++it) {
//     i1 = (*it)[0];
//     i2 = (*it)[1];
//     i3 = (*it)[2];
//     glVertex3f(themesh->vertices[i1][0],themesh->vertices[i1][1],
//                themesh->vertices[i1][2]);
//     glVertex3f(themesh->vertices[i2][0], themesh->vertices[i2][1],
//                themesh->vertices[i2][2]);
//     glVertex3f(themesh->vertices[i3][0], themesh->vertices[i3][1],
//                themesh->vertices[i3][2]);
//     maximo = max(max(max(themesh->vertices[i1][2], themesh->vertices[i2][2]), themesh->vertices[i3][2]), maximo);
//   }
//   glEnd();
// }

// void drawMesh(TriMesh *themesh, Coeficientes cf) {
//   int i1;
//   int i2;
//   int i3;
//   glColor3f(1.0,0.0,0.0);
//   glBegin(GL_TRIANGLES);
//   for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
//       it != themesh->faces.end();
//       ++it) {
//     i1 = (*it)[0];
//     i2 = (*it)[1];
//     i3 = (*it)[2];
//     glVertex3f(themesh->vertices[i1][0]*cf.cX,themesh->vertices[i1][1]*cf.cY,
//                themesh->vertices[i1][2]*cf.cZ);
//     glVertex3f(themesh->vertices[i2][0]*cf.cX, themesh->vertices[i2][1]*cf.cY,
//                themesh->vertices[i2][2]*cf.cZ);
//     glVertex3f(themesh->vertices[i3][0]*cf.cX, themesh->vertices[i3][1]*cf.cY,
//                themesh->vertices[i3][2]*cf.cZ);
//   }
//   glEnd();
// }


// void display(void) {
//   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();

//   //  glColor3f(1.0,0.0,0.0);
//   // glTranslatef(0.0,0.0,-1.5);
//   // glScalef(0.2,0.2,0.2);
//   // drawMesh(themesh,cf);

//   gluLookAt(0.0,-1.0,-1.0,-1.0,0.0,0.0,0.0,1.0,0.0);
//   // cout << "CfX = " << cf.cX <<  endl; 
//   // cout << "CfY = " << cf.cY <<  endl; 
//   // cout << "CfZ = " << cf.cZ <<  endl; 
  
//   glColor4f(1.0,0.0,0.0,0.5);
//   glutSolidCube(1.0);
  
//   //  drawMesh();
//   glutSwapBuffers();
//   glFlush ();
//   return;
// }

// void reshape (int w, int h) {
//   float aspectratio;
//   aspectratio = (float) w / (float) h;
//   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
//   glMatrixMode (GL_PROJECTION);
//   glLoadIdentity();
//   gluPerspective(100.0f, aspectratio, 0.5, 200.0);
//   glMatrixMode(GL_MODELVIEW);
// }

// int main (int argc, char **argv) {
//   /* Inicialización de ventana */
//   glutInit(&argc, argv);
//   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
//   glutInitWindowSize (1280,800);
//   glutInitWindowPosition (100, 150);

//   /* Cargar figura .obj */

//   const char *filename = argv[1];
//   themesh = TriMesh::read(filename);
//   themesh->need_faces();
//   //  scale(themesh, 0.1, 0.1,0.1);
//   cf = coeficientesMaya(themesh);

//   glutCreateWindow (argv[0]);
//   /* Propiedades de openGL */
//   glEnable(GL_DEPTH_TEST);
//   glEnable( GL_LINE_SMOOTH );
//   glEnable( GL_POLYGON_SMOOTH );
//   glClearDepth (1.0f);
//   glClearColor(0.0,0.0,0.0,0.0f);
//   glShadeModel(GL_SMOOTH);

//   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
//   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   glEnable( GL_BLEND );

//   /* Directivas para graficar */
//   glutReshapeFunc(reshape);
//   glutDisplayFunc(display);
//   glutMainLoop();

//   exit (EXIT_SUCCESS);
// }


