#include "include/Parser.h"

Punto parsePunto(TiXmlHandle rootPunto)
{
  // Punto x
  TiXmlElement *pElem = rootPunto.FirstChild("x").Element();
  int x = atof(pElem->GetText());
  // Punto y
  pElem = rootPunto.FirstChild("y").Element();
  int y = atof(pElem->GetText());
  Punto pto(x,y);
  return pto;
}

Trayectoria parseTrayectoria(TiXmlHandle rootTrayectoria)
{
  // velocidad 
  TiXmlElement *pElem = rootTrayectoria.FirstChild("velocidad").Element();
  double vel = atof(pElem->GetText())/5.0;
  // número de puntos
  pElem = rootTrayectoria.FirstChild("puntos").Element();
  int pts = atoi(pElem->GetText());
  // Procesar lista de puntos
  vector<Punto> listPts;
  int i = 0;
  for(TiXmlElement *nodoPunto = rootTrayectoria.FirstChild("punto").Element();
      i < pts; nodoPunto = nodoPunto->NextSiblingElement())
    {
      TiXmlHandle rootPunto(nodoPunto);
      Punto pt = parsePunto(rootPunto);
      listPts.push_back(pt);
      i++;
    }
  Trayectoria tr(vel,pts,listPts);
  return tr;
}

Jugador parseJugador(TiXmlHandle rootJugador, int vidas) 
{
  // disparo
  TiXmlElement *pElem = rootJugador.FirstChild("velocidad").Element();
  double velocidad = atof(pElem->GetText())/5.0;
  pElem = rootJugador.FirstChild("posInicial").Element();
  TiXmlHandle rootPosInicial(pElem);
  pElem = rootPosInicial.FirstChild("punto").Element();
  TiXmlHandle rootPunto(pElem);
  Punto posInicial = parsePunto(rootPunto);
  pElem = rootJugador.FirstChild("turbo").Element();
  int turbo = atoi(pElem->GetText());
  pElem = rootJugador.FirstChild("velocidadTurbo").Element();
  double velocidadTurbo = atof(pElem->GetText())/5.0;
  pElem = rootJugador.FirstChild("maya").Element();
  TiXmlHandle rootMaya(pElem);
  pElem = rootMaya.FirstChild("archivo").Element();
  char *archivoMaya = strdup(pElem->GetText());
  Jugador jug(vidas, velocidad, turbo, velocidadTurbo, archivoMaya,
              posInicial);
  jug.cf = coeficientesMaya(jug.themesh);
  return jug;
}

Contrincante parseContrincante(TiXmlHandle rootContrincante) 
{
  TiXmlElement *pElem = rootContrincante.FirstChild("trayectoria").Element();
  TiXmlHandle rootTrayectoria(pElem);
  Trayectoria tr = parseTrayectoria(rootTrayectoria);
  pElem = rootContrincante.FirstChild("maya").Element();
  TiXmlHandle rootMaya(pElem);
  pElem = rootMaya.FirstChild("archivo").Element();
  char* archivoMaya = strdup(pElem->GetText());
  Contrincante ctr = Contrincante(tr, archivoMaya);
  ctr.cf = coeficientesMaya(ctr.themesh);
  return ctr;
}

vector<Objeto> parseObjetos(TiXmlHandle rootObjetos) 
{
  // cantidad 
  TiXmlElement *pElem = rootObjetos.FirstChild("cantidad").Element();
   int cant = atoi(pElem->GetText());
  // objetos
  vector<Objeto> listObjetos;
  int i = 0;
  for(TiXmlElement *nodoObjeto = rootObjetos.FirstChild("objeto").Element();
      i < cant; nodoObjeto = nodoObjeto->NextSiblingElement())
    {
      TiXmlHandle rootObjeto(nodoObjeto);
      pElem = rootObjeto.FirstChild("maya").Element();
      TiXmlHandle rootMaya(pElem);
      // archivo
      pElem = rootMaya.FirstChild("archivo").Element();
      char *archivo = strdup(pElem->GetText());
      // x 
      pElem = rootMaya.FirstChild("x").Element();
      int x = atoi(pElem->GetText());
      // y 
      pElem = rootMaya.FirstChild("y").Element();
      int y = atoi(pElem->GetText());
      Punto pto(x,y);
      Objeto obj = Objeto(pto, archivo);
      listObjetos.push_back(obj);
      i++;
    }
  return listObjetos;
}

Tablero parseTablero(TiXmlHandle rootTablero) {
  // Ancho
  TiXmlElement *pElem = rootTablero.FirstChild("ancho").Element();
  double ancho = atof(pElem->GetText());
  // Largo
  pElem = rootTablero.FirstChild("largo").Element();
  double largo = atof(pElem->GetText());
  Tablero tab(ancho, largo);
  return tab;
}

Nivel parseNivel(TiXmlHandle rootNivel)
{
  // Parsear Nivel
  // Identificador 
  TiXmlElement *pElem = rootNivel.FirstChild("id").Element();
  int ident = atoi(pElem->GetText());
  // Vidas
  pElem = rootNivel.FirstChild("vidas").Element();
  int vidas = atoi(pElem->GetText());
  // Tablero
  pElem = rootNivel.FirstChild("tablero").Element();
  TiXmlHandle rootTablero(pElem);
  Tablero tab = parseTablero(rootTablero);
  // Salida
  pElem = rootNivel.FirstChild("salida").Element();
  TiXmlHandle rootSalida(pElem);
  pElem = rootSalida.FirstChild("punto").Element();
  TiXmlHandle rootPunto(pElem);
  Punto salida = parsePunto(rootPunto);
  pElem = rootNivel.FirstChild("terrenoBN").FirstChild("textura").FirstChild
    ("archivo").Element();
  char * terrenoBN = strdup(pElem->GetText());
  // Jugador 
  pElem = rootNivel.FirstChild("jugador").Element();
  TiXmlHandle rootJugador(pElem);
  Jugador jug = parseJugador(rootJugador, vidas);
  // Número de contrincantes
  pElem = rootNivel.FirstChild("contrincantes").Element();
  int numContr = atoi(pElem->GetText());
  // Iterar sobre los contrincantes 
  vector<Contrincante> listaContr;
  int i = 0;
  for(TiXmlElement *nodoContrincantes = 
        rootNivel.FirstChild("contrincante").Element();
      i < numContr; nodoContrincantes = nodoContrincantes->NextSiblingElement())
    {
      TiXmlHandle rootContrincante(nodoContrincantes);
      Contrincante ctr = parseContrincante(rootContrincante);
      listaContr.push_back(ctr);
      i++;
    }
  // Objetos
  // Cantidad de objetos
  pElem = rootNivel.FirstChild("objetos").FirstChild("cantidad").Element();
  int cantObjetos = atoi(pElem->GetText());
  // Lista de objetos
  pElem = rootNivel.FirstChild("objetos").Element();
  TiXmlHandle rootObjetos(pElem);
  vector<Objeto> listaObjetos = parseObjetos(rootObjetos);
  // Crear Nivel 
  Nivel nv(ident,vidas,tab,salida,terrenoBN,jug,numContr,listaContr,
           cantObjetos,listaObjetos);
  return nv;
}

Juego parse (char *archivo)
{
  Juego j;
  /* Comienzo Parsing de XML */
  TiXmlDocument doc(archivo);
  if (!doc.LoadFile()) return j;

  TiXmlHandle hDoc(&doc);
  TiXmlElement* pElem;
  TiXmlHandle hRoot(0);

  pElem = hDoc.FirstChildElement().Element();
  hRoot = TiXmlHandle(pElem);

  // Bloque: Proyecto
  pElem = hRoot.FirstChild("niveles").Element();
  // Número de niveles
  int numNiveles = atoi(pElem->GetText());
  // Iterar sobre lista de niveles
  vector<Nivel> lstNivel;
  for(TiXmlElement *nodoNivel = hRoot.FirstChild("nivel").Element();
      nodoNivel; nodoNivel = nodoNivel->NextSiblingElement()) 
    {
      TiXmlHandle rootNivel(nodoNivel);
      Nivel niv = parseNivel(rootNivel);
      lstNivel.push_back(niv);
    }
  j = Juego(numNiveles, lstNivel);
  return j;
}
