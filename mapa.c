/*
============================================================
  Fichero: mapa.c
  Creado: 01-12-2025
  Ultima Modificacion: dilluns, 1 de desembre de 2025, 05:26:10
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "mapa.h"

#define MAXHAB 9 /* numero maximo de habitaciones */

static u1 nhabs=0;
static u1 phabs[MAXHAB];

static void habsini() {
	localidad* p=mapa;
	while(p!=mapa+MAPAA) *p++=(localidad){ONUL,{0,1,0}};
	nhabs=rnd(1,MAXHAB);
	//TODO: Definir las posiciones donde estaran las habitaciones
}
	

