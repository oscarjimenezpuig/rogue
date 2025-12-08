/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 19:23:57
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define inmap(R,C) ((R)>=0 && (R)<MAPAR && (C)>=0 && (C)<MAPAC)

static u1 visibilidad() {
	/* determina la visibilidad del jugador */
	const int DR[]={0,-1,1,0,0};
	const int DC[]={0,0,0,-1,1};
	const int DD=5;
	int jr=PJUG->r;
	int jc=PJUG->c;
	int tr,tc;
	dimget(&tr,&tc);
	int ri=jr-tr/2;
	int rs=jr+tr/2;
	int ci=jc-tc/2;
	int cs=jc+tc/2;
	for(int r=ri;r<rs;r++) {
		for(int c=ci;c<cs;c++) {
			if(inmap(r,c)) {
				localidad_t* l=mapget(r,c);
				l->visible=0;
			}
		}
	}
	localidad_t* lj=mapget(jr,jc);
	if(lj->tipo==TRANSITABLE) {
		int jh=0;
		if((jh=lj->habitacion)!=0 && lj->oscuro==0) {
			for(int r=ri;r<rs;r++) {
				for(int c=ci;c<cs;c++) {
					if(inmap(r,c)) {
						localidad_t* l=mapget(r,c);
						if(l->habitacion==jh) l->visibilizado=l->visible=1;
					}
				}
			}
		} else {
			for(int k=0;k<DD;k++) {
				int nr=jr+DR[k];
				int nc=jc+DC[k];
				localidad_t* l=mapget(nr,nc);
				l->visibilizado=l->visible=1;
			}
		}
		return 1;
	}
	return 0;
}

#undef inmap

static u1 jugmov() {
	/* movimiento del jugador */
	int dr=PJUG->r;
	int dc=PJUG->c;
	if(inkey('i')) dr+=-1;
	else if(inkey('j')) dc+=-1;
	else if(inkey('l')) dc+=1;
	else if(inkey('k')) dr+=1;
	if(npccanpos(OJUG,dr,dc)) {
		PJUG->r=dr;
		PJUG->c=dc;
		visibilidad();
		return 1;
	}
	return 0;
}

u1 cogenjug(objeto_t o) {
	pobjeto_t po=objget(o);
	return (po && po->r==PJUG->r && po->c==PJUG->c && (po->tipo & COGIBLE));
}

static u1 jugcog() {
	/* opcion de coger del jugador */
	objeto_t occ[OBJETOS];
	u2 occs=objsfnd(occ,cogenjug);
	if(occs) {
		objeto_t sel;
		if(occs==1) sel=*occ;
		else {
			char* nom[occs];
			for(int k=0;k<occs;k++) {
				pobjeto_t p=objget(occ[k]);
				nom[k]=p->nombre;
			}
			u1 num=menu("Escoge objeto para coger",occs,nom);
			if(num==occs) return 0;
			else sel=occ[num];
		}
		if(npccog(OJUG,sel)) {
			mensaje("Lo coges...");
			return 1;
		} else {
			mensaje("No puedes coger eso");
		}
	} else {
		mensaje("Nada para coger aqui...");
	}
	return 0;
}

static u1 dejanjug(objeto_t o) {
	pobjeto_t po=objget(o);
	return (po && (po->tipo & COGIBLE) && po->item.contenedor==OJUG);
}

static u1 jugdej() {
	/* opcion de dejar del jugador */
	objeto_t occ[OBJETOS];
	u2 occs=objsfnd(occ,dejanjug);
	if(occs) {
		objeto_t sel;
		if(occs==1) sel=*occ;
		else {
			char* nom[occs];
			for(int k=0;k<occs;k++) {
				pobjeto_t p=objget(occ[k]);
				nom[k]=p->nombre;
			}
			u1 num=menu("Escoge objeto para dejar",occs,nom);
			if(num==occs) return 0;
			else sel=occ[num];
		}
		if(npcdej(OJUG,sel)) {
			mensaje("Lo dejas...");
			return 1;
		} else {
			mensaje("No puedes dejar eso...");
		}
	} else {
		mensaje("No tienes nada...");
	}
	return 0;
}

static u1 juginv() {
	objeto_t occ[OBJETOS];
	u2 occs=objsfnd(occ,dejanjug);
	if(occs) {
		cls();
		at(0,0);
		attr(BOLD);
		prints("INVENTARIO");
		at(1,0);
		attr(0);
		prints("Tienes (%i/%i)",occs,PJUG->npc.capacidad);
		for(int k=0;k<occs;k++) {
			pobjeto_t po=objget(occ[k]);
			at(k+2,1);
			prints("- %s",po->nombre);
		}
		while(inkey('\n')==0) listen();
	} else mensaje("No tienes nada...");
	return 1;
}

static  npc_t setnpcjug() {
	/* define caracteristicas del jugador */
	npc_t npc={10,1,1,5,1,2,0};
	return npc;
}

static void setnomjug(char* str) {
	/* funcion que escoge el nombre del jugador */
	at(0,0);
	attr(BOLD);
	prints("Cual es tu nombre? ");
	attr(0);
	inmode(ECHO|ENTER|DELAY|CURSOR);
	listen();
	strbuf(NAMOBJLEN,str);
	inmode(DELAY);
}

static u1 check_puerta(int* r,int* c,u1 tipo) {
	/* comprueba si hay o no una puerta o pasadizo oculto en la vecindad */
	const int DR[]={-1,1,0,0};
	const int DC[]={0,0,-1,1};
	const int SIZ=4;
	int jr=PJUG->r;
	int jc=PJUG->c;
	for(int k=0;k<SIZ;k++) {
		int rr=jr+DR[k];
		int cc=jc+DC[k];
		localidad_t* l=mapget(rr,cc);
		if(l && l->tipo==tipo) {
			*r=rr;
			*c=cc;
			return 1;
		}
	}
	return 0;
}

static void open_oculta() {
	/* para abrir puerta oculta se necesita habilidad */
	int pr,pc;
	if(check_puerta(&pr,&pc,OCULTA)) {
		u1 need=rnd(1,MAXCAR);
		if(PJUG->npc.habilidad>need) {
			mensaje("Has detectado una puerta oculta...");
			localidad_t* l=mapget(pr,pc);
			l->tipo=TRANSITABLE;
		}
	}
}

static u1 jughaskey(objeto_t o) {
	pobjeto_t po=objget(o);
	return (po && (po->tipo & COGIBLE) && po->item.contenedor==OJUG && (po->tipo & LLAVE) && po->item.abrepuerta);
}

static u1 jugabr() {
	/* abrir una puerta, necesita llave */
	int pr,pc;
	if(check_puerta(&pr,&pc,PUERTA)) {
		objeto_t oll[OBJETOS];
		u2 olls=objsfnd(oll,jughaskey);
		if(olls) {
			localidad_t* l=mapget(pr,pc);
			l->tipo=TRANSITABLE;
			mensaje("Abres la puerta...");
			if(rnd(0,PRLL)==0) {
				mensaje("... pero la llave se ha roto...");
				pobjeto_t pll=objget(*oll);
				pll->item.contenedor=OBJNUL;
				return 1;
			}
		} else {
			mensaje("No tienes llave...");
		}
	} else {
		mensaje("Ninguna puerta cercana para abrir...");
	}
	return 0;
}

static u1 jugfrz() {
	/* fuerza la puerta, necesitas fuerza y habilidad */
	int pr,pc;
	if(check_puerta(&pr,&pc,PUERTA)) {
		u1 funec=rnd(1,MAXCAR);
		u1 hanec=rnd(1,MAXCAR);
		mensaje("Fuerzas la puerta...");
		if(PJUG->npc.fuerza>funec && PJUG->npc.habilidad>hanec) {
			mensaje("... y la consigues abrir...");
			localidad_t* l=mapget(pr,pc);
			l->tipo=TRANSITABLE;
			return 1;
		} else {
			mensaje("... pero no consigues abrirla...");
		}
	} else {
		mensaje("Aqui no hay nada que pueda forzar...");
	}
	return 0;
}

static u1 keynew() {
	/* se define la llave inicial que lleva el jugador */
	atributo_t allave={'?',BOLD,CYAN,BLACK};
	item_t illave;
	illave.contenedor=OJUG;
	illave.abrepuerta=1;
	if(itmnew(OLLV,"Llave",COGIBLE|LLAVE,allave,illave)) {
		pobjeto_t pll=objget(OLLV);
		pll->r=pll->c=-1;
		return 1;
	}
	return 0;
}

u1 jugnew() {
	char nombre[NAMOBJLEN];
	setnomjug(nombre);
	if(npcnew(OJUG,nombre,ATJUG,1,setnpcjug())) {
		PJUG->tipo|=JUGADOR;
		if(keynew() && PJUG->npc.capacidad==0) PJUG->npc.capacidad=1;
		if(npcrndpos(OJUG,HABITACION)) {
			return visibilidad();
		}
	}
	return 0;
}

u1 jugact() {
	u1 ret=0;
	if(npccanact(OJUG)) {
		listen();
		if(inkey(TARR) || inkey(TABJ) || inkey(TIZQ) || inkey(TDER)) {
			ret=jugmov();
			open_oculta();
		} else if(inkey(TCOG)) {
			ret=jugcog();
		} else if(inkey(TDEJ)) {
			ret=jugdej();
		} else if(inkey(TINV)) {
			ret=juginv();
		} else if(inkey(TABR)) {
			ret=jugabr();
		} else if(inkey(TFRZ)) {
			ret=jugfrz();
		}
	}
	if(ret) visibilidad();
	return 0;
}



		
		


