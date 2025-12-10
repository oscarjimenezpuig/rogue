/*
============================================================
  Fichero: pantalla.c
  Creado: 04-12-2025
  Ultima Modificacion: dimarts, 9 de desembre de 2025, 19:01:39
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

void panshw(int mri,int mci,int mrs,int mcs,int ro,int co) {
	cls();
	mapshw(mri,mci,mrs,mcs,ro,co);
	show();
}

