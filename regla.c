/*
============================================================
  Fichero: regla.c
  Creado: 28-12-2025
  Ultima Modificacion: diumenge, 28 de desembre de 2025, 22:15:40
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
