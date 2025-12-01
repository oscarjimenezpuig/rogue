/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: dilluns, 1 de desembre de 2025, 05:22:09
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <string.h>

#include "curse.h"

/* CONSTANTES */

#define OBJETOS 512 /* numero de objetos */

#define ONUL 0 /* objeto nulo */

#define MAPAC 189 /* dimensiones del mapa */
#define MAPAR 45
#define MAPAA (MAPAR*MAPAC)

/* TIPOS */

typedef signed char s1;
typedef unsigned short u2;
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

typedef struct {
	u1 objeto;
	struct {
		u1 visible : 1;
		u1 obstaculo : 1;
		u1 habitacion : 4; /* si habitacion es 0, se corresponde a camino */
	};
} localidad;
		
/* VARIABLES */

extern localidad mapa[MAPAA];

/* FUNCIONES */

/* objeto.c */

u1 objnew(u1 id,u1 type);
//definicion de un objeto nuevo

pobjeto objget(u1 id);
//obtenemos el apuntador al objeto

#endif /* ROGUE_H */
