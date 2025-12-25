/*
============================================================
  Fichero: item.c
  Creado: 18-12-2025
  Ultima Modificacion: dijous, 25 de desembre de 2025, 08:27:42
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define TRIES 1000 /* numero de tries para situar el objeto */

#define LLPA (atributo_t){'?',BOLD,CYAN,BLACK} //atributo de una llave de puerta
#define OROA (atributo_t){'$',BOLD,YELLOW,BLACK} //atributo de tesoro

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
		it->ior=it->arm=it->lla=it->ani=it->ves=0;
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

void llplev(uint n) {
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






