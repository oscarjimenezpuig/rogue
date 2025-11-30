/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: diumenge, 30 de novembre de 2025, 20:53:38
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <string.h>

#include "curse.h"

/* CONSTANTES */

#define OBJETOS 512

/* TIPOS */

typedef signed char s1;
typedef unsigned int u4;
typedef char strs[20];
typedef char strl[256];

struct objeto_s {
	u1 type;
	char* nombre;
	char chr;
	s1 ink;
	s1 background;
};

typedef u4 objeto;
typedef struct objeto_s* pobjeto;

/* VARIABLES */

/* FUNCIONES */

u1 objnew(u4 id,u1 type);
//definicion de un objeto nuevo

pobjeto objget(u1 id);
//obtenemos el apuntador al objeto

#endif /* ROGUE_H */
