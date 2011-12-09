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
double Punto::getX() {
  return x;
}

double Punto::getY() {
  return y;
}

double Punto::getZ() {
  return z;
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
  if (velocidadEnAceleracion >= velocidad) return velocidad;
  velocidadEnAceleracion += velocidadIncremental;
  return velocidadEnAceleracion;
}

void Nivel::setGo() {
  for(vector<Contrincante>::iterator it = listaContrincantes.begin();
      it != listaContrincantes.end(); ++it) {
    (*it).setGo();
  }
}

void Jugador::actualizarVallas(double vel) {
  Punto v1, v2, v3, v4;
  Quad tmp;
  if (cambioDir && vel >= velocidad) { 
    switch(dir) {
    case ARRIBA:
      v1 = Punto(ubicacionActual.getX()*5.0,ubicacionActual.getY()*5.0+vel+6.0,1.0);
      v2 = Punto(ubicacionActual.getX()*5.0,ubicacionActual.getY()*5.0-vel+6.0,1.0);
      v3 = Punto(ubicacionActual.getX()*5.0+1.0,
                 ubicacionActual.getY()*5.0-vel+6.0,1.0);
      v4 = Punto(ubicacionActual.getX()*5.0+1.0,
                 ubicacionActual.getY()*5.0+vel+6.0,1.0);
      tmp = Quad(v1,v2,v3,v4);
      estela.push_back(tmp);
      break;
    case ABAJO:
      v1 = Punto(ubicacionActual.getX()*5.0, ubicacionActual.getY()*5.0-vel+6.0,1.0);
      v2 = Punto(ubicacionActual.getX()*5.0+1,
                 ubicacionActual.getY()*5.0-vel+6.0,1.0);
      v3 = Punto(ubicacionActual.getX()*5.0+1, 
                 ubicacionActual.getY()*5.0+vel+6.0,1.0);
      v4 = Punto(ubicacionActual.getX()*5.0, 
                 ubicacionActual.getY()*5.0+vel+6.0,1.0);
      tmp = Quad(v1,v2,v3,v4);
      estela.push_back(tmp);
      break;
    case IZQUIERDA:
      v1 = Punto(ubicacionActual.getX()*5.0-vel,ubicacionActual.getY()*5.0+6.0,1.0);
      v2 = Punto(ubicacionActual.getX()*5.0-vel,ubicacionActual.getY()*5.0+5.0,1.0);
      v3 = Punto(ubicacionActual.getX()*5.0+vel,ubicacionActual.getY()*5.0+5.0,1.0);
      v4 = Punto(ubicacionActual.getX()*5.0+vel,ubicacionActual.getY()*5.0+6.0,1.0);
      tmp = Quad(v1,v2,v3,v4);
      estela.push_back(tmp);
      break;
    case DERECHA:
      v1 = Punto(ubicacionActual.getX()*5.0+vel,ubicacionActual.getY()*5.0+6.0,1.0);
      v2 = Punto(ubicacionActual.getX()*5.0+vel,ubicacionActual.getY()*5.0+5.0,1.0);
      v3 = Punto(ubicacionActual.getX()*5.0-vel,ubicacionActual.getY()*5.0+5.0,1.0);
      v4 = Punto(ubicacionActual.getX()*5.0-vel,ubicacionActual.getY()*5.0+6.0,1.0);
      tmp = Quad(v1,v2,v3,v4);
      estela.push_back(tmp);
      break;
    }
    cambioDir = 0;
  }
  else {  // Se actualiza la coordenada de la valla.
    if (vel >= velocidad && !cambioDir) { 
      switch(dir) {
      case ARRIBA:
        tmp = estela.back();
        tmp.v1 = Punto(ubicacionActual.getX()*5.0, 
                       ubicacionActual.getY()*5.0+vel+6.0,1.0);
        tmp.v4 = Punto(ubicacionActual.getX()*5.0+1, 
                       ubicacionActual.getY()*5.0+vel+6.0,1.0);
        estela.pop_back();
        estela.push_back(tmp);
        break;
      case ABAJO:
        tmp = estela.back();
        tmp.v1 = Punto(ubicacionActual.getX()*5.0, 
                       ubicacionActual.getY()*5.0-vel+6.0,1.0);
        tmp.v2 = Punto(ubicacionActual.getX()*5.0+1, 
                       ubicacionActual.getY()*5.0-vel+6.0,1.0);
        estela.pop_back();
        estela.push_back(tmp);
        break;
      case IZQUIERDA:
        tmp = estela.back();
        tmp.v1 = Punto(ubicacionActual.getX()*5.0-vel,
                       ubicacionActual.getY()*5.0+6.0,1.0);
        tmp.v2 = Punto(ubicacionActual.getX()*5.0-vel, 
                       ubicacionActual.getY()*5.0+5.0,1.0);
        estela.pop_back();
        estela.push_back(tmp);
        break;
      case DERECHA:
        tmp = estela.back();
        tmp.v1 = Punto(ubicacionActual.getX()*5.0+vel,
                       ubicacionActual.getY()*5.0+6.0,1.0);
        tmp.v2 =  Punto(ubicacionActual.getX()*5.0+vel, 
                        ubicacionActual.getY()*5.0+5.0,1.0);
        estela.pop_back();
        estela.push_back(tmp);
        break;
      }
    }
  }
}

void Jugador::cambiarDireccion() {
  cambioDir = 1;
}

void Jugador::dibujarJugador() {
  glPushMatrix();
  // Punto de origen -- Punto destino ?
  //  t.calcularTrayectoria(&posActual);
  //  glColor3f(1.0,0.0,0.0);
  glScalef(0.2,0.2,0.2);
  // Actualizar desplazamiento del jugador
  // Juego paralizado ?
  double vel;
  if (go) {
    switch (dir) {
    case ARRIBA:
      vel = incrementarVel();
      actualizarVallas(vel);
      ubicacionActual.setY(ubicacionActual.getY() + vel);
      break;
    case ABAJO:
      vel = incrementarVel();
      actualizarVallas(vel);
      ubicacionActual.setY(ubicacionActual.getY() - vel);
      break;
    case IZQUIERDA:
      vel = incrementarVel();
      actualizarVallas(vel);
      ubicacionActual.setX(ubicacionActual.getX() - vel);
      break;
    case DERECHA:
      vel = incrementarVel();
      actualizarVallas(vel);
      ubicacionActual.setX(ubicacionActual.getX() + vel);
      break;
    }
  }
  dibujarEstela();
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

double Nivel::sumarColumna(int rep, int ancho, int offset, int columna, 
                    double *pix) {
  double sum = 0.0;
  for(int i = 0; i < rep; i++) {
    sum += pix[columna + offset +i*ancho]; 
  }
  return sum;
}

double Nivel::sumarFila(int rep, int fila, int anchoPM, int offset, 
                        double* transformadaI) {
  double sum = 0.0;
  for(int i = 0; i < rep; i++) {
    sum += transformadaI[fila*anchoPM + offset + i];
  }
  return sum;
}

double * Nivel::ajustarPixels(double *pix, int anchoPM, int largoPM) {
  int nuevoLargo = PUNTOSY;
  int nuevoAncho = PUNTOSX;
  int tam_loteI = largoPM / PUNTOSY;
  int tam_loteJ = anchoPM / PUNTOSX;
  int offset = 0;
  double *transformadaI = (double *) malloc(sizeof(double)*
                                            anchoPM*PUNTOSY);
  // Transformación en i 
  for(int j = 0; j < PUNTOSY; j++) {
    for(int i = 0; i < anchoPM; i++) {
      transformadaI[i + anchoPM*j] = 
        sumarColumna(tam_loteI, anchoPM, offset, i, pix) / tam_loteI; 
    }
    offset += tam_loteI;
  }
  
  double *transformada = (double *) malloc(sizeof(double)*PUNTOSX*PUNTOSY);
  // Transformación en j 
  for(int j = 0; j < PUNTOSY; j++) {
    offset = 0;
    for(int i = 0; i < PUNTOSX; i++) {
      transformada[i + PUNTOSX*j] = 
        sumarFila(tam_loteJ, j, anchoPM, offset, transformadaI) / tam_loteJ;
      offset += tam_loteJ;
    }
  }
  free(transformadaI);
  return transformada;
}

void Nivel::normalizarTransformada(double* transformada) {
  for(int fila = 0; fila < PUNTOSY; fila++) {
    for(int col = 0; col < PUNTOSX; col++) {
      transformada[col + fila*PUNTOSX] = (double) 
        ALTOF*transformada[col + fila*PUNTOSX] / 255.0;
    }
  }
}

double * Nivel::cargarFractal() {
  fipImage fp = fipImage();
  if (fp.load(terrenoBN))
    cout << "Terreno cargado exitosamente" << endl;
  BYTE *p= fp.accessPixels();
  int anchoPM = fp.getWidth();
  int largoPM = fp.getHeight();

  double *monopix = (double *) malloc(sizeof(double)*anchoPM*largoPM);
  int z = 0;
  for(int i = 0; i < fp.getHeight(); i++) {
    for(int j = 0; j < fp.getLine(); j = j + 3) {
      monopix[z] = (double) p[j + i*fp.getLine()];
      z++;
    }
  }
  
  // Se construye y normaliza transformada
  double *tr = ajustarPixels(monopix, anchoPM, largoPM);
  normalizarTransformada(tr);
  free(monopix);
  return tr;
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

void Nivel::dibujarFractal() {
  double sepX = (double) TERRENOX / (double) PUNTOSX;
  double sepY = (double) TERRENOY / (double) PUNTOSY;
  double baseX = TERRENOX;//*5.0;
  double baseY = TERRENOY;//*5.0 + 6.0;
  glPushMatrix();
  glColor4f(1.0,0.0,0.0,0.5);
  glTranslatef(0.0,0.0,1.5);
  for(int i = 0; i < PUNTOSY - 1; i++) {
    for(int j = 0; j < PUNTOSX - 1; j++) {
      glBegin(GL_TRIANGLES);
      // Primer triangulo
      glVertex3f(baseX + sepX*j,baseY - sepY*j,transformada[j + i*PUNTOSX]);
      glVertex3f(baseX + sepX*(j+1),baseY - sepY*(j+1),transformada[j + i*PUNTOSX + (PUNTOSX + 1)]);
      glVertex3f(sepX*j + baseX, baseY - sepY*(j+1), transformada[j + i*PUNTOSX + PUNTOSX]);
      // Segundo triangulo
      glVertex3f(sepX*j + baseX,baseY - sepY*j, transformada[j + i*PUNTOSX]);
      glVertex3f(sepX*(j+1) + baseX,baseY  - sepY*j ,transformada[j + 1 + i*PUNTOSX]);
      glVertex3f(sepX*(j+1) + baseX,baseY - sepY*(j+1),transformada[j + i*PUNTOSX + (PUNTOSX + 1)]);
      glEnd();
    }
  }
  glPopMatrix();
}


Punto Nivel::getSalida() {
  return salida;
}

// Dibuja la estela
void Jugador::dibujarEstela() {
  glPushMatrix();
  glBegin(GL_QUADS);
  glColor4f(0.0, 1.0/255.0,1.0/255.0,0.5);
  for(vector<Quad>::iterator it = estela.begin();
      it != estela.end();
      ++it) {
    Quad tmp = (*it);
    glVertex3f(tmp.v1.getX(),tmp.v1.getY(),tmp.v1.getZ());
    glVertex3f(tmp.v2.getX(),tmp.v2.getY(),tmp.v2.getZ());
    glVertex3f(tmp.v3.getX(),tmp.v3.getY(),tmp.v3.getZ());
    glVertex3f(tmp.v4.getX(),tmp.v4.getY(),tmp.v4.getZ());
  }
  glEnd();
  glPopMatrix();
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
  cout << "z = " << z << endl;
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
