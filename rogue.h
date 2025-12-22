/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: dilluns, 22 de desembre de 2025, 21:52:27
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h>

#include "curse.h"

/* CONSTANTES */

/* generales */
#define EOS '\0' /* final de string */
#define SLEN 20 /* longitud de los string definidos */

/* dimensiones del mapa */
#define MAPAC 159
#define MAPAR 48

/* definicion de booleanos */
#define TRUE 1
#define FALSE 0

/* reglas */
#define VMC 15 /* valor maximo de cualquier caracteristica */
#define HmO 1 /* habilidad minima necesaria para abrir una puerta (sube una por nivel)*/
#define HmF 3 /* habilidad minima para forzar la puerta (sube una por  nivel) */
#define FmF 2 /* fuerza minima para forzar la puerta (sube una por nivel) */
#define PLR 4 /* probabilidad de que se rompa una llave */
#define OXD 1 /* oro por cada 10 cuadrados de transitable */
#define OML 9 /* maximo de sacos de oro por nivel */
#define OmL 1 /* minimo de sacos por nivel */

/* MACROS */

#define MAX(A,B) ((A)>(B))?(A):(B)
#define MIN(A,B) ((A)>(B))?(B):(A)

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
	char nom[SLEN+1];
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
			uint cog : 1; /* 1: es cogible */
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

extern uint nivel; /* planta en la que se encuentra el jugador */

/* FUNCIONES */

/* map.c */

void mapnew(Bool stair_up,Bool stair_down);
/* creacion del mapa stair_up tiene escalera para subir, stair_down para bajar */

localidad_t* mapget(int r,int c);
/* consigue las caracteristicas de la localidad en la columna fila pedidas */

Bool maprndpos(int* r,int* c,Bool pasadizo);
/* busca una posicion transitable, si pasadizo=true->habitacion y pasadizo, sino solo pasadizo */

uint mapngh(int r,int c,localidad_t* ln[4]);
/* da las cuatro localidades vecinas a la posicion dada N,S,E,O
 * en caso de que este fuera del mapa, la localidad sera NULL
 * se devuelve el numero de localidades diferentes a NULL*/

/* pantalla.c */

void panshw(int mri,int mci,int mrs,int mcs,int ro,int co);
/*muestra un sector del mapa que empieza en las coordenadas mri,mci
 * con dimensiones mrs,mcs y la posicion inicial en la pantalla terminal es ro,co */

/* mensaje.c */

void mensaje(const char* men,...);
/* se introduce un mensaje que se separara por trozos */

/* menu.c */

uint menu(char* cabecera,uint opciones,char* opcion[]);
/* se crea un menu, funciona con las teclas i: subir j: bajar q: quitar s: seleccionar.
 * Devuelve la opcion selecconada o opciones en caso de no selecionar nada */

/* objeto.c */

uint objsiz();
/* dice el numero de objetos que se han definido */

objeto_t* objnew(char* nom,atributo_t atr,Bool npc,Bool jug);
/* definimos objeto nuevo poniendo el nombre y diciendo si es o no un npc */

Bool objinipos(objeto_t* obj,int r,int c);
/* damos posicion inicial a un objeto 
 * no puede existir ningun objeto en la misma posicion y debe ser zona transitable del mapa */

uint objfnd(objeto_t* obj[],Condicion cond);
/* se busca una serie de objetos que cumplan una determinada condicion */

Bool objmov(objeto_t* obj,int dr,int dc);
/* movemos un objeto haciendo este desplazamiento (solo npc's moviles) */

uint objinv(objeto_t* obj,objeto_t* con[]);
/* se da el inventario de un objeto npc */

Bool objcog(objeto_t* obj,objeto_t* itm);
/* un objeto npc coge un objeto */

Bool objdej(objeto_t* obj,objeto_t* itm);
/* un objeto npc deja un objeto */

Bool objcanact(objeto_t* obj);
/* dice si un objeto npc esta en disposicion de actuar */

/* jugador.c */

Bool jugnew();
/* definicion del jugador */

Bool jugact();
/* accion del jugador determinada por el teclado */

Bool jugshw();
/* se muestra la pantalla cogiendo como centro la posicion del jugador */

/* item.c */

void llplev();
/* crea todas las llaves de un nivel en funcion de las puertas que hay */

void orolev();
/* crea el oro por nivel */

void rhrlev();
/* creacion del red herring, objeto que no sirve para nada (uno por nivel) */

/* rogue.c */


#endif /* ROGUE_H */
