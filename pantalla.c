/*
============================================================
  Fichero: pantalla.c
  Creado: 04-12-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 19:51:45
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

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

static u1 objisinscr(objeto_t o,int mri,int mci,int mrs,int mcs) {
	pobjeto_t po=objget(o);
	int r=po->r;
	int c=po->c;
	localidad_t* l=mapget(r,c);
	return (l && l->visible && r>=mri && r<mri+mrs && c>=mci && c<mci+mcs);
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

static u1 isitm(objeto_t o) {
	/* comprueba los items que estan en el suelo */
	pobjeto_t po=objget(o);
	return (po && (po->tipo & NPC)==0);
}

static void showitm(int mri,int mci,int mrs,int mcs,int ro,int co) {
	/* se muestran los npcs */
	objeto_t itm[OBJETOS];
	u2 itms=objsfnd(itm,isitm);
	for(int k=0;k<itms;k++) {
		if(objisinscr(itm[k],mri,mci,mrs,mcs)) objprt(itm[k],mri,mci,ro,co);
	}
}

#define cj(A) PJUG->npc.A

static void showmarc() {
	/* muestra todos los puntos que tiene un jugador */
	const char NAM[]={'G','F','A','H','M','V','C','O'};
	const u1 SIZ=8;
	int val[]={cj(golpes),cj(fuerza),cj(armadura),cj(habilidad),cj(magia),cj(velocidad),cj(capacidad),cj(oro)};
	int dr,dc;
	dimget(&dr,&dc);
	at(dr-2,0);
	ink(7);
	for(int k=0;k<SIZ;k++) {
		attr(BOLD);
		printc(NAM[k]);
		attr(0);
		prints(": %i  ",val[k]);
	}
}		

void showscr(int mri,int mci,int mrs,int mcs,int ro,int co) {
	cls();
	showmap(mri,mci,mrs,mcs,ro,co);
	showitm(mri,mci,mrs,mcs,ro,co);
	shownpc(mri,mci,mrs,mcs,ro,co);
	showmarc();
	show();
}

