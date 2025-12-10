/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: mié 10 dic 2025 15:26:36
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
#define TQUT 'Q' /* tecla finalizar juego 7*/
#define TABR 'a' /* abrir una puerta 8*/
#define TFRZ 'f' /* forzar la puerta 9*/

#define KEYN {TARR,TABJ,TIZQ,TDER,TCOG,TDEJ,TINV,TQUT,TABR,TFRZ};
#define KEYS 10

/* posicion inicial pantalla */
#define RO 1
#define CO 0

Bool quit=FALSE;


static void visset() {
	/* determinamos la visibilidad del jugador */
	const int DSP[]={1,-1,0,0};
	const uint SIZ=4;
	int jr=jugador->r;
	int jc=jugador->c;
	int tr,tc;
	dimget(&tr,&tc);
	int ri=jr-tr/2;
	int rs=jr+tr/2;
	int ci=jc-tc/2;
	int cs=jc+tc/2;
	for(int r=ri;r<rs;r++) {
		for(int c=ci;c<cs;c++) {
			localidad_t* l=mapget(r,c);
			if(l) l->vis=(l->vis==2)?1:(l->vis);
		}
	}
	localidad_t* lj=mapget(jr,jc);
	if(lj && lj->trs==1) {
		lj->vis=2;
		int hab=lj->hab;
		if(hab!=0 && lj->osc==0) {
			for(int r=ri;r<rs;r++) {
				for(int c=ci;c<cs;c++) {
					localidad_t* l=mapget(r,c);
					if(l && l->hab==hab) l->vis=2;
				}
			}
		} else {
			for(int k=0;k<SIZ;k++) {
				int nr=jr+DSP[k];
				int nc=jc+DSP[SIZ-k-1];
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
	inmode(DELAY);
}

static void carset() {
	/* funcion que da las caracteristicas de jugador */
	jugador->fue=3;
	jugador->hab=3;
	jugador->cve=jugador->vel=3;
	jugador->cap=3;
}

static void jugpos() {
	/* funcion que da la posicion del jugador */
	int r,c;
	if(maprndpos(&r,&c,FALSE)) {
		jugador->r=r;
		jugador->c=c;
	}
}

Bool jugnew() {
	objnew("",JATR,TRUE,TRUE);
	if(jugador) {
		nomset();
		carset();
		visset();
		jugpos();
		return TRUE;
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
	quit=TRUE;
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
			case 7:
				return jugqut();
		}
	}
	return FALSE;
}

Bool jugshw() {
	localidad_t* l=NULL;
	if(jugador && (l=mapget(jugador->r,jugador->c))) {
		int jr=jugador->r;
		int jc=jugador->c;
		int tr,tc;
		dimget(&tr,&tc);
		int ri=jr-tr/2;
		int rs=tr-3;
		int ci=jc-tc/2;
		int cs=tc;
		panshw(ri,ci,rs,cs,RO,CO);
		return TRUE;
	}
	return FALSE;
}

	
				




	
