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
  double desplazamiento = calcDesp(velocidad/**1/60.0*/);
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

void Jugador::dibujarJugador() 
{
  return;
}

// Dibujar objeto
void Objeto::dibujarObjeto() {
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

// Dibuja trayectoria de contrincante
void Nivel::dibujarTrayectoriaC()
{
  return;
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
