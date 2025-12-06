/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: dissabte, 6 de desembre de 2025, 12:41:51
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define inmap(R,C) ((R)>=0 && (R)<MAPAR && (C)>=0 && (C)<MAPAC)

static u1 visibilidad() {
	/* determina la visibilidad del jugador */
	const int DR[]={-1,1,0,0};
	const int DC[]={0,0,-1,1};
	const int DD=4;
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
						if(l->habitacion==jh) l->visible=1;
					}
				}
			}
		} else {
			for(int k=0;k<DD;k++) {
				int nr=jr+DR[k];
				int nc=jc+DC[k];
				localidad_t* l=mapget(nr,nc);
				l->visible=1;
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

static  npc_t setnpcjug() {
	/* define caracteristicas del jugador */
	npc_t npc={10,1,1,1,1,2,0};
	return npc;
}

static void setnomjug(char* str) {
	/* funcion que escoge el nombre del jugador */
	at(0,0);
	prints("Cual es tu nombre? ");
	inmode(ECHO|ENTER|DELAY|CURSOR);
	listen();
	strbuf(NAMOBJLEN,str);
	inmode(DELAY);
}

static u1 check_puerta(int* r,int* c,u1 tipo) {
	/* comprueba si hay o no una puerta o pasadizo oculto */
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


//TODO: Programar el descubrimiento de la puerta
			
u1 jugnew() {
	char nombre[NAMOBJLEN];
	setnomjug(nombre);
	if(npcnew(OJUG,nombre,ATJUG,1,setnpcjug())) {
		PJUG->tipo|=JUGADOR;
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
		if(inkey('i') || inkey('j') || inkey('k') || inkey('l')) {
			ret=jugmov();
		}
	}
	if(ret) visibilidad();
	return 0;
}



		
		


