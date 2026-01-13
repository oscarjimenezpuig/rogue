/*
============================================================
  Fichero: item.c
  Creado: 18-12-2025
  Ultima Modificacion: mar 13 ene 2026 13:29:43
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define TRIES 1000 /* numero de tries para situar el objeto */

#define LLPA (atributo_t){'?',BOLD,CYAN,BLACK} /* atributo de una llave de puerta */
#define OROA (atributo_t){'$',BOLD,YELLOW,BLACK} /* atributo de tesoro */
#define ANIA (atributo_t){'o',BOLD,YELLOW,BLACK} /* atributo de anillo */

/* colores de los diferentes tipos de armas */
#define CAL MAGENTA
#define CAE BLUE
#define CAP RED;
#define CAM YELLOW;

typedef struct {
	char nom[SLEN+1]; /* nombre */
	a.chr; /* caracter asociado al arma */
	a.ink; /* color asociado al arma */
	uint tia: 2; /* tipo del arma */
	uint dad: 3; /* tipo del dado de daño */
	uint nad: 2; /* numero de dados de daño */
	uint pfu: 4; /* puntos de fuerza necesarios para darnos extra */
	uint pha: 4; /* puntos de habilidad necesarios para darnos extra */
	uint und: 3; /* unidades disponibles iniciales */
} arma_t;

static arma_t arma[ANT*ATI];

static Bool itmplc(objeto_t* item,Bool pasadizo) {
	int r,c;
	maprndpos(&r,&c,pasadizo);
	for(int k=0;k<TRIES;k++) {
		if(objinipos(item,r,c)) return TRUE;
	}
	return FALSE;
}

static objeto_t* itmnew(char* nom,atributo_t a) {
	objeto_t* it=objnew(nom,a,FALSE,FALSE);
	if(it) {
		it->cog=it->ior=it->arm=it->lla=it->ani=it->prt=it->ves=0;
		it->con=NULL;
	}
	return it;
}

static objeto_t* llvnew() {
	objeto_t* ll=itmnew("LLAVE",LLPA);
	if(ll) {
		ll->lla=1;
		ll->cog=1;
	}
	return ll;
}

void llvlev(uint n) {
	for(int k=0;k<n;k++) {
		objeto_t* ll=llvnew();
		if(!ll || !itmplc(ll,TRUE)) break; 
	}
}

static uint ororep(uint oro,uint* c) {
	/* reparte el oro en montones */
	int montones=rnd(OmL,OML);
	for(int k=0;k<montones;k++) c[k]=0;
	for(int k=0;k<oro;k++) {
		c[rnd(0,montones-1)]+=1;
	}
	return montones;
}

static objeto_t* oronew(uint oro) {
	/* crea un nuevo objeto oro */
	objeto_t* o=itmnew("TESORO",OROA);
	if(o) {
		o->cog=1;
		o->ior=1;
		o->cor=oro;
	}
	return o;
}

void orolev(uint oro) {
	uint monton[OML+1];
	uint montones=ororep(oro,monton);
	for(int k=0;k<montones;k++) {
		if(monton[k]>0) {
			objeto_t* o=oronew(monton[k]);
			if(!o || !itmplc(o,FALSE)) break;
		}
	}
}

void anilev(uint a) {
	/* crea el anillo */
	if(a) {
		objeto_t* o=itmnew("ANILLO",ANIA);
		if(o) {
			o->cog=1;
			o->ani=1;
			itmplc(o,FALSE);
		}
	}
}

static void arftin(char* name,char chr,uint tipo,uint dado,uint dados,uint fuex,uint haex) {
	/* introduccion de las caracteristicas del arma */
	static uint armas=0;
	arma_t a;
	char* pn=name;
	char* pa=a.nom;
	while(*pn!=EOS) *pa++=*pn++;
	*pa=EOS;
	a.chr=chr;
	a.tia=tipo;
	a.dad=dado;
	a.nda=dados;
	a.pfu=fuex;
	a.pha=haex;
	switch(tipo) {
		case 1:
			a.und=MAL;
			a.ink=CAL;
			break;
		case 2:
			a.und=MAE;
			a.ink=CAE;
			break;
		case 3:
			a.und=MAP;
			a.ink=CAP;
			break;
		case 4:
			a.und=MAM;
			a.ink=CAM;
			break;
	}
	arma[armas++]=a;
}

#define nta(T,C,N,D,S,F,H) arftin(N,(C),(T),(D),(S),(F),(H))

static void artoti() {
	/* define todos los tipos de armas */
	nta(1,'-',"Daga ritual",1,2,6,3);
}

#undef nta


void itmrmp(objeto_t* o) {
	if(o && o->npc==0 && o->ior==0) {
		o->r=o->c=-1;
		o->con=NULL;
		o->ves=0;
	}
}










