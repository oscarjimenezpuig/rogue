/*
============================================================
  Fichero: pantalla.c
  Creado: 04-12-2025
  Ultima Modificacion: jue 18 dic 2025 13:23:41
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
	const atributo_t ATRNUL={' ',0,BLACK,BLACK};
	for(int r=0;r<mrs;r++) {
		ROW=ro+r;
		COL=co;
		for(int c=0;c<mcs;c++) {
			atributo_t a=ATRNUL;
			localidad_t* l=mapget(mri+r,mci+c);
			if(l && (l->vis) && (l->obs || l->trs)) {
				if(l->obs || l->trs==3) a=ATROBS;
				else if(l->trs==2) a=ATRPUE;
				else if(l->esc==1) a=ATRESU;
				else if(l->esc==-1) a=ATREBA;
				else if(l->trs==1 && l->hab) a=ATRHAB;
				else a=ATRPAS;
				if(l->vis==1) {
					a.ink=COLVIS;
					a.bkg=BLACK;
				}
			}
			ATR=a.atr;
			INK=a.ink;
			BKG=a.bkg;
			printc(a.chr);
		}
	}
}

static void objshw(objeto_t* o,int mri,int mci,int ro,int co) {
	int r=ro+o->r-mri;
	int c=co+o->c-mci;
	ROW=r;
	COL=c;
	ATR=o->atr.atr;
	INK=o->atr.ink;
	BKG=o->atr.bkg;
	printc(o->atr.chr);
}

static Bool isitm(objeto_t* o) {
	if(o && !o->npc) {
		localidad_t* l=mapget(o->r,o->c);
		return (l && l->vis==2)?TRUE:FALSE;
	}
	return FALSE;
}

static void itmshw(int mri,int mci,int ro,int co) {
	/* muestra todos los items */
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
		return (l && l->vis==2)?TRUE:FALSE;
	}
	return FALSE;
}

static void npcshw(int mri,int mci,int ro,int co) {
	/* muestra todos los npcs */
	objeto_t* npc[objsiz()];
	uint itms=objfnd(npc,isnpc);
	for(int k=0;k<itms;k++) {
		objeto_t* oe=npc[k];
		objshw(oe,mri,mci,ro,co);
	}
}

static void cajshw() {
	/* muestra las caracteristicas del jugador */
	char* const NAM[]={"Fue","Hab","Vel","Cap","Oro"};
	const uint SIZ=5;
	uint jca[]={jugador->fue,jugador->hab,jugador->vel,jugador->cap,jugador->oro};
	BKG=INK=BLACK;
	for(COL=0;COL<COLS;COL++) {
		ROW=ROWS-2;
		printc(' ');
	}
	ROW=ROWS-2;
	COL=0;
	INK=WHITE;
	for(int k=0;k<SIZ;k++) {
		ATR=BOLD;
		prints("%s: ",NAM[k]);
		ATR=NONE;
		prints("%i   ",jca[k]);
	}
}

static Bool isitijp(objeto_t* o) {
	return (o && o->npc==0 && mapget(o->r,o->c) && o->r==jugador->r && o->c==jugador->c);
}

static void itmsay() {
	/* dice todos los items que estan bajo el jugador */
	objeto_t* itm[objsiz()];
	uint itms=objfnd(itm,isitijp);
	if(itms) {
		INK=WHITE;
		ATR=BOLD;
		ROW=ROWS-1;
		COL=0;
		prints("Aqui puedes ver: ");
		ATR=NONE;
		int k=0;
		objeto_t* oe;
		for(;k<itms-1;k++) {
			oe=itm[k];
			prints("%s, ",oe->nom);
		}
		oe=itm[k];
		prints("%s",oe->nom);
	}
}

void panshw(int mri,int mci,int mrs,int mcs,int ro,int co) {
	cls();
	mapshw(mri,mci,mrs,mcs,ro,co);
	itmshw(mri,mci,ro,co);
	npcshw(mri,mci,ro,co);
	cajshw();
	itmsay();
	show();
}

