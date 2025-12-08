/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 19:49:15
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h> //dbg

#include "curse.h"

/* CONSTANTES */

/* numero de niveles */
#define MINLEVEL 1 /* minimo numero de nivel */
#define MAXLEVEL 10 /* maximo numero de nivel */
#define LEVELS (MAXLEVEL-MINLEVEL+1) /* numero de levels totales */

/* numero de objetos */
#define OBJETOS 512

/* longitud nombres objeto */
#define NAMOBJLEN 20

/* objeto nulo */
#define OBJNUL OBJETOS

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
#define LLAVE 256 /* objeto que permite abrir */
#define ORO 512 /* oro (este objeto cuando se coge pasa no ser dejable */

/* tipos de transitable */
#define HABITACION 1
#define PASADIZO 2

/* atributos del mapa */
#define ATROBS (atributo_t){' ',REVERSE,RED,RED} /* atributo de las paredes */
#define ATRHAB (atributo_t){'.',0,WHITE,BLACK} /* atributo de suelo de habitacion */
#define ATRPAS (atributo_t){'#',DIM,WHITE,BLACK} /* atributo de pasadizo de habitacion */
#define ATRPUE (atributo_t){'?',REVERSE,YELLOW,BLACK} /* atributo de la puerta */
#define ATRESU (atributo_t){'>',BOLD,MAGENTA,BLACK} /* escalera de subida */
#define ATREDW (atributo_t){'<',BOLD,MAGENTA,BLACK} /* escalera de bajada */

#define COLVIS BLUE /*color de lo visibilizado */

/* npc */
#define MAXCAR 15  /* valor maximo de las caracteristicas */

/* jugador */
#define OJUG 0 /* identidad del jugador */
#define PJUG objget(OJUG) /* puntero al jugador */
#define ATJUG (atributo_t){'@',BOLD,WHITE,BLACK} /* atributos del jugador */

/* llave inicial */
#define OLLV 1 /* identidad de la llave inicial */
#define PRLL 2 /* posibilidad de que una llave se rompa cuando se abre la puerta */

/* mensaje */
#define STRCON "MAS" /* string que se pone al final del mensaje si tiene continuacion */

/* teclado */
#define TARR 'i' /* arriba */
#define TABJ 'k' /* abajo */
#define TIZQ 'j' /* izquierda */
#define TDER 'l' /* derecha */
#define TCOG 'c' /* coger */
#define TDEJ 'd' /* dejar */
#define TINV 'I' /* inventario */
#define TQUT 'Q' /* tecla finalizar juego */
#define TMFI 'M' /* cerrar menu sin seleccionar */
#define TMOK 'm' /* cerrar menu seleccionando */
#define TABR 'a' /* abrir una puerta */
#define TFRZ 'f' /* forzar la puerta */

/* oro */
#define ORCPNI 100 /* oro por nivel */
#define ATRORO (atributo_t){'$',BOLD,YELLOW,BLACK}

/* TIPOS */

typedef signed char s1;
typedef unsigned short u2;
typedef signed short s2;
typedef unsigned int u4;

typedef u2 objeto_t;

typedef struct {
	char chr;
	u1 attr;
	s1 ink;
	s1 bkg;
} atributo_t;

typedef struct {
	u1 golpes : 4;
	u1 fuerza : 4;
	u1 armadura : 4;
	u1 habilidad : 4;
	u1 magia : 4;
	u1 velocidad : 4;
	u1 capacidad : 4;
	u1 vact; /* cuando vact==15-velocidad, el npc puede actuar */
	u2 oro;
} npc_t;

typedef struct {
	objeto_t contenedor; /* si es cogible, da el contenedor */
	u2 oro; /* cantidad de oro */
	struct {
		u1 abrepuerta : 1; /* si es llave, mira si puede abrir puerta */
	};
} item_t;

struct objeto_s {
	u2 tipo;
	char nombre[NAMOBJLEN+1];
	atributo_t atributo;
	int r,c;
	union {
		npc_t npc;
		item_t item;
	};
};

typedef struct objeto_s* pobjeto_t;

typedef struct {
	u1 tipo : 3;
	u1 visible : 1;
	u1 oscuro : 1;
	u1 habitacion : 4; /* si habitacion es 0, se corresponde a camino */
	u1 visibilizado : 1;
	s1 escalera : 2; /* escalera -1 bajada, 1 subida, 0 sin escalera */
} localidad_t;

typedef struct {
	int seed;
	u1 enemigos;
	u1 oro;
	u1 llaves;
	u1 objetos;
} nivel_t;
	
typedef u1 (*Condicion)(objeto_t);
		
/* VARIABLES */

//extern localidad_t mapa[MAPAA];

/* FUNCIONES */

/* objeto.c */

u1 objnew(objeto_t o,u2 tipo);
/* definicion de un objeto nuevo */

void objname(objeto_t o,char* name);
/* asigna al objeto el nombre */

pobjeto_t objget(objeto_t o);
/* obtenemos el apuntador al objeto */

u2 objsfnd(objeto_t* os,Condicion c);
/* a partir de una condicion encontramos una matriz de objetos que la cumplen, se devuelve el numero de objetos*/

u2 objsfree();
/* da el primer identificador de objeto libre */

/* map.c */

void mapnew(u1 level);
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

u2 npcinv(objeto_t o,objeto_t* c);
/* da la lista de objetos contenida por o */

u1 npccancog(objeto_t o,objeto_t c);
/* dice si un objeto se puede coger */

u1 npccog(objeto_t o,objeto_t c);
/* coge un objeto (llama a npccancog) */

u1 npccandej(objeto_t o,objeto_t c);
/* se mira si el objeto o puede dejar al objeto c */

u1 npcdej(objeto_t o,objeto_t c);
/* el objeto o deja el objeto c en la localidad establecida (llamada a npccandej) */

u1 npccanact(objeto_t o);
/* mira si un npc puede o no actuar (depende de vact y velocidad) */

/* jugador.c */

u1 jugnew();
/* definicion del jugador */

u1 jugact();
/* accion del jugador */

/* item.c */

u1 itmnew(objeto_t identificador,char* n,u2 tipo,atributo_t a,item_t item);
/* definicion de un item */

/* mensaje.c */

void mensaje(char* men);
/* se introduce un mensaje que se separara por trozos */

/* menu.c */

u1 menu(char* cabecera,u1 opciones,char* opcion[]);
/* se crea un menu, funciona con las teclas i: subir j: bajar q: quitar s: seleccionar.
 * Devuelve la opcion selecconada o opciones en caso de no selecionar nada */

/* oro.c */

void orosnew(int cantidad);
/* crea y coloca oro en todo el nivel en la cantidad dicha */

/* rogue.c */


#endif /* ROGUE_H */
