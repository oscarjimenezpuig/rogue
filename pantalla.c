/*
============================================================
  Fichero: pantalla.c
  Creado: 04-12-2025
  Ultima Modificacion: mar 09 dic 2025 11:15:08
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
#define ATREDW (atributo_t){'<',BOLD,MAGENTA,BLACK} /* escalera de bajada */

#define COLVIS BLUE /*color de lo visibilizado */

static void showmap(int mri,int mci,int mrs,int mcs,int ro,int co) {
	for(int r=0;r<mrs;r++) {
		for(int c=0;c<mcs;c++) {
			localidad_t* l=mapget(mri+r,mci+c);
			if(l && (l->visible || l->visibilizado) && l->tipo!=VACIO) {
				atributo_t a={0,0,0,0};
				switch(l->tipo) {
					case OBSTACULO:
					case OCULTA:
						a=ATROBS;
						break;
					case TRANSITABLE:
						if(l->habitacion==0) a=ATRPAS;
						else if(l->escalera==-1) a=ATREDW;
						else if(l->escalera==1) a=ATRESU;
						else a=ATRHAB;
						break;
					case PUERTA:
						a=ATRPUE;
						break;
				}
				at(ro+r,co+c);
				attr(a.attr);
				if(l->visible) {
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

void showscr(int mri,int mci,int mrs,int mcs,int ro,int co) {
	cls();
	showmap(mri,mci,mrs,mcs,ro,co);
	show();
}

