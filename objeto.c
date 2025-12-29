/*
============================================================
  Fichero: objeto.c
  Creado: 09-12-2025
  Ultima Modificacion: dilluns, 29 de desembre de 2025, 09:53:48
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define OBJETOS 64 /* numero de objetos maximos en un nivel */

objeto_t* jugador=NULL;

static objeto_t objeto[OBJETOS];
static uint objetos=1;

void objclr() {
	objetos=1;
}

uint objsiz() {
	return objetos;
}

objeto_t* objnew(char* n,atributo_t a,Bool npc,Bool jug) {
	objeto_t* new=NULL;
	if(objetos<OBJETOS) {
		new=(npc && jug)?objeto:objeto+objetos++;
		char* p=n;
		char* q=new->nom;
		while(*p!=EOS && p-n<SLEN) *q++=*p++;
		*q=EOS;
		new->atr=a;
		new->r=new->c=-1;
		if(npc) {
			new->npc=1;
			if(jug) {
				jugador=new;
				new->jug=new->mov=1;
			} else new->jug=new->mov=0;
			new->fue=new->hab=new->vel=new->cap=new->cve=0;
			new->ata=0;
			new->oro=0;
		} else {
			new->npc=0;
			new->cog=new->ior=new->arm=new->lla=new->ani=new->ves=0;
			new->con=NULL;
		}
	}
	return new;
}

objeto_t* objcpy(struct objeto_s objcar) {
	Bool inpc=objcar.npc;
	Bool ijug=(inpc)?objcar.jug:FALSE;
	objeto_t* o=objnew(objcar.nom,objcar.atr,inpc,ijug);
	if(o) *o=objcar;
	return o;
}

Bool objinipos(objeto_t* o,int r,int c) {
	if(o) {
		localidad_t* l=mapget(r,c);
		if(l && l->trs==1) {
			if(o->npc) {
				for(int k=0;k<objetos;k++) {
					objeto_t* oe=objeto+k;
					if(oe->npc && oe->r==r && oe->c==c) return FALSE;
				}
			}
			o->r=r;
			o->c=c;
			return TRUE;
		}
	}
	return FALSE;
}
				

uint objfnd(objeto_t* o[],Condicion c) {
	objeto_t* p=objeto;
	objeto_t** po=o;
	while(p!=objeto+objetos) {
		if(c(p)) {
			*po=p;
			po++;
		}
		p++;
	}
	return po-o;
}

static objeto_t* npce=NULL;

static Bool isnpcinp(objeto_t* o) {
	/* condicion de busqueda de npc en una posicion */
	return (o->npc && o->r==npce->r && o->c==npce->c);
}

static Bool isinv(objeto_t* o) {
	/* objetos poseidos por npce */
	return (o->npc==0 && o->con==npce);
}

Bool objmov(objeto_t* o,int dr,int dc) {
	if(o && o->npc && o->mov) {
		localidad_t* l=mapget(o->r,o->c);
		if(l) {
			int nr=o->r+dr;
			int nc=o->c+dc;
			localidad_t* nl=mapget(nr,nc);
			if(nl && nl->trs==1) {
				npce=o;
				objeto_t* npc[objetos];
				uint npcs=objfnd(npc,isnpcinp);
				if(npcs==1) {
					o->r=nr;
					o->c=nc;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

uint objinv(objeto_t* o,objeto_t* c[]) {
	if(o && o->npc) {
		npce=o;
		return objfnd(c,isinv);
	}
	return 0;
}

#define oij (o==jugador)

Bool objcog(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		objeto_t* inv[objetos];
		uint cinv=objinv(o,inv);
		if(cinv<o->cap) {		
			if(itm && itm->npc==0 && (itm->cog) && itm->r==o->r && itm->c==o->c && itm->con==NULL) {
				if(!itm->prt || !objisprt(o)) {
					itm->r=itm->c=-1;
					if(itm->ior) {
						if(oij) menin("Coges %i monedas de oro...",itm->cor);
						o->oro+=itm->cor;
					} else if(itm->prt) {
						if(oij) menin("Cogers %s y te la pones...",itm->nom);
					} else {
						if(oij) menin("Coges %s...",itm->nom);
						itm->con=o;
					}
					return TRUE;
				} else {
					if(oij) menin("No puedes coger una armadura, ya llevas una...");
				}
			}
		} else {
			if(oij) menin("Llevas demasiadas cosas...");
		}
	}
	if(oij) menin("No puedes cogerlo...");
	return FALSE;
}

Bool objdej(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		localidad_t* l=mapget(o->r,o->c);
		if(l && itm && itm->npc==0 && itm->con==o) {
			if(!itm->ves) {
				itm->con=NULL;
				itm->r=o->r;
				itm->c=o->c;
				if(oij) menin("Dejas %s...",itm->nom);
				return TRUE;
			} else {
				if(oij) menin("Debes quitartelo antes de dejarlo...");
			}
		}
	}
	if(oij) menin("No puedes dejarlo...");
	return FALSE;
}

objeto_t* objisprt(objeto_t* o) {
	objeto_t* inv[objsiz()];
	uint invs=objinv(o,inv);
	for(int k=0;k<invs;k++) {
		if(inv[k]->prt) return inv[k];
	}
	return NULL;
}

objeto_t* objisves(objeto_t* o) {
	objeto_t* inv[objsiz()];
	uint invs=objinv(o,inv);
	for(int k=0;k<invs;k++) {
		if(inv[k]->ves) return inv[k];
	}
	return NULL;
}

Bool objves(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		if(itm && !itm->npc && itm->con==o && itm->arm && !itm->ves) {
			if(!objisves(o)) {
				itm->ves=1;
				if(oij) menin("Te pones %s...",itm->nom);
				return TRUE;
			} else {
				if(oij) menin("No puedes llevar puesto nada mas...");
			}
		} else {
			if(oij) menin("No te puedes poner eso...");
		}
	}
	return FALSE;
}

Bool objdes(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		objeto_t* op=objisves(o);
		if(op==itm) {
			itm->ves=0;
			if(oij) menin("Te quitas %s...",itm->nom);
			return TRUE;
		} else if(oij) menin("No llevas eso puesto...");
	}
	return FALSE;
}

#undef oij

Bool objcanact(objeto_t* o) {
	if(o && o->npc) {
		localidad_t* l=mapget(o->r,o->c);
		if(l) {
			if(o->cve==o->vel) {
				o->cve=0;
				return TRUE;
			} else {
				o->cve++;
			}
		}
	}
	return FALSE;
}








	
