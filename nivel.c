/*
============================================================
  Fichero: nivel.c
  Creado: 23-12-2025
  Ultima Modificacion: mié 21 ene 2026 12:20:58
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define NIVS (NFI-NIN+1)

uint num_nivel=0;

static nivel_t nivel[NIVS];

static uint num_cha; /* numero de casillas habitacion en el mapa */
static uint num_cpa; /* numero de casillas pasadizo en el mapa */
static uint num_cpu; /* numero de puertas en el mapa */
static uint num_hab; /* numero de habitaciones en el mapa */

static void nivcountnum() {
	/* cuenta y da valor a todas las variables num */
	num_cha=num_cpa=num_cpu=num_hab=0;
	for(int r=0;r<MAPAR;r++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t* l=mapget(r,c);
			if(l->trs==1) {
				if(l->hab==0) num_cpa++;
				else {
					num_cha++;
					if(l->hab>num_hab) num_hab=l->hab;
				}
			} else if(l->trs==2) num_cpu++;
		}
	}
}

static void nivini() {
	/* inicia todos los niveles poniendo la semilla y definiendolos en 0 */
	randomize(-1);
	nivel_t* p=nivel;
	uint cn=NIN;
	while(p!=nivel+NIVS) {
		p->def=0;
		p->sem=rnd(0,1000000);
		p++->num=cn++;
	}
}

static void nivmap(uint niv,uint seed) {
	/* crea el mapa con la semilla dada */
	randomize(seed);
	Bool sup,sdw;
	sup=sdw=TRUE;
	if(niv==NIN) sup=FALSE;
	else if(niv==NFI) sdw=FALSE;
	mapnew(sup,sdw);
}

static void nivitm(uint niv) {
	/* colocacion de todos los items segun las cantidades */
	nivel_t* p=nivel+(niv-NIN);
	llvlev(p->lls);
	orolev(p->oro);
	anilev(p->ani);
	armlev(p->ars);
	prtlev(p->prs);
}

static void nivene(uint niv) {
	/* colocacion de todos los enemigos segun las cantidades */
	nivel_t* p=nivel+(niv-NIN);
	enelev(p->ens);
}

static Bool nivnue(uint niv) {
	/* crea todo lo relativo a un nivel visitado por primera vez */
	Bool ret=FALSE;
	if(niv>=NIN && niv<=NFI) {
		ret=TRUE;
		/* creacion del mapa y contado de casillas */
		nivel_t* p=nivel+(niv-NIN);
		p->def=1;
		nivmap(niv,p->sem);
		nivcountnum();
		/* oro */
		p->oro=(num_cha*OXD)/10;
		/* llaves */
		uint nll=1;
		if(PLR) {
			nll=(num_cpu/PLR)+(num_cpu%PLR!=0)?1:0;
			nll=(nll>15)?15:nll;
		}
		p->lls=nll;
		/* anillo */
		p->ani=(niv==NFI)?1:0;
		/* armas */
		int nua=APN;
		p->ars=(nua<=0)?1:(nua>15)?15:nua;
		/* protecciones */
		int nup=PPN;
		p->prs=(nup<=0)?1:(nup>15)?15:nup;
		/* colocacion de todos los objetos */
		nivitm(niv);
		/* enemigos */
		p->ens=0;
		for(int k=0;k<num_hab;k++) p->ens+=rnd(EmH,EMH);
		/* colocacion de enemigos */
		nivene(niv);
	}
	return ret;
}

static void nivopd(nivel_t* p) {
	/* una vez visitado el nivel se abren las puertas cerradas segun el numero de llaves y todas las ocultas */
	int probabilidad=p->lls*PLR;
	for(int r=0;r<MAPAR;r++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t* l=mapget(r,c);
			if(l && l->trs>1) {
				if(l->trs==3) l->trs=1;
				else if(l->trs==2 && rnd(0,probabilidad)==0) l->trs=1;
			}
		}
	}
}

static Bool nivvie(uint niv) {
	/* crea todo lo relativo a un nivel visitado no por primera vez */
	Bool ret=FALSE;
	if(niv>=NIN && niv<=NFI) {
		nivel_t* p=nivel+(niv-NIN);
		ret=TRUE;
		/* mapa */
		nivmap(niv,p->sem);
		/* objetos */
		nivitm(niv);
		/* enemigos */
		nivene(niv);
		/* opertura de ocultas y puertas */
		nivopd(p);
	}
	return ret;
}				

void nivprm() {
	cls();
	nivini();
	num_nivel=1;
	nivnue(num_nivel);
	jugnew();
	jugpos(0);
	randomize(-1);
}

static Bool objinniv(objeto_t* o) {
	if(o) {
		if(o->r!=-1 && o->c!=-1) return TRUE;
		else if(o->npc==0){
			objeto_t* npc=o->con;
			if(npc && npc->jug==0) return TRUE;
		}
	}
	return FALSE;
}

static void nivres(uint niv) {
	/* cuenta todos los objetos que se han quedado en el nivel */
	nivel_t* p=nivel+(niv-NIN);
	p->lls=p->ars=p->ens=p->ani=p->oro=p->prs=0;
	objeto_t* oin[objsiz()];
	uint oins=objfnd(oin,objinniv);
	for(uint k=0;k<oins;k++) {
		objeto_t* oe=oin[k];
		if(oe->npc) {
			if(!oe->jug) p->ens++;
		} else if(oe->ior) {
			p->oro+=oe->cor;
		} else if(oe->arm) {
			p->ars++;
		} else if(oe->prt) {
			p->prs++;
		} else if(oe->lla) {
			p->lls++;
		} else if(oe->ani) {
			p->ani=1;
		}
	}
}

static Bool objguaniv(objeto_t* o) {
	if(o && (o->npc==0 && o->cog==1 && o->con==jugador)) return TRUE;
	return FALSE;
}

static uint nivgua(objeto_t* caro) {
	/* se guardan en caro los objetos que contiene el jugador */
	objeto_t* pcaro[objsiz()];
	uint caros=objfnd(pcaro,objguaniv);
	for(int k=0;k<caros;k++) {
		caro[k]=*pcaro[k];
	}
	return caros;
}

static void nivsetgua(uint caros,objeto_t* caro) {
	/* se crean los objetos que llevaba el jugador */
	for(int k=0;k<caros;k++) {
		objeto_t oe=caro[k];
		objcpy(oe);
	}
}

Bool nivchg(int dir) {
	if((dir==-1 && num_nivel>NIN) || (dir==1 && num_nivel<NFI)) {
		cls();
		nivres(num_nivel); /* se guardan objetos que quedan en el nivel */
		objeto_t ocar[objsiz()]; 
		uint ocars=nivgua(ocar); /* se obtienen los objetos que pasaran al siguiente nivel */
		objclr(); /* se borran los objetos actuales */
		num_nivel+=dir; /* se aumenta el numero de nivel */
		nivsetgua(ocars,ocar); /* se definen los objetos del jugador y el jugador */
		/* se establece el nuevo nivel */
		if(nivel[num_nivel-NIN].def==0) nivnue(num_nivel);
		else nivvie(num_nivel);
		/* se situa al jugador en la posicion adecuada */
		if(jugpos(dir)) {
			randomize(-1);
			return TRUE;
		}
		/* se quita al fantasma */
		fantasma=NULL;
	}
	return FALSE;
}

		


		



