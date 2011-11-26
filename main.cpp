#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include "include/TriMesh.h"
#include "include/XForm.h"
#include "include/TriMesh_algo.h"

using namespace std;


vector<TriMesh *> meshes;
vector<xform> xforms;
vector<bool> visible;
vector<string> filenames;
TriMesh::BSphere global_bsph;
xform global_xf;
int current_mesh = -1;


void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glColor3f(1.0,0.0,0.0);
  glutSolidCube(2.0);

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
  gluPerspective(200.0f, aspectratio, 0.5, 200.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    
  /* Inicialización de ventana */
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
  glutInitWindowSize (1280,800);
  glutInitWindowPosition (100, 150);
  
  // Cargar figura .ply 
  
  const char *filename = argv[2];
  TriMesh *themesh = TriMesh::read(filename);
  themesh->need_normals();
  themesh->need_tstrips();
  themesh->need_bsphere();
  meshes.push_back(themesh);
  xforms.push_back(xform());
  visible.push_back(true);
  filenames.push_back(filename);

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

  return 0;
}

