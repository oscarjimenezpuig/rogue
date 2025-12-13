/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: vie 12 dic 2025 11:17:42
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
	int tr,tc;
	dimget(&tr,&tc);
	*rs=tr-3;
	*ri=jr-*rs/2;
	*cs=tc;
	*ci=jc-*cs/2;
}	

static void visset() {
	/* determinamos la visibilidad del jugador */
	const int DSP[]={1,-1,0,0};
	const uint SIZ=4;
	int ri,ci,rs,cs;
	recjug(&ri,&ci,&rs,&cs);
	for(int r=ri;r<rs;r++) {
		for(int c=ci;c<cs;c++) {
			localidad_t* l=mapget(r,c);
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
			for(int k=0;k<SIZ;k++) {
				int nr=jugador->r+DSP[k];
				int nc=jugador->c+DSP[SIZ-k-1];
				localidad_t* l=mapget(nr,nc);
				if(l) l->vis=2;
			}
		}
	}
}

static void nomset() {
	/* funcion que escoge el nombre del jugador */
	at(0,0);
	attr(BOLD);
	prints("Cual es tu nombre? ");
	attr(0);
	inmode(ECHO|ENTER|DELAY|CURSOR);
	listen();
	strbuf(SLEN,jugador->nom);
	inmode(NORMAL);
}

static void carset() {
	/* funcion que da las caracteristicas de jugador */
	jugador->fue=3;
	jugador->hab=3;
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
	const char KECH[]=KEYN;
	for(int k=0;k<KEYS;k++) {
		if(inkey(KECH[k])) return k;
	}
	return -1;
}

static Bool jugmov(int ckey) {
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

static Bool jugqut() {
	jugador=NULL;
	return TRUE;
}

Bool jugact() {
	if(objcanact(jugador)) {
		listen();
		int ckey=chkkey();
		switch(ckey) {
			case 0:
			case 1:
			case 2:
			case 3:
				return jugmov(ckey);
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

	
				




	
