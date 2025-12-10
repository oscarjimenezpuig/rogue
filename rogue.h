/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: dimecres, 10 de desembre de 2025, 05:20:04
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h>

#include "curse.h"

/* CONSTANTES */

#define EOS '\0' /* final de string */
#define SLEN 20 /* longitud de los string definidos */

#define TRUE 1
#define FALSE 0

/* teclado */
#define TARR 'i' /* arriba */
#define TABJ 'k' /* abajo */
#define TIZQ 'j' /* izquierda */
#define TDER 'l' /* derecha */
#define TCOG 'c' /* coger */
#define TDEJ 'd' /* dejar */
#define TINV 'I' /* inventario */
#define TQUT 'Q' /* tecla finalizar juego */
#define TABR 'a' /* abrir una puerta */
#define TFRZ 'f' /* forzar la puerta */

/* MACROS */

/* TIPOS */

typedef unsigned int uint;
typedef unsigned char Bool;

typedef struct {
	uint chr : 7; /* caracter */
	uint atr : 7; /* atributo */
	uint ink : 3; /* tinta */
	uint bkg : 3; /* fondo */
} atributo_t;

typedef struct {
	uint obs : 1; /* 1: obstaculo */
	uint trs : 2; /* 1: transitable normal, 2 : puerta, 3: oculta */
	uint vis : 2; /* 1: visibilizado 2: visible actual */
	uint osc : 1; /* 1: oscura */
	uint hab : 4; /* numero habitacion */
	int esc : 2; /* -1: escalera descenso, 1: escalera ascenso */
} localidad_t;

struct objeto_s {
	char nom[SLEN];
	atributo_t atr;
	int r,c;
	uint npc : 1; /* 1: es npc 0: es item */
	union {
		struct {
			uint jug : 1; /* 1: es jugador */
			uint mov : 1; /* 1: es movil */
			uint fue : 4; /* fuerza */
			uint hab : 4; /* habilidad */
			uint vel : 4; /* velocidad */
			uint cap : 4; /* capacidad */
			uint cve : 4; /* contador velocidad */
			uint oro : 12; /* oro */
		};
		struct {
			uint ior : 1; /* 1: es oro */
			uint arm : 1; /* 1: es arma */
			uint lla : 1; /* 1: es llave */
			uint ani : 1; /* 1: es anillo */
			uint ves : 1; /* 1: esta vestido */
			struct objeto_s* con; /* da el contenedor */
			union {
				uint cor : 8; /* cantidad de oro del tesoro */
			};
		};
	};
};

typedef struct objeto_s objeto_t;

typedef Bool (*Condicion)(objeto_t*);

/* VARIABLES */

extern objeto_t* jugador; /* variable que guarda la direccion del jugador */

/* FUNCIONES */

/* map.c */

void mapnew(Bool stair_up,Bool stair_down);
/* creacion del mapa stair_up tiene escalera para subir, stair_down para bajar */

localidad_t* mapget(int r,int c);
/* consigue las caracteristicas de la localidad en la columna fila pedidas */

u1 maprndpos(int* r,int* c,Bool pasadizo);
/* busca una posicion transitable, si pasadizo=true->habitacion y pasadizo, sino solo pasadizo */

/* pantalla.c */

void panshw(int mri,int mci,int mrs,int mcs,int ro,int co);
/*muestra un sector del mapa que empieza en las coordenadas mri,mci
 * con dimensiones mrs,mcs y la posicion inicial en la pantalla terminal es ro,co */

/* mensaje.c */

void mensaje(char* men);
/* se introduce un mensaje que se separara por trozos */

/* menu.c */

uint menu(char* cabecera,uint opciones,char* opcion[]);
/* se crea un menu, funciona con las teclas i: subir j: bajar q: quitar s: seleccionar.
 * Devuelve la opcion selecconada o opciones en caso de no selecionar nada */

/* objeto.c */

objeto_t* objnew(char* nom,atributo_t atr,Bool npc,Bool jug);
/* definimos objeto nuevo poniendo el nombre y diciendo si es o no un npc */

Bool objinipos(objeto_t* obj,int r,int c);
/* damos posicion inicial a un objeto 
 * no puede existir ningun objeto en la misma posicion y debe ser zona transitable del mapa */

uint objfnd(objeto_t* obj[],Condicion cond);
/* se busca una serie de objetos que cumplan una determinada condicion */

Bool objmov(objeto_t* obj,int dr,int dc);
/* movemos un objeto haciendo este desplazamiento (solo npc's moviles) */

uint ojbinv(objeto_t* obj,objeto_t* con[]);
/* se da el inventario de un objeto npc */

Bool objcog(objeto_t* obj,objeto_t* itm);
/* un objeto npc coge un objeto */

Bool objdej(objeto_t* obj,objeto_t* itm);
/* un objeto npc deja un objeto */


/* rogue.c */


#endif /* ROGUE_H */
