/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: jue 18 dic 2025 14:37:21
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

/* atributos */
#define JATR (atributo_t){'@',BOLD,WHITE,BLACK}

/* teclado */
#define TARR 'i' /* arriba 0*/
#define TABJ 'k' /* abajo 1*/
#define TIZQ 'j' /* izquierda 2*/
#define TDER 'l' /* derecha 3*/
#define TCOG 'c' /* coger 4*/
#define TDEJ 'd' /* dejar 5*/
#define TINV 'I' /* inventario 6*/
#define TABR 'a' /* abrir una puerta 7*/
#define TFRZ 'f' /* forzar la puerta 8*/
#define TQUT 'Q' /* tecla finalizar juego 9*/

#define KEYN {TARR,TABJ,TIZQ,TDER,TCOG,TDEJ,TINV,TABR,TFRZ,TQUT};
#define KEYS 10

/* posicion inicial pantalla */
#define RO 1
#define CO 0

static void recjug(int* ri,int* ci,int* rs,int* cs) { 
	/* rectangulo de visibilidad donde el centro es la posicion del jugador */
	int jr=jugador->r;
	int jc=jugador->c;
	*rs=ROWS-3;
	*ri=jr-*rs/2;
	*cs=COLS;
	*ci=jc-*cs/2;
}	

static void visset() {
	/* determinamos la visibilidad del jugador */
	int ri,ci,rs,cs;
	recjug(&ri,&ci,&rs,&cs);
	for(int r=0;r<rs;r++) {
		for(int c=0;c<cs;c++) {
			localidad_t* l=mapget(r+ri,c+ci);
			if(l) l->vis=(l->vis)?1:0;
		}
	}
	localidad_t* lj=mapget(jugador->r,jugador->c);
	if(lj && lj->trs==1) {
		lj->vis=2;
		int hab=lj->hab;
		if(lj->osc==0) {
			for(int r=0;r<rs;r++) {
				for(int c=0;c<cs;c++) {
					localidad_t* l=mapget(r+ri,c+ci);
					if(l && l->hab==hab) l->vis=2;
				}
			}
		} else {
			localidad_t* n[4];
			mapngh(jugador->r,jugador->c,n);
			for(int k=0;k<4;k++) {
				if(n[k]) n[k]->vis=2;
			}
		}
	}
}

static void nomset() {
	/* funcion que escoge el nombre del jugador */
	INK=WHITE;
	ROW=COL=0;
	ATR=BOLD;
	prints("Cual es tu nombre? ");
	ATR=NONE;
	listen(INPUT);
	bufget(SLEN,jugador->nom);
}

static void carset() {
	/* funcion que da las caracteristicas de jugador */
	jugador->fue=4;
	jugador->hab=5;
	jugador->cve=jugador->vel=3;
	jugador->cap=3;
}

static Bool jugpos() {
	/* funcion que da la posicion del jugador */
	int r,c;
	if(maprndpos(&r,&c,FALSE)) {
		jugador->r=r;
		jugador->c=c;
		localidad_t* l=mapget(r,c);
		return (l!=NULL)?TRUE:FALSE;
	}
	return FALSE;
}

Bool jugnew() {
	objnew("",JATR,TRUE,TRUE);
	if(jugador) {
		nomset();
		carset();
		if(jugpos()) {
			visset();
			return TRUE;
		}
	}
	return FALSE;
}

static int chkkey() {
	/* funcion que detecta el caracter que se pulsa y lo pasa a un numero */
	const char KECH[]=KEYN;
	for(int k=0;k<KEYS;k++) {
		if(inkey(KECH[k])) return k;
	}
	return -1;
}

static Bool jugmov(int ckey) {
	/* funcion que mueve el jugador si se puede */
	const int DR[]={-1,1,0,0};
	const int DC[]={0,0,-1,1};
	int dr=DR[ckey];
	int dc=DC[ckey];
	if(objmov(jugador,dr,dc)) {
		visset();
		return TRUE;
	}
	return FALSE;
}

static Bool jugdsc() {
	/* busca las celdas vecinas y descubre las puertas ocultas */
	localidad_t* n[4];
	mapngh(jugador->r,jugador->c,n);
	localidad_t* pe=NULL;
	for(int k=0;k<4 && !pe;k++) {
		pe=n[k];
		if(pe && (pe->obs==1 || pe->trs!=3)) pe=NULL;
	}
	if(pe && rnd(HmO+nivel,VMC)<jugador->hab) {
		mensaje("Has descubierto una puerta oculta...");
		pe->trs=1;
		return TRUE;
	}
	return FALSE;
}

static Bool jugfrp() {
	/* accion de forzar la puerta de un jugador */
	localidad_t* n[4];
	mapngh(jugador->r,jugador->c,n);
	localidad_t* pe=NULL;
	for(int k=0;k<4 && !pe;k++) {
		pe=n[k];
		if(pe && (pe->trs!=2)) pe=NULL;
	}
	if(pe) {
		if(rnd(HmF+nivel,VMC)<jugador->hab && rnd(FmF+nivel,VMC)<jugador->fue) {
			mensaje("Has podido forzar la puerta...");
			pe->trs=1;
		} else mensaje("No has podido forzar la puerta...");
		return TRUE;
	} else mensaje("No veo cerca ninguna puerta para forzar...");
	return FALSE;
}

static Bool jugqut() {
	jugador=NULL;
	return TRUE;
}

Bool jugact() {
	if(objcanact(jugador) && !jugdsc()) {
		listen(INKEY);
		int ckey=chkkey();
		switch(ckey) {
			case 0:
			case 1:
			case 2:
			case 3:
				return jugmov(ckey);
			case 8:
				return jugfrp();
			case 9:
				return jugqut();
		}
	}
	return FALSE;
}

Bool jugshw() {
	localidad_t* l=NULL;
	if(jugador && (l=mapget(jugador->r,jugador->c))) {
		int ri,ci,rs,cs;
		recjug(&ri,&ci,&rs,&cs);
		panshw(ri,ci,rs,cs,RO,CO);
		return TRUE;
	}
	return FALSE;
}

	
				




	
