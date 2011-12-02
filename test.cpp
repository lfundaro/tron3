#include <cstdlib>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include "include/TriMesh.h"
//#include "include/Vec.h"
//#include "include/Color.h"

using namespace std;

TriMesh *themesh;
float maximo;

void drawMesh() {
  int i1;
  int i2;
  int i3;
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_TRIANGLES);
  for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
      it != themesh->faces.end();
      ++it) {
    i1 = (*it)[0];
    i2 = (*it)[1];
    i3 = (*it)[2];
    glVertex3f(themesh->vertices[i1][0],themesh->vertices[i1][1],
               themesh->vertices[i1][2]);
    glVertex3f(themesh->vertices[i2][0], themesh->vertices[i2][1],
               themesh->vertices[i2][2]);
    glVertex3f(themesh->vertices[i3][0], themesh->vertices[i3][1],
               themesh->vertices[i3][2]);
    //    cout << themesh->vertices[i3][2] << endl;
    maximo = max(max(max(themesh->vertices[i1][2], themesh->vertices[i2][2]), themesh->vertices[i3][2]), maximo);
  }
  glEnd();
}

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //  glColor3f(1.0,0.0,0.0);
  glTranslatef(0.0,0.0,-maximo*2);
  glRotatef(90.0,1.0,0.0,0.0);
  //  glutSolidCube(1.0);
 
  drawMesh();

  cout << maximo << endl;
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
  gluPerspective(90.0f, aspectratio, 0.5, 200.0);
  glMatrixMode(GL_MODELVIEW);
}

int main (int argc, char **argv) {
  /* Inicialización de ventana */
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
  glutInitWindowSize (1280,800);
  glutInitWindowPosition (100, 150);

  /* Cargar figura .obj */

  const char *filename = argv[1];
  themesh = TriMesh::read(filename);
  themesh->need_faces();

  glutCreateWindow (argv[0]);
  /* Propiedades de openGL */
  glEnable(GL_DEPTH_TEST);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glClearDepth (1.0f);
  glClearColor(0.0,0.0,0.0,0.0f);
  glShadeModel(GL_SMOOTH);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  /* Directivas para graficar */
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMainLoop();

  exit (EXIT_SUCCESS);
}


