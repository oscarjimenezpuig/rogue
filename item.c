/*
============================================================
  Fichero: item.c
  Creado: 18-12-2025
  Ultima Modificacion: vie 19 dic 2025 12:29:18
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

static uint numpue() {
	/* calcula el numero de puertas que hay en un nivel */
	uint tot=0;
	for(int f=0;f<MAPAR;f++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t* l=mapget(f,c);
			if(l && l->trs==2) ++tot;
		}
	}
	return tot;
}

static uint numlln() {
	/* da el numero de llaves del nivel */
	uint np=numpue();
	uint rp=(PLR)?np/PLR:0;
	uint rs=(PLR)?np%PLR:0;
	if(rs!=0) ++rp;
	return rp;
}

static objeto_t* llvnew() {
	objeto_t* ll=itmnew("LLAVE",LLPA);
	if(ll) {
		ll->lla=1;
	}
	return ll;
}

void llplev() {
	uint lls=numlln();
	for(int k=0;k<lls;k++) {
		objeto_t* ll=llvnew();
		if(!ll || !itmplc(ll,TRUE)) break; 
	}
}

static uint orototal() {
	/* cuenta los transitables con habitacion de un mapa y da el oro en funcion de estos*/
	uint trs=0;
	for(int f=0;f<MAPAR;f++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t* l=mapget(f,c);
			if(l && l->trs==1 && l->hab!=0) trs++;
		}
	}
	return (trs*OXD)/10;
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
	objeto_t* o=itmnew("TESORO",OROA);
	if(o) {
		o->ior=1;
		o->cor=oro;
	}
	return o;
}

void orolev() {
	uint total=orototal();
	uint monton[OML+1];
	uint montones=ororep(total,monton);
	for(int k=0;k<montones;k++) {
		if(monton[k]>0) {
			objeto_t* o=oronew(monton[k]);
			if(!o || !itmplc(o,FALSE)) break;
		}
	}
}






