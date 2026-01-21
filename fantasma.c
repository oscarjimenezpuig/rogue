/*
============================================================
  Fichero: fantasma.c
  Creado: 21-01-2026
  Ultima Modificacion: mié 21 ene 2026 12:25:32
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

objeto_t* fantasma=NULL;

#define ATRFAN (atributo_t){'"',BOLD,BLACK,WHITE}

#define par rnd(0,MAPAR-1)
#define pac rnd(0,MAPAC-1)

static void fannew() {
	/* define el fantasma como objeto del nivel */
	fantasma=objnew("fantasma",ATRFAN,FALSE,FALSE);
	if(fantasma) {
		fantasma->fan=1;
		int r,c;
		localidad_t* l=NULL;
		do {
			r=par;
			c=pac;
			l=mapget(r,c);
		}while((l->obs+l->trs+l->vis+l->osc)==0);
		fantasma->r=fantasma->dr=r;
		fantasma->c=fantasma->dc=c;
	}
}

static Bool fancanmov(int* dir) {
	/* mira en que direcciones se puede mover el fantasma */
	int var[2]={SGN(fantasma->dr,fantasma->r),SGN(fantasma->dc,fantasma->c)};
	int pos=rnd(0,1);
	int tries=1;
another:
	if(var[pos]) {
		dir[pos]=var[pos];
		return TRUE;
	}
	pos=(pos+1)%2;
	if(tries) {
		--tries;
		goto another;
	}
	return FALSE;
}

static Bool cnpcvis(objeto_t* o) {
	/* condicion para detectar un npc vivo cercano al fantasma */
	return (o && o->npc && o->vid>0 && objdis(fantasma,o)<=SQV);
}

static Bool fanchk() {
	/* busca alrededor para ver si encuentra un npc para atacar */
	objeto_t* npcc[objsiz()];
	uint npccs=objfnd(npcc,cnpcvis);
	objeto_t* npcs=NULL;
	int dnpcs=-1;
	for(int k=0;k<npccs;k++) {
		objeto_t* oe=npcc[k];
		int doe=objdis(oe,fantasma);
		if(npcs==NULL || doe<dnpcs) {
			npcs=oe;
			dnpcs=doe;
		}
	}
	if(npcs) {
		if(dnpcs==0) {
			localidad_t* l=mapget(fantasma->r,fantasma->c);
			if(l->vis==2) menin("El fantasma arranca el alma a %s...",npcs->nom);
			objmue(npcs);
		} else {
			fantasma->dr=npcs->r;
			fantasma->dc=npcs->c;
		}
		return TRUE;
	}
	return FALSE;
}

static void fanmov() {
	/* mueve el fantasma a su destino */
	int dir[]={0,0};
	if(fancanmov(dir)) {
		fantasma->r+=dir[0];
		fantasma->c+=dir[1];
		fanchk();
	} else {
		fantasma->r=par;
		fantasma->c=pac;
	}
}

void fanact() {
	if(!fantasma && regla_fantasma()) {
		menin("Notas un escalofrio en el cuerpo... ");
		fannew();
	}
	if(fantasma) {
		fanmov();
	}
}
		


