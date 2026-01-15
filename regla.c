/*
============================================================
  Fichero: regla.c
  Creado: 28-12-2025
  Ultima Modificacion: jue 15 ene 2026 12:17:30
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

Bool regla_ataque(objeto_t* a,objeto_t* d) {
	int pfa=DAI+a->fue/PFA;
	int pfd=d->fue/CAR;
	objeto_t* armd=objisprt(d);
	if(armd) pfd+=armd->nar;
	return (pfa>pfd);
}

int regla_dano(objeto_t* a) {
	int mf=PFA;
	int mh=PHA;
	int dd=PDA;
	int nd=NDA;
	objeto_t* oa=objisves(a);
	if(oa) {
		mf=oa->pfu;
		mh=oa->pha;
		dd=oa->dad;
		nd=oa->nad;
	}
	int pe=0;
	if(mf>0) pe+=(a->fue/mf);
	if(mh>0) pe+=(a->hab/mh);
	for(int k=0;k<nd;k++) {
		switch(dd) {
			case 1:
				pe+=DAT;
				break;
			case 2:
				pe+=DAC;
				break;
			case 3:
				pe+=DAO;
				break;
			case 4:
				pe+=DAD;
				break;
			default:
				pe+=DAI;
				break;
		}
	}
	return pe;
}

Bool regla_arma_rotura(objeto_t* a) {
	int dados=a->tia+1;
	for(int k=0;k<dados;k++) {
		if(DAI!=1) return FALSE;
	}
	return TRUE;
}

Bool regla_proteccion_rotura(objeto_t* p) {
	int dados=p->tip+1;
	for(int k=0;k<dados;k++) {
		if(DAI!=1) return FALSE;
	}
	return TRUE;
}

static int nhab=-1;
static Bool eneinrom(objeto_t* o) {
	if(o && o->npc && !o->jug && o->vid>0) {
		localidad_t* l=mapget(o->r,o->c);
		if(l) {
			return (l->hab==nhab);
		}
	}
	return FALSE;
}

int regla_descanso() {
	if(jugador->vid<VMC) {
		localidad_t* l=mapget(jugador->r,jugador->c);
		if(l) {
			if(l->osc==1) return 2;
			else {
				nhab=l->hab;
				objeto_t* ene[objsiz()];
				int enes=objfnd(ene,eneinrom);
				if(enes) return 3;
				else return 0;
			}
		}
		return -1;
	} else return 1;
}


	
