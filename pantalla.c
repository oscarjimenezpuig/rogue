/*
============================================================
  Fichero: pantalla.c
  Creado: 04-12-2025
  Ultima Modificacion: dimecres, 10 de desembre de 2025, 19:37:33
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

/* atributos del mapa */
#define ATROBS (atributo_t){' ',REVERSE,RED,RED} /* atributo de las paredes */
#define ATRHAB (atributo_t){'.',0,WHITE,BLACK} /* atributo de suelo de habitacion */
#define ATRPAS (atributo_t){'#',DIM,WHITE,BLACK} /* atributo de pasadizo de habitacion */
#define ATRPUE (atributo_t){'?',REVERSE,YELLOW,BLACK} /* atributo de la puerta */
#define ATRESU (atributo_t){'>',BOLD,MAGENTA,BLACK} /* escalera de subida */
#define ATREBA (atributo_t){'<',BOLD,MAGENTA,BLACK} /* escalera de bajada */

#define COLVIS BLUE /*color de lo visibilizado */

static void mapshw(int mri,int mci,int mrs,int mcs,int ro,int co) {
	for(int r=0;r<mrs;r++) {
		for(int c=0;c<mcs;c++) {
			localidad_t* l=mapget(mri+r,mci+c);
			if(l && (l->vis) && (l->obs || l->trs)) {
				atributo_t a={0,0,0,0};
				if(l->obs || l->trs==3) a=ATROBS;
				else if(l->trs==2) a=ATRPUE;
				else if(l->esc==1) a=ATRESU;
				else if(l->esc==-1) a=ATREBA;
				else if(l->trs==1 && l->hab) a=ATRHAB;
				else a=ATRPAS;
				at(ro+r,co+c);
				attr(a.atr);
				if(l->vis==2) {
					ink(a.ink);
					background(a.bkg);
				} else {
					ink(COLVIS);
					background(BLACK);
				}
				printc(a.chr);
			}
		}
	}
}

static void objshw(objeto_t* o,int mri,int mci,int ro,int co) {
	int r=ro+o->r-mri;
	int c=co+o->c-mci;
	at(r,c);
	attr(o->atr.atr);
	ink(o->atr.ink);
	background(o->atr.bkg);
	printc(o->atr.chr);
}

static Bool isitm(objeto_t* o) {
	if(o && o->npc==0) {
		localidad_t* l=mapget(o->r,o->c);
		return (l!=NULL && l->vis==2)?TRUE:FALSE;
	}
	return FALSE;
}

static void itmshw(int mri,int mci,int ro,int co) {
	objeto_t* itm[objsiz()];
	uint itms=objfnd(itm,isitm);
	for(int k=0;k<itms;k++) {
		objeto_t* oe=itm[k];
		objshw(oe,mri,mci,ro,co);
	}
}

static Bool isnpc(objeto_t* o) {
	if(o && o->npc) {
		localidad_t* l=mapget(o->r,o->c);
		return (l!=NULL && l->vis==2)?TRUE:FALSE;
	}
	return FALSE;
}

static void npcshw(int mri,int mci,int ro,int co) {
	objeto_t* npc[objsiz()];
	uint itms=objfnd(npc,isnpc);
	for(int k=0;k<itms;k++) {
		objeto_t* oe=npc[k];
		objshw(oe,mri,mci,ro,co);
	}
}

void panshw(int mri,int mci,int mrs,int mcs,int ro,int co) {
	cls();
	mapshw(mri,mci,mrs,mcs,ro,co);
	itmshw(mri,mci,ro,co);
	npcshw(mri,mci,ro,co);
	show();
}

