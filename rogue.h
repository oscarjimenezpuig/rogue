/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: mar 09 dic 2025 14:20:53
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h> //dbg

#include "curse.h"

/* CONSTANTES */

#define EOS '\0' /* final de string */

/* tipos de localidad */
#define VACIO 0
#define OBSTACULO 1 /* pared */
#define TRANSITABLE 2 /* camino o habitacion */
#define PUERTA 3 /* lugar de una puerta */
#define OCULTA 4 /* lugar de una puerta oculta (en principio, como pared) */

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

/* TIPOS */

typedef unsigned int uint;

typedef struct {
	char chr;
	struct {
		uint attr : 6 
		uint ink : 3;
		uint bkg : 3;
	};
} atributo_t;

typedef struct {
	uint obs : 1; /* 1: obstaculo */
	uint trs : 2; /* 1: transitable normal, 2 : puerta, 3: oculta */
	uint vis : 2; /* 1: visibilizado 2: visible actual */
	uint osc : 1; /* 1: oscura */
	uint hab : 4; /* numero habitacion */
	int esc : 2; /* -1: escalera descenso, 1: escalera ascenso */
} localidad_t;

typedef struct {
	u1 tipo : 3;
	u1 visible : 1;
	u1 oscuro : 1;
	u1 habitacion : 4; /* si habitacion es 0, se corresponde a camino */
	u1 visibilizado : 1;
	s1 escalera : 2; /* escalera -1 bajada, 1 subida, 0 sin escalera */
} localidad_t;
		
/* VARIABLES */

/* FUNCIONES */

/* objeto.c */

/* map.c */

void mapnew(u1 level);
/* creacion del mapa de un nivel */

localidad_t* mapget(int r,int c);
/* consigue las caracteristicas de la localidad en la columna fila pedidas */

u1 maprndpos(int* r,int* c,);
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
