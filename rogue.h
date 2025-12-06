/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: dissabte, 6 de desembre de 2025, 12:41:33
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h> //dbg

#include "curse.h"

/* CONSTANTES */

/* numero de objetos */
#define OBJETOS 512

/* longitud nombres objeto */
#define NAMOBJLEN 20

/* dimensiones del mapa */
#define MAPAC 159
#define MAPAR 48
#define MAPAA (MAPAR*MAPAC)

/* tipos de localidad */
#define VACIO 0
#define OBSTACULO 1 /* pared */
#define TRANSITABLE 2 /* camino o habitacion */
#define PUERTA 3 /* lugar de una puerta */
#define OCULTA 4 /* lugar de una puerta oculta (en principio, como pared) */

/* tipos de objeto */
#define COGIBLE 1 /* objeto que se puede coger */
#define ABRIBLE 2 /* objeto que se puede abrir con una llave */
#define ARMA 4 /* objeto que se puede usar como arma */
#define VESTIBLE 8 /* objeto que se puede vestir */
#define NPC 16 /* objeto que es pensante */
#define MOVIL 32 /* objeto que se puede mover */
#define JUGADOR 64 /* objeto que es jugador */
#define ESCALERA 128 /* objeto que es una escalera */

/* tipos de transitable */
#define HABITACION 1
#define PASADIZO 2

/* atributos del mapa */
#define ATROBS (atributo_t){' ',0,RED,RED} /* atributo de las paredes */
#define ATRHAB (atributo_t){'.',0,WHITE,BLACK} /* atributo de suelo de habitacion */
#define ATRPAS (atributo_t){'#',DIM,WHITE,BLACK} /* atributo de pasadizo de habitacion */
#define ATRPUE (atributo_t){'?',0,BLACK,YELLOW} /* atributo de la puerta */

/* jugador */
#define OJUG 0 /* identidad del jugador */
#define PJUG objget(OJUG) /* puntero al jugador */
#define ATJUG (atributo_t){'@',BOLD,WHITE,BLACK} /* atributos del jugador */

/* TIPOS */

typedef signed char s1;
typedef unsigned short u2;
typedef signed short s2;
typedef unsigned int u4;

typedef struct {
	char chr;
	u1 attr;
	s1 ink;
	s1 bkg;
} atributo_t;

typedef struct {
	s2 golpes,fuerza,armadura,habilidad,magia,velocidad,oro;
	u2 vact; /* cuando vact==velocidad, el npc puede actuar */
} npc_t;

struct objeto_s {
	u2 tipo;
	char nombre[NAMOBJLEN+1];
	atributo_t atributo;
	int r,c;
	union {
		npc_t npc;
	};
};

typedef u2 objeto_t;
typedef struct objeto_s* pobjeto_t;

typedef struct {
	u1 tipo : 3;
	u1 visible : 1;
	u1 oscuro : 1;
	u1 habitacion : 3; /* si habitacion es 0, se corresponde a camino */
} localidad_t;

typedef u1 (*Condicion)(objeto_t);
		
/* VARIABLES */

extern localidad_t mapa[MAPAA];

/* FUNCIONES */

/* objeto.c */

u1 objnew(objeto_t o,u2 tipo);
/* definicion de un objeto nuevo */

pobjeto_t objget(objeto_t o);
/* obtenemos el apuntador al objeto */

u2 objrndpos(objeto_t o,u1 tipos_transitable);
/* coloca al objeto en una posicion del mapa que coincida con el tipo */

u2 objsfnd(objeto_t* os,Condicion c);
/* a partir de una condicion encontramos una matriz de objetos que la cumplen, se devuelve el numero de objetos*/

/* map.c */

void mapnew();
/* creacion del mapa de un nivel */

localidad_t* mapget(int r,int c);
/* consigue las caracteristicas de la localidad en la columna fila pedidas */

u1 maprndpos(int* r,int* c,u1 tipo_transitable);
/* busca una posicion del transitable que cumpla la condicion del tipo de transitable */

/* pantalla.c */

void showscr(int mri,int mci,int mrs,int mcs,int ro,int co);
/*muestra un sector del mapa que empieza en las coordenadas mri,mci
 * con dimensiones mrs,mcs y la posicion inicial en la pantalla terminal es ro,co */

/* npc.c */

u1 npcnew(objeto_t id,char* nombre,atributo_t a,u1 movil,npc_t npc);
/* definicion de un npc */

u2 npcfnd(pobjeto_t* npc);
/* busca todos los npc`s del juego(en punteros) */

u1 npcinplace(int r,int c);
/* dice si en una posicion hay un npc */

u1 npccanpos(objeto_t o,int r,int c);
/* dice si un objeto se puede posicionar en la posicion especificada */
/* no se podra poner si hay otro npc en la misma posicion o no es transitable*/

u1 npcpos(objeto_t o,int r,int c);
/* coloca, si se puede un objeto en r,c (llama a npccanpos) */

u1 npcrndpos(objeto_t o,u1 tipo_transitable);
/* se coloca un npc en una posicion random */

u1 npccanact(objeto_t o);
/* mira si un npc puede o no actuar (depende de vact y velocidad) */

/* jugador.c */

u1 jugnew();
/* definicion del jugador */

u1 jugact();
/* accion del jugador */

/* rogue.c */


#endif /* ROGUE_H */
