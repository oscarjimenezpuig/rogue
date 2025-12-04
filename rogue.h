/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: mié 03 dic 2025 10:40:04
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h> //dbg

#include "curse.h"

/* CONSTANTES */

#define OBJETOS 512 /* numero de objetos */

#define MAPAC 159 /* dimensiones del mapa */
#define MAPAR 48
#define MAPAA (MAPAR*MAPAC)

/* tipos de localidad */
#define VACIO 0
#define OBSTACULO 1 //pared
#define TRANSITABLE 2 //camino o habitacion
#define PUERTA 3 //lugar de una puerta
#define OCULTA 4 //lugar de una puerta oculta (en principio, como pared)

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
	u1 tipo : 2;
	u1 visible : 1;
	u1 oscuro : 1;
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

/* map.c */

void map_new();
//creacion del mapa de un nivel

#endif /* ROGUE_H */
