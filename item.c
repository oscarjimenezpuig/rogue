/*
============================================================
  Fichero: item.c
  Creado: 18-12-2025
  Ultima Modificacion: jue 18 dic 2025 13:15:21
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define LLPA (atributo_t){'?',BOLD,CYAN,BLACK} //atributo de una llave de puerta
#define LLCA (atributo_t){'?',BOLD,GREEN,BLACK} //atributo de otro tipo de llaveG

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

static objeto_t* llvnew(uint puerta) {
	atributo_t a=(puerta)?LLPA:LLCA;
	objeto_t* ll=objnew("Llave",a,FALSE,FALSE);
	if(ll) {
		ll->lla=1;
		ll->pue=puerta;
	}
	return ll;
}

void llplev() {
	uint lls=numlln();
	for(int k=0;k<lls;k++) {
		objeto_t* ll=llvnew(1);
		if(ll) {
			int r,c;
			maprndpos(&r,&c,TRUE);
			while(!objinipos(ll,r,c));
		} else break;
	}
}

