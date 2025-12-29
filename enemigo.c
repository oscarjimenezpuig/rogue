/*
============================================================
  Fichero: enemigo.c
  Creado: 29-12-2025
  Ultima Modificacion: dilluns, 29 de desembre de 2025, 10:19:23
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

typedef struct {
	char* nom;
	char chr;
	uint ink : 3;
	uint mov : 1;
	uint fue : 4;
	uint hab : 4;
	uint vel : 4;
	uint cap : 4;
} enemigo_t;

static objeto_t* enenew(enemigo_t e,uint nivel) {
	/* definicion de un enemigo a partir de sus caracteristicas y su nivel */
	//TODO: Normalizar las caracteristicas a partir de los puntos iniciales, finales y el nivel
	atributo_t a={e.chr,BOLD,e.ink,BLACK};
	objeto_t* o=objnew(e.nom,a,TRUE,FALSE);
	if(o) {
		o->mov=e.mov;
		o->fue=e.fue;
		o->hab=e.hab;
		o->vel=e.vel;
		o->cap=e.cap;
		o->cve=o->vel;
	}
	return o;
}


		
