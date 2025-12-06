/*
============================================================
  Fichero: pantalla.c
  Creado: 04-12-2025
  Ultima Modificacion: dissabte, 6 de desembre de 2025, 12:28:08
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

static void showmap(int mri,int mci,int mrs,int mcs,int ro,int co) {
	for(int r=0;r<mrs;r++) {
		for(int c=0;c<mcs;c++) {
			localidad_t* l=mapget(mri+r,mci+c);
			if(l && l->visible && l->tipo!=VACIO) {
				atributo_t a={0,0,0,0};
				switch(l->tipo) {
					case OBSTACULO:
					case OCULTA:
						a=ATROBS;
						break;
					case TRANSITABLE:
						if(l->habitacion==0) a=ATRPAS;
						else a=ATRHAB;
						break;
					case PUERTA:
						a=ATRPUE;
						break;
				}
				at(ro+r,co+c);
				attr(a.attr);
				ink(a.ink);
				background(a.bkg);
				printc(a.chr);
			}
		}
	}
}

static u1 objisinscr(objeto_t o,int mri,int mci,int mrs,int mcs) {
	pobjeto_t po=objget(o);
	int r=po->r;
	int c=po->c;
	localidad_t* l=mapget(r,c);
	return (l->visible && r>=mri && r<mri+mrs && c>=mci && c<mci+mcs);
}

static u1 isnpc(objeto_t o) {
	pobjeto_t po=objget(o);
	return (po && (po->tipo & NPC));
}

static void objprt(objeto_t o,int mri,int mci,int ro,int co) {
	pobjeto_t po=objget(o);
	int r=po->r;
	int c=po->c;
	int prr=r-mri+ro;
	int prc=c-mci+co;
	at(prr,prc);
	attr(po->atributo.attr);
	ink(po->atributo.ink);
	background(po->atributo.bkg);
	printc(po->atributo.chr);
}

static void shownpc(int mri,int mci,int mrs,int mcs,int ro,int co) {
	/* se muestran los npcs */
	objeto_t npc[OBJETOS];
	u2 npcs=objsfnd(npc,isnpc);
	for(int k=0;k<npcs;k++) {
		if(objisinscr(npc[k],mri,mci,mrs,mcs)) objprt(npc[k],mri,mci,ro,co);
	}
}

void showscr(int mri,int mci,int mrs,int mcs,int ro,int co) {
	cls();
	showmap(mri,mci,mrs,mcs,ro,co);
	shownpc(mri,mci,mrs,mcs,ro,co);
	show();
}

