/*
============================================================
  Fichero: npc.c
  Creado: 05-12-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 19:38:26
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

u1 npcnew(objeto_t o,char* n,atributo_t a,u1 mov,npc_t npc) {
	u1 tipo=NPC|((mov)?MOVIL:0);
	if(objnew(o,tipo)) {
		objname(o,n);
		pobjeto_t po=objget(o);
		po->atributo=a;
		po->npc=npc;
		po->npc.vact=po->npc.velocidad;
		return 1;	
	}
	return 0;
}

static u1 isnpc(objeto_t o) {
	pobjeto_t po=objget(o);
	return (po && (po->tipo & NPC));
}

u2 npcfnd(pobjeto_t* pn) {
	objeto_t n[OBJETOS];
	u2 ns=objsfnd(n,isnpc);
	for(u2 k=0;k<ns;k++) {
		pn[k]=objget(n[k]);
	}
	return ns;
}

u1 npcinplace(int r,int c) {
	pobjeto_t po[OBJETOS];
	u2 pos=npcfnd(po);
	for(u2 k=0;k<pos;k++) {
		if(po[k]->r==r && po[k]->c==c) return 1;
	}
	return 0;
}		

u1 npccanpos(objeto_t o,int r,int c) {
	if((mapget(r,c)->tipo)!=TRANSITABLE) return 0;
	else if(npcinplace(r,c)) return 0;
	else return 1;
}

u1 npcpos(objeto_t o,int r,int c) {
	if(npccanpos(o,r,c)) {
		pobjeto_t po=objget(o);
		po->r=r;
		po->c=c;
		return 1;
	}
	return 0;
}

u1 npcrndpos(objeto_t o,u1 tt) {
	const u2 TRIES=MAPAA;
	u2 tries=TRIES;
	int r,c;
	while(tries--) {
		if(maprndpos(&r,&c,tt)) {
			if(!npcinplace(r,c)) {
				pobjeto_t po=objget(o);
				po->r=r;
				po->c=c;
				return 1;
			}
		}
	}
	return 0;
}

u2 npcinv(objeto_t o,objeto_t* c) {
	objeto_t* pc=c;
	for(objeto_t k=0;k<OBJETOS;k++) {
		pobjeto_t po=objget(k);
		if(po && po->tipo & COGIBLE && po->item.contenedor==o) {
			*pc++=k;
		}
	}
	return pc-c;
}

u1 npccancog(objeto_t o,objeto_t c) {
	pobjeto_t po=objget(o);
	pobjeto_t pc=objget(c);
	if(po && pc) {
		if(pc->tipo & COGIBLE && pc->item.contenedor==OBJNUL) {
			objeto_t cono[OBJETOS];
			return (npcinv(o,cono)<po->npc.capacidad);
		}
	}
	return 0;
}

u1 npccog(objeto_t o,objeto_t c) {
	if(npccancog(o,c)) {
		pobjeto_t pc=objget(c);
		if(pc->tipo & ORO) {
			pobjeto_t po=objget(o);
			po->npc.oro+=pc->item.oro;
		} else pc->item.contenedor=o;
		pc->r=pc->c=-1;
		return 1;
	}
	return 0;
}

u1 npccandej(objeto_t o,objeto_t c) {
	pobjeto_t po=objget(o);
	pobjeto_t pc=objget(c);
	return (po && pc && mapget(po->r,po->c)!=NULL && (pc->tipo & COGIBLE) && pc->item.contenedor==o);
}

u1 npcdej(objeto_t o,objeto_t c) {
	if(npccandej(o,c)) {
		pobjeto_t po=objget(o);
		pobjeto_t pc=objget(c);
		pc->item.contenedor=OBJNUL;
		pc->r=po->r;
		pc->c=po->c;
		return 1;
	}
	return 0;
}

static u1 isllav(objeto_t o) {
	/* selecciona los objetos llave que hay en el juego */
	pobjeto_t po=objget(o);
	return (po && (po->tipo & LLAVE) && po->item.abrepuerta && (po->tipo & COGIBLE) && (po->item.contenedor!=OBJNUL));
}

u1 npccanabr(objeto_t o,int* pr,int* pc,objeto_t* ll) {
	const int DSP[]={0,0,1,-1};
	const int SIZ=4;
	pobjeto_t po=objget(o);
	if(po) {
		*pr=*pc=-1;
		for(int k=0;k<SIZ;k++) {
			int ppr=po->r+DSP[k];
			int ppc=po->c+DSP[SIZ-1-k];
			localidad_t* l=mapget(ppr,ppc);
			if(l && l->tipo==PUERTA) {
				*pr=ppr;
				*pc=ppc;
				objeto_t oll[OBJETOS];
				u2 olls=objsfnd(oll,isllav);
				for(int k=0;k<olls;k++) {
					pobjeto_t pll=objget(oll[k]);
					if(pll && pll->item.contenedor==o) {
						*ll=oll[k];
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

u1 npcabr(int pr,int pc) {
	localidad_t* l=mapget(pr,pc);
	if(l && l->tipo==PUERTA) {
		l->tipo=TRANSITABLE;
		return 1;
	}
	return 0;
}

u1 npcpiella(objeto_t ll) {
	if(rnd(0,PRLL)==0) {
		pobjeto_t pll=objget(ll);
		pll->item.contenedor=OBJNUL;
		pll->r=pll->c=-1;
		return 1;
	}
	return 0;
}
			
u1 npccanact(objeto_t o) {
	pobjeto_t po=objget(o);
	if(po && po->tipo & NPC) {
		if(po->npc.vact==MAXCAR-po->npc.velocidad) {
			po->npc.vact=0;
			return 1;
		} else {
			po->npc.vact++;
		}
	}
	return 0;
}

	


