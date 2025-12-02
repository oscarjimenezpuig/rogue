/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: dilluns, 1 de desembre de 2025, 20:06:24
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include "curse.h"

/* CONSTANTES */

#define OBJETOS 512 /* numero de objetos */

#define MAPAC 189 /* dimensiones del mapa */
#define MAPAR 45
#define MAPAA (MAPAR*MAPAC)

/* tipos de localidad */
#define VACIO 0
#define OBSTACULO 1
#define TRANSITABLE 2

/* TIPOS */

typedef signed char s1;
typedef unsigned short u2;
typedef unsigned int u4;

struct objeto_s {
	u1 type;
	char* nombre;
	char chr;
	s1 ink;
	s1 background;
};

typedef u1 objeto_t;
typedef struct objeto_s* pobjeto_t;

typedef struct {
	u1 visible : 1;
	u1 tipo : 2;
	u1 habitacion : 4; /* si habitacion es 0, se corresponde a camino */
} localidad_t;
		
/* VARIABLES */

extern localidad_t mapa[MAPAA];

/* FUNCIONES */

/* objeto.c */

u1 objnew(objeto_t o,u1 type);
//definicion de un objeto nuevo

pobjeto_t objget(objeto_t o);
//obtenemos el apuntador al objeto

#endif /* ROGUE_H */
