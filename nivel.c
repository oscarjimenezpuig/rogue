/*
============================================================
  Fichero: nivel.c
  Creado: 23-12-2025
  Ultima Modificacion: dimarts, 23 de desembre de 2025, 19:59:00
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define NIVS (NFI-NIN+1)

static nivel_t nivel[NIVS];

static uint num_cha; /* numero de casillas habitacion en el mapa */
static uint num_cpa; /* numero de casillas pasadizo en el mapa */
static uint num_cpu /* numero de puertas en el mapa */
static uint num_hab; /* numero de habitaciones en el mapa */

static void count_num() {
	/* cuenta y da valor a todas las variables num */
	num_cha=num_cpa=num_cpu=num_hab=0;
	for(int r=0;r<MAPAR;r++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t l=mapget(r,c);
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
	/* inicia todos los niveles colocando la semilla en 0 */
	nivel_t* p=nivel;
	uint cn=NIN
	while(p!=nivel+NIVS) {
		p->sem=0;
		p++->num=cn++;
	}
}

static Bool nivnue(uint n) {
	/* crea todo lo relativo a un nivel visitado por primera vez */
	if(n>=NIN && n<=NFI) {
		nivel_t* p=nivel+(n-NIN);
		p->sem=rnd(1,100000);
		Bool sup,sdw=TRUE;
		if(n==NIN) sup=FALSE;
		else if(n==NFI) sdw=FALSE;
		mapnew(sup,sdw);
		count_num();
		/* situamos oro */
		p->oro=(num_cha*OXD)/10;
		orolev(p->oro);
		/* situamos redherring */
		if(n==NIN) rhrlev();
		/* situamos las llaves */




