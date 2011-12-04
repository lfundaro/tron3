#include "include/ModeloDatos.h"

using namespace std;

// Getters y setters de trayectoria
double Trayectoria::getVelocidad() 
{
  return velocidad;
}

int Trayectoria::getNumPuntos()
{
  return numPuntos;
}

// Getters y setters de Punto
double Punto::getX()
{
  return x;
}

double Punto::getY()
{
  return y;
}

// Setters de punto
void Punto::setX(double nuevoVal)
{
  x = nuevoVal;
  return;
}

void Punto::setY(double nuevoVal)
{
  y = nuevoVal;
  return;
}

void Trayectoria::actLambdaX(double val)
{
  lambdaX += val;
  return;
}

void Trayectoria::actLambdaY(double val)
{
  lambdaY += val;
  return;
}

double Trayectoria::normalizarD(double desplazamiento)
{
  double d =sqrt(pow(listaPuntos[(origen + 1) % numPuntos].getX() - 
                     listaPuntos[origen].getX(),2) +
                 pow(listaPuntos[(origen + 1) % numPuntos].getY() - 
                     listaPuntos[origen].getY(),2));
  return velocidad / d;
}

double Trayectoria::calcDesp(double velocidad)
{
  double desp = velocidad*1.0/60.0;
  time_t now;
  time(&now);
  double difft = difftime(now,timeStamp);
  if (difft >= 1.0) 
    {
      desp = velocidad*(difft);
      time(&timeStamp);
    }
  return desp;
}

void Trayectoria::ecuacionRecta(Punto *posActual)
{
  double desplazamiento = calcDesp(velocidad/*10.0*/);
  if (desplazamiento > 0.0) {
    double norm = normalizarD(desplazamiento);
    double y_prima = listaPuntos[origen].getY() + (lambdaY + norm)*
      (listaPuntos[(origen + 1) % numPuntos].getY() - listaPuntos[origen].getY()); 
    actLambdaY(norm);
    double x_prima = listaPuntos[origen].getX() + (lambdaX + norm)*
      (listaPuntos[(origen + 1) % numPuntos].getX() - listaPuntos[origen].getX()); 
    actLambdaX(norm);
    posActual->setX(x_prima);
    posActual->setY(y_prima);
  }
  return;
}

int Trayectoria::cambiarOrigen()
{
  if (lambdaX >= 1.0 || lambdaY >= 1.0) 
    {
      origen = (origen + 1) % numPuntos;
      return 1;
    }
  return 0;
}

void Trayectoria::calcularNuevaPosicion(Punto *posActual)
{
  ecuacionRecta(posActual);
  return;
}

void Trayectoria::lambdaReset()
{
  lambdaX = 0.0;
  lambdaY = 0.0;
  return;
}

void Trayectoria::calcularTrayectoria(Punto *posActual)
{
  if (cambiarOrigen()) {lambdaReset();}
  calcularNuevaPosicion(posActual);
  return;
}

void drawMesh(TriMesh *themesh, Coeficientes cf) {
  int i1;
  int i2;
  int i3;
  glColor3f(1.0,0.0,0.0);
  glRotatef(-90.0,0.0,0.0,1.0);
  glBegin(GL_TRIANGLES);
  for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
      it != themesh->faces.end();
      ++it) {
    i1 = (*it)[0];
    i2 = (*it)[1];
    i3 = (*it)[2];
    glVertex3f(themesh->vertices[i1][0]*cf.cX,themesh->vertices[i1][1]*cf.cY,
               themesh->vertices[i1][2]*cf.cZ);
    glVertex3f(themesh->vertices[i2][0]*cf.cX, themesh->vertices[i2][1]*cf.cY,
               themesh->vertices[i2][2]*cf.cZ);
    glVertex3f(themesh->vertices[i3][0]*cf.cX, themesh->vertices[i3][1]*cf.cY,
               themesh->vertices[i3][2]*cf.cZ);
  }
  glEnd();
}

Coeficientes coeficientesMayaJugadores(TriMesh *themesh) {
  int i1;
  int i2;
  int i3;
  float maximoX, maximoY, maximoZ;
  maximoX = maximoY = maximoZ = 0.0; 
  for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
      it != themesh->faces.end();
      ++it) {
    i1 = (*it)[0];
    i2 = (*it)[1];
    i3 = (*it)[2];
    maximoX = max(max(max(themesh->vertices[i1][0], themesh->vertices[i2][0]),
                      themesh->vertices[i3][0]), maximoX);
    maximoY = max(max(max(themesh->vertices[i1][1], themesh->vertices[i2][1]),
                      themesh->vertices[i3][1]), maximoZ);
    maximoZ = max(max(max(themesh->vertices[i1][2], themesh->vertices[i2][2]),
                      themesh->vertices[i3][2]), maximoZ);
  }
  double cX = 5.0 / maximoX;
  double cY = 5.0 / maximoY;
  double cZ = 5.0 / maximoZ;
  Coeficientes c = Coeficientes(cX, cY, cZ);
  return c;
}

Coeficientes coeficientesMayaObjetos(TriMesh *themesh) {
  int i1;
  int i2;
  int i3;
  float maximoX, maximoY, maximoZ;
  maximoX = maximoY = maximoZ = 0.0; 
  for(vector<TriMesh::Face>::iterator it = themesh->faces.begin();
      it != themesh->faces.end();
      ++it) {
    i1 = (*it)[0];
    i2 = (*it)[1];
    i3 = (*it)[2];
    maximoX = max(max(max(themesh->vertices[i1][0], themesh->vertices[i2][0]),
                      themesh->vertices[i3][0]), maximoX);
    maximoY = max(max(max(themesh->vertices[i1][1], themesh->vertices[i2][1]),
                      themesh->vertices[i3][1]), maximoZ);
    maximoZ = max(max(max(themesh->vertices[i1][2], themesh->vertices[i2][2]),
                      themesh->vertices[i3][2]), maximoZ);
  }
  double cX = 2.0 / maximoX;
  double cY = 2.0 / maximoY;
  double cZ = 2.0 / maximoZ;
  Coeficientes c = Coeficientes(cX, cY, cZ);
  return c;
}


void Jugador::dirIzquierda() {
  dir = IZQUIERDA;
}

void Jugador::dirDerecha() {
  dir = DERECHA;
}

void Jugador::dirArriba() {
  dir = ARRIBA;
}

void Jugador::dirAbajo() {
  dir = ABAJO;
}

void Contrincante::setGo() {
  go = 1;
}

void Jugador::setGo() {
  go = 1;
}

void Jugador::activarTurbo() {
  velocidad += velocidadTurbo;
  turbo--;
}

void Jugador::desactivarTurbo() {
  velocidad -= velocidadTurbo;
}

int Jugador::getNumTurbo() {
  return turbo;
}

double Jugador::incrementarVel() {
  if (velocidadIncremental >= velocidad) return velocidad;
  velocidadIncremental += velocidadIncremental;
  return velocidadIncremental;
}

void Nivel::setGo() {
  for(vector<Contrincante>::iterator it = listaContrincantes.begin();
      it != listaContrincantes.end(); ++it) {
    (*it).setGo();
  }
}

void Jugador::dibujarJugador() {
  glPushMatrix();
  // Punto de origen -- Punto destino ?
  //  t.calcularTrayectoria(&posActual);
  //  glColor3f(1.0,0.0,0.0);
  glScalef(0.2,0.2,0.2);
  // Actualizar desplazamiento del jugador
  // Juego paralizado ?
  if (go) {
    switch (dir) {
    case ARRIBA:
      ubicacionActual.setY(ubicacionActual.getY() + incrementarVel());
      break;
    case ABAJO:
      ubicacionActual.setY(ubicacionActual.getY() - incrementarVel());
      break;
    case IZQUIERDA:
      ubicacionActual.setX(ubicacionActual.getX() - incrementarVel());
      break;
    case DERECHA:
      ubicacionActual.setX(ubicacionActual.getX() + incrementarVel());
      break;
    }
  }
  glTranslatef(ubicacionActual.getX()*5.0, ubicacionActual.getY()*5.0 + 6,0.0);
  // Rotar ?
  if (go) {
    switch (dir) {
    case ARRIBA:
      break;
    case ABAJO:
      glRotatef(180.0,0.0,0.0,1.0);
      break;
    case IZQUIERDA:
      glRotatef(90.0,0.0,0.0,1.0);
      break;
    case DERECHA:
      glRotatef(-90.0,0.0,0.0,1.0);
      break;
    }
  }
  drawMesh(themesh, cf);
  glPopMatrix();
  return;
}

void Contrincante::dibujarContrincante() {
  glPushMatrix();
  glColor3f(0.2,0.9,0.5);
  // Verificar si se puede mover 
  // Jugador con un sólo punto en la trayectoria
  if (t.numPuntos == 1) 
    {
      glScalef(0.2,0.2,0.2);
      glTranslatef(ubicacionActual.getX()*5.0, 
                   ubicacionActual.getY()*5.0+6.0, 0.0);
      drawMesh(themesh,cf);
    }
  else  // Múltiples puntos
    {
      // Punto de origen -- Punto destino ?
      if (go) {
        t.calcularTrayectoria(&ubicacionActual);
      }
      glColor3f(1.0,0.0,0.0);
      glScalef(0.2,0.2,0.2);
      glTranslatef(ubicacionActual.getX()*5.0, 
                   ubicacionActual.getY()*5.0+6.0, 0.0);
      drawMesh(themesh, cf);
    }
  glPopMatrix();
  return;
}

void Nivel::dibujarJugadores() {
  // Verificar si juego en movimiento
  // Dibujamos el jugador
  j.dibujarJugador();
  // Dibujar contrincantes
  for(int i = 0; i < numContrincantes; i++) 
    {
      listaContrincantes[i].dibujarContrincante();
    }
}

// Dibujar objeto
void Objeto::dibujarObjeto() {
  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  //  glTranslatef(ubicacion.getX()*5.0, ubicacion.getY()*5.0 + 6,-1.5);
  glTranslatef(ubicacion.getX(), ubicacion.getY(),1.0);
  glRotatef(90.0,0.0,0.0,1.0);
  glScalef(0.2,0.2,0.2);
  drawMesh(themesh, cf);
  glPopMatrix();
  return;
}

//Dibujar obstaculos
void Nivel::dibujarObstaculos()
{
  for(int i = 0; i < numObjetos; i++) 
    {
      listaObjetos[i].dibujarObjeto();
    }
}

Punto Nivel::getSalida() {
  return salida;
}

// Dibuja trayectoria de contrincante
void Contrincante::dibujarTrayectoriaC()
{
  if (t.numPuntos == 1) { 
    glPushMatrix();
    glColor3f(1.0,0.0,0.0);
    glPointSize(20);
    glBegin(GL_POINTS);
    glVertex3f(t.listaPuntos[0].getX(), t.listaPuntos[0].getY(), 0.0);
    glEnd();
    glPopMatrix();
  }
  else {
    glColor3f(1.0,0.0,0.0);
    glPushMatrix();
    glLineWidth(10);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < t.numPuntos - 1; i++) 
      {
        glVertex3f(t.listaPuntos[i].getX(), t.listaPuntos[i].getY(), 0.0);
        glVertex3f(t.listaPuntos[i+1].getX(), t.listaPuntos[i+1].getY(), 0.0);
      }
    glEnd();
    glPopMatrix();
  }
  return;
}


void Nivel::dibujarTrayectoriaContrincantes() {
  for(int i = 0; i < numContrincantes; i++) 
    {
      listaContrincantes[i].dibujarTrayectoriaC();
    }
}

void Punto::Print() 
{
  cout << "Punto" << endl;
  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
}

void Trayectoria::Print()
{
  cout << "Trayectoria" << endl;
  cout << "velocidad = " << velocidad << endl;
  cout << "numPuntos = " << numPuntos << endl;
  for(vector<Punto>::iterator it = listaPuntos.begin();
      it != listaPuntos.end(); ++it)
    {
      (*it).Print();
    }
}

void Objeto::Print()
{
  cout << "Objeto" << endl;
  cout << "Ubicación = " << endl;
  ubicacion.Print();
  cout << "ArchivoMaya = " << archivoMaya << endl;
}

void Jugador::Print()
{
  cout << "Jugador" << endl;
  cout << "vidas = " << vidas << endl;
  cout << "velocidad = " << velocidad << endl;
  cout << "turbo = " << turbo << endl;
  cout << "velocidadTurbo = " << velocidadTurbo << endl;
  cout << "archivoMaya = " << archivoMaya << endl;
  cout << "Ubicación actual = " << endl;
  ubicacionActual.Print();
}

void Contrincante::Print() {
  t.Print();
  cout << "ArchivoMaya = " << maya << endl;
}

void Nivel::Print()
{
  cout << "Nivel" << endl;
  cout << "ID = " << id << endl;
  cout << "vidas = " << vidas << endl;
  t.Print();
  cout << "Salida" << endl;
  salida.Print();
  cout << "TerrenoBN = " << terrenoBN << endl;
  j.Print();
  cout << "numContrincantes = " << numContrincantes << endl;
  cout << "Contrincantes" << endl;
  for(vector<Contrincante>::iterator it = listaContrincantes.begin();
      it != listaContrincantes.end(); ++it)
    {
      (*it).Print();
    }
  cout << "NumObjetos = " << numObjetos << endl;
  for(vector<Objeto>::iterator it = listaObjetos.begin();
      it != listaObjetos.end(); ++it)
    {
      (*it).Print();
    }
}

void Juego::Print()
{
  cout << "Juego" << endl;
  cout << "NumNiveles = " << numNiveles << endl;
  for(vector<Nivel>::iterator it = listaNiveles.begin();
      it != listaNiveles.end(); ++it)
    {
      (*it).Print();
    }
}

void Tablero::Print() {
  cout << "Tablero" << endl;
  cout << "Ancho = " << ancho << endl;
  cout << "Largo = " << largo << endl;
}
