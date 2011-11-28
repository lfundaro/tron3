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

using namespace std;

/* INICIO Variables globales */
Juego j;
float tamX = 100.0;
float tamY = 100.0;
float tamZ = 2.0;
// float tamX = 100.0;
// float tamY = 100.0;
float giroH = 0;
float giroV = 0;
int mouVenX = 0;
int mouVenY = 0;
double mouJueX = tamX/2;
double mouJueY = tamY/2;
double mouJueZ = 1;
int nivelActual = 0;
Punto posActualJugador;
vector<Punto> posContrincante;
double incr = 0.001;
GLfloat ambiente[] = { 0.01*tamX, 0.02*tamY, 0.02, 1.0 };
GLfloat difusa[] = { 0.05*tamX, 0.05*tamY, 0.05, 1.0 };
GLfloat especular[] = { 0.08*tamX, 0.08*tamY, 0.08, 1.0 };
GLfloat posicion[] = { 0.5*tamX,0.5*tamY,0.3,0.0};


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
/* FIN Variables para display list */

/* FIN Variables globales */

void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  /* Coordenad=as del sistema */
  glLoadIdentity();
  cam.go(tamX, tamY, giroH, giroV, avanceX, avanceY, camUpDown);

  //gluLookAt (0.0, (-x)/10.0,(-x)/10*6, 0.0,(-y)/20.0,(-x)/3.0, 0.0, 0.0,-1.0);
  //  glRotatef(rot, 0.0,0.0,1.0);

  /* Tablero */
  //  dibujarMira(mouJueX,mouJueY,1.0,0.0,0.0);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D, texName);

  glCallList(base);

  glBindTexture(GL_TEXTURE_2D, texName2);
  dibujarParedes(tamX,tamY,tamZ);
  glDisable(GL_TEXTURE_2D);

  glLightfv(GL_LIGHT0, GL_POSITION, ambiente);
  glLightfv(GL_LIGHT0, GL_POSITION, difusa);
  glLightfv(GL_LIGHT0, GL_POSITION, especular);
  glLightfv(GL_LIGHT0, GL_POSITION, posicion);
  dibujarTablero(tamX,tamY,tamZ);

  //  redraw();
  // Dibujar trayectoria de Jugador
  j.listaNiveles[nivelActual].j.dibujarTrayectoriaJ();
  j.listaNiveles[nivelActual].dibujarTrayectoriaC();
  // Dibujar jugadores en posición inicial
  j.listaNiveles[nivelActual].dibujarJugadores();
  // Dibujar obstáculos
  j.listaNiveles[nivelActual].dibujarObstaculos();
  // Dibujar discos
  j.listaNiveles[nivelActual].dibujarDiscos();
  glutPostRedisplay();
  glutSwapBuffers();
  glFlush ();
  return;

  // glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  
  // //  gluLookAt(0.0, -10.2, 61.2, 0.0, -5.1,  -34.0, 0.0, 1.0, 0.0);
  // //	 eyeX , eyeY , eyeZ , centerX , centerY , centerZ , upX , upY , upZ 
  // gluLookAt(0.0,0.0,0.5,0.0,0.0,0.0,0.0,1.0,0.0);
  
  // //  dibujarBase();
  // glColor3f(1.0,0.0,0.0);
  // glutSolidCube(1.0);
  // //  redraw();

  // glutSwapBuffers();
  // glFlush ();
  // return;
}

// Idle callback
void idle()
{
  glutPostRedisplay();
}

static void init(void) {
  base = glGenLists(1);
  glNewList(base, GL_COMPILE);
  dibujarBase(tamX, tamY);
  glEndList();
  return;
}

void reshape (int w, int h) {
  float aspectratio;
  aspectratio = (float) w / (float) h;
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(200.0, aspectratio, 0.5, 100.0);
  //  glFrustum(-91.1373, 91.1373, -51.6207, 51.6207,299.26, 448.891);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    
  /* Inicialización de ventana */
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB |  GLUT_DEPTH);
  glutInitWindowSize (1280,800);
  glutInitWindowPosition (100, 150);

  /* Abrir archivo e inicializar estructuras de juego */
  char *archivo = argv[1];
  j = parse(archivo);
  /* Límites del juego */
  for(int i = 0; i < j.numNiveles; i++)
    {
      j.listaNiveles[i].setLimite((double) tamX, (double) tamY);
    }
  
  // Cargar figura .ply 
  
  // const char *filename = argv[1];
  // TriMesh *themesh = TriMesh::read(filename);
  // themesh->need_normals();
  // themesh->need_tstrips();
  // themesh->need_bsphere();
  // meshes.push_back(themesh);
  // xforms.push_back(xform());
  // visible.push_back(true);
  // filenames.push_back(filename);

  glutCreateWindow (argv[0]);
  init();

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
  //  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
  
  //  resetview();
  
  glutMainLoop();

  return 0;
}



































// // Globals
// vector<TriMesh *> meshes;
// vector<xform> xforms;
// vector<bool> visible;
// vector<string> filenames;
// TriMesh::BSphere global_bsph;
// xform global_xf;
// GLCamera camera;

// int current_mesh = -1;

// bool draw_edges = false;
// bool draw_2side = false;
// bool draw_shiny = true;
// bool draw_lit = true;
// bool draw_falsecolor = false;
// bool draw_index = false;
// bool white_bg = false;
// int point_size = 1,line_width = 1;

// /* INICIO Código Trimesh */

// // Make some mesh current
// void set_current(int i)
// {
// 	if (i >= 0 && i < meshes.size() && visible[i])
// 		current_mesh = i;
// 	else
// 		current_mesh = -1;
// 	camera.stopspin();
// }


// // Change visiblility of a mesh
// void toggle_vis(int i)
// {
// 	if (i >= 0 && i < meshes.size())
// 		visible[i] = !visible[i];
// 	if (current_mesh == i && !visible[i])
// 		set_current(-1);
// }


// // Signal a redraw
// void need_redraw()
// {
// 	glutPostRedisplay();
// }


// // Clear the screen
// void cls()
// {
// 	glDisable(GL_DITHER);
// 	glDisable(GL_BLEND);
// 	glDisable(GL_DEPTH_TEST);
// 	glDisable(GL_NORMALIZE);
// 	glDisable(GL_LIGHTING);
// 	glDisable(GL_NORMALIZE);
// 	glDisable(GL_COLOR_MATERIAL);
// 	if (white_bg)
// 		glClearColor(1, 1, 1, 0);
// 	else
// 		glClearColor(0.08, 0.08, 0.08, 0);
// 	glClearDepth(1);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// }


// // Set up lights and materials
// void setup_lighting(int id)
// {
// 	Color c(1.0f);
// 	if (draw_falsecolor)
// 		c = Color::hsv(-3.88 * id, 0.6 + 0.2 * sin(0.42 * id), 1.0);
// 	glColor3fv(c);

// 	if (!draw_lit || meshes[id]->normals.empty()) {
// 		glDisable(GL_LIGHTING);
// 		return;
// 	}

// 	GLfloat mat_specular[4] = { 0.18, 0.18, 0.18, 0.18 };
// 	if (!draw_shiny) {
// 		mat_specular[0] = mat_specular[1] =
// 		mat_specular[2] = mat_specular[3] = 0.0f;
// 	}
// 	GLfloat mat_shininess[] = { 64 };
// 	GLfloat global_ambient[] = { 0.02, 0.02, 0.05, 0.05 };
// 	GLfloat light0_ambient[] = { 0, 0, 0, 0 };
// 	GLfloat light0_diffuse[] = { 0.85, 0.85, 0.8, 0.85 };
// 	if (current_mesh >= 0 && id != current_mesh) {
// 		light0_diffuse[0] *= 0.5f;
// 		light0_diffuse[1] *= 0.5f;
// 		light0_diffuse[2] *= 0.5f;
// 	}
// 	GLfloat light1_diffuse[] = { -0.01, -0.01, -0.03, -0.03 };
// 	GLfloat light0_specular[] = { 0.85, 0.85, 0.85, 0.85 };
// 	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
// 	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
// 	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
// 	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
// 	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
// 	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
// 	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
// 	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, draw_2side);
// 	glEnable(GL_LIGHTING);
// 	glEnable(GL_LIGHT0);
// 	glEnable(GL_LIGHT1);
// 	glEnable(GL_COLOR_MATERIAL);
// 	glEnable(GL_NORMALIZE);
// }


// // Draw triangle strips.  They are stored as length followed by values.
// void draw_tstrips(const TriMesh *themesh)
// {
// 	static bool use_glArrayElement = false;
// 	static bool tested_renderer = false;
// 	if (!tested_renderer) {
// 		use_glArrayElement = !!strstr(
// 			(const char *) glGetString(GL_RENDERER), "Intel");
// 		tested_renderer = true;
// 	}

// 	const int *t = &themesh->tstrips[0];
// 	const int *end = t + themesh->tstrips.size();
// 	if (use_glArrayElement) {
// 		while (likely(t < end)) {
// 			glBegin(GL_TRIANGLE_STRIP);
// 			int striplen = *t++;
// 			for (int i = 0; i < striplen; i++)
// 				glArrayElement(*t++);
// 			glEnd();
// 		}
// 	} else {
// 		while (likely(t < end)) {
// 			int striplen = *t++;
// 			glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_INT, t);
// 			t += striplen;
// 		}
// 	}
// }


// // Draw the mesh
// void draw_mesh(int i)
// {
// 	const TriMesh *themesh = meshes[i];

// 	glPushMatrix();
// 	glMultMatrixd(xforms[i]);

// 	glDepthFunc(GL_LESS);
// 	glEnable(GL_DEPTH_TEST);

// 	if (draw_2side) {
// 		glDisable(GL_CULL_FACE);
// 	} else {
// 		glCullFace(GL_BACK);
// 		glEnable(GL_CULL_FACE);
// 	}

// 	// Vertices
// 	glEnableClientState(GL_VERTEX_ARRAY);
// 	glVertexPointer(3, GL_FLOAT,
// 			sizeof(themesh->vertices[0]),
// 			&themesh->vertices[0][0]);

// 	// Normals
// 	if (!themesh->normals.empty() && !draw_index) {
// 		glEnableClientState(GL_NORMAL_ARRAY);
// 		glNormalPointer(GL_FLOAT,
// 				sizeof(themesh->normals[0]),
// 				&themesh->normals[0][0]);
// 	} else {
// 		glDisableClientState(GL_NORMAL_ARRAY);
// 	}

// 	// Colors
// 	if (!themesh->colors.empty() && !draw_falsecolor && !draw_index) {
// 		glEnableClientState(GL_COLOR_ARRAY);
// 		glColorPointer(3, GL_FLOAT,
// 			       sizeof(themesh->colors[0]),
// 			       &themesh->colors[0][0]);
// 	} else {
// 		glDisableClientState(GL_COLOR_ARRAY);
// 	}

// 	// Main drawing pass
// 	if (themesh->tstrips.empty()) {
// 		// No triangles - draw as points
// 		glPointSize(point_size);
// 		glDrawArrays(GL_POINTS, 0, themesh->vertices.size());
// 		glPopMatrix();
// 		return;
// 	}

// 	if (draw_edges) {
// 		glPolygonOffset(10.0f, 10.0f);
// 		glEnable(GL_POLYGON_OFFSET_FILL);
// 	}

// 	draw_tstrips(themesh);
// 	glDisable(GL_POLYGON_OFFSET_FILL);

// 	// Edge drawing pass
// 	if (draw_edges) {
// 		glPolygonMode(GL_FRONT, GL_LINE);
// 		glLineWidth(line_width);
// 		glDisableClientState(GL_COLOR_ARRAY);
// 		glDisable(GL_COLOR_MATERIAL);
// 		GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
// 		GLfloat light0_diffuse[] = { 0.8, 0.8, 0.8, 0.0 };
// 		GLfloat light1_diffuse[] = { -0.2, -0.2, -0.2, 0.0 };
// 		GLfloat light0_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
// 		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
// 		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
// 		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
// 		glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
// 		GLfloat mat_diffuse[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
// 		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
// 		glColor3f(0, 0, 1); // Used iff unlit
// 		draw_tstrips(themesh);
// 		glPolygonMode(GL_FRONT, GL_FILL);
// 	}

// 	glPopMatrix();
// }


// // Draw the scene
// void redraw()
// {
// 	timestamp t = now();
//     //    camera.setupGL(global_xf * global_bsph.center, global_bsph.r);
// 	glPushMatrix();
// 	glMultMatrixd(global_xf);
//     //    cls();
// 	for (int i = 0; i < meshes.size(); i++) {
// 		if (!visible[i])
// 			continue;
//         setup_lighting(i);
// 		draw_mesh(i);
// 	}

// 	glPopMatrix();
//     //	glutSwapBuffers();
// 	printf("\r                        \r%.1f msec.", 1000.0f * (now() - t));
//     //	fflush(stdout);
// }


// // Update global bounding sphere.
// void update_bsph()
// {
// 	point boxmin(1e38, 1e38, 1e38);
// 	point boxmax(-1e38, -1e38, -1e38);
// 	bool some_vis = false;
// 	for (int i = 0; i < meshes.size(); i++) {
// 		if (!visible[i])	
// 			continue;
// 		some_vis = true;
// 		point c = xforms[i] * meshes[i]->bsphere.center;
// 		float r = meshes[i]->bsphere.r;
// 		for (int j = 0; j < 3; j++) {
// 			boxmin[j] = min(boxmin[j], c[j]-r);
// 			boxmax[j] = max(boxmax[j], c[j]+r);
// 		}
// 	}
// 	if (!some_vis)
// 		return;
// 	point &gc = global_bsph.center;
// 	float &gr = global_bsph.r;
// 	gc = 0.5f * (boxmin + boxmax);
// 	gr = 0.0f;
// 	for (int i = 0; i < meshes.size(); i++) {
// 		if (!visible[i])	
// 			continue;
// 		point c = xforms[i] * meshes[i]->bsphere.center;
// 		float r = meshes[i]->bsphere.r;
// 		gr = max(gr, dist(c, gc) + r);
// 	}
// }


// // Set the view...
// void resetview()
// {
// 	camera.stopspin();

// 	// Reload mesh xforms
// 	for (int i = 0; i < meshes.size(); i++)
// 		if (!xforms[i].read(xfname(filenames[i])))
// 			xforms[i] = xform();

// 	update_bsph();

// 	// Set camera to first_mesh.camxf if we have it, else default
// 	if (!global_xf.read(replace_ext(filenames[0], "camxf")))
// 		global_xf = xform::trans(0, 0, -5.0f * global_bsph.r) *
// 			    xform::trans(-global_bsph.center);
// }


// // Save the current image to a PPM file.
// // Uses the next available filename matching filenamepattern
// void dump_image()
// {
// 	// Find first non-used filename
// 	const char filenamepattern[] = "img%d.ppm";
// 	int imgnum = 0;
// 	FILE *f;
// 	while (1) {
// 		char filename[1024];
// 		sprintf(filename, filenamepattern, imgnum++);
// 		f = fopen(filename, "rb");
// 		if (!f) {
// 			f = fopen(filename, "wb");
// 			printf("\n\nSaving image %s... ", filename);
// 			fflush(stdout);
// 			break;
// 		}
// 		fclose(f);
// 	}

// 	// Read pixels
// 	GLint V[4];
// 	glGetIntegerv(GL_VIEWPORT, V);
// 	GLint width = V[2], height = V[3];
// 	char *buf = new char[width*height*3];
// 	glPixelStorei(GL_PACK_ALIGNMENT, 1);
// 	glReadPixels(V[0], V[1], width, height, GL_RGB, GL_UNSIGNED_BYTE, buf);

// 	// Flip top-to-bottom
// 	for (int i = 0; i < height/2; i++) {
// 		char *row1 = buf + 3 * width * i;
// 		char *row2 = buf + 3 * width * (height - 1 - i);
// 		for (int j = 0; j < 3 * width; j++)
// 			swap(row1[j], row2[j]);
// 	}

// 	// Write out file
// 	fprintf(f, "P6\n#\n%d %d\n255\n", width, height);
// 	fwrite(buf, width*height*3, 1, f);
// 	fclose(f);
// 	delete [] buf;

// 	printf("Done.\n\n");
// }


// // Save scan transforms
// void save_xforms()
// {
// 	for (int i = 0; i < xforms.size(); i++) {
// 		string xffile = xfname(filenames[i]);
// 		printf("Writing %s\n", xffile.c_str());
// 		xforms[i].write(xffile);
// 	}
// }


// // Save camera xform
// void save_cam_xform()
// {
// 	std::string camfile = replace_ext(filenames[0], "camxf");
// 	printf("Writing %s\n", camfile.c_str());
// 	global_xf.write(camfile);
// }


// // ICP
// void do_icp(int n)
// {
// 	camera.stopspin();

// 	if (current_mesh < 0 || current_mesh >= meshes.size())
// 		return;
// 	if (n < 0 || n >= meshes.size())
// 		return;
// 	if (!visible[n] || !visible[current_mesh] || n == current_mesh)
// 		return;
// 	ICP(meshes[n], meshes[current_mesh], xforms[n], xforms[current_mesh], 2);
// 	update_bsph();
// 	need_redraw();
// }


// // Handle mouse button and motion events
// static unsigned buttonstate = 0;

// void doubleclick(int button, int x, int y)
// {
// 	// Render and read back ID reference image
//   camera.setupGL(global_xf * global_bsph.center, global_bsph.r);
//   glDisable(GL_BLEND);
// 	glDisable(GL_LIGHTING);
// 	glClearColor(1,1,1,1);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glEnable(GL_DEPTH_TEST);
// 	draw_index = true;
// 	glPushMatrix();
// 	glMultMatrixd(global_xf);
// 	for (int i = 0; i < meshes.size(); i++) {
// 		if (!visible[i])
// 			continue;
// 		glColor3ub((i >> 16) & 0xff,
// 			   (i >> 8)  & 0xff,
// 			    i        & 0xff);
// 		draw_mesh(i);
// 	}
// 	glPopMatrix();
// 	draw_index = false;
// 	GLint V[4];
// 	glGetIntegerv(GL_VIEWPORT, V);
// 	y = int(V[1] + V[3]) - 1 - y;
// 	unsigned char pix[3];
// 	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pix);
// 	int n = (pix[0] << 16) + (pix[1] << 8) + pix[2];

// 	if (button == 0 || buttonstate == (1 << 0)) {
// 		// Double left click - select a mesh
// 		set_current(n);
// 	} else if (button == 2 || buttonstate == (1 << 2)) {
// 		// Double right click - ICP current to clicked-on
// 		do_icp(n);
// 	}
// }

// // Idle callback
// void idle()
// {
//   glutPostRedisplay();
// }

// /* FIN Código Trimesh */

// // float zoom = 90.0;
// // #define ZOOM_FACTOR 1.0

// // void teclaZoomIn()
// // {
// //   glMatrixMode(GL_PROJECTION);
// //   glLoadIdentity();
// //   zoom -= ZOOM_FACTOR;
// //   //  gluPerspective(zoom, 1, 0.5, 100.0);
// //   glFrustum(-91.1373, 91.1373, -51.6207, 51.6207,299.26 - zoom, 448.891);
// //   glMatrixMode(GL_MODELVIEW);
// // }

// // void teclaZoomOut()
// // {
// //   glMatrixMode(GL_PROJECTION);
// //   glLoadIdentity();
// //   zoom += ZOOM_FACTOR;
// //   //  gluPerspective(zoom, 1, 0.5, 100.0);
// //   glFrustum(-91.1373, 91.1373, -51.6207, 51.6207,299.26 + zoom, 448.891);
// //   glMatrixMode(GL_MODELVIEW);
// // }

// // void keyboard (unsigned char key, int x, int y) {
// //   switch (key)
// //     {
// //     case 'I': case 'i':
// //       teclaZoomIn();
// //       break;
// //     case 'O': case 'o':
// //       teclaZoomOut();
// //       break;
// //     default:
// //       printf("Didnt match\n");
// //       break;
// //     }
// //   return;
// // }


