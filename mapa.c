/*
============================================================
  Fichero: mapa.c
  Creado: 01-12-2025
  Ultima Modificacion: dilluns, 1 de desembre de 2025, 20:07:13
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define HABR 3 /* numero maximo de habitaciones en fila */
#define HABC 3 /* numero maximo de habitaciones en columna */
#define HABA (HABR*HABC)

#define NHM 2 /* numero de habitaciones minimas */
#define PEH 2 /* posibilidad de que se forme una habitacion */
#define PHO 4 /* posibilidad de habitacion oscura */
#define MHR (MAPAR/HABR) /*maximo de filas que puede tener una habitacion */
#define MHC (MAPAC/HABC) /*maximo de columnas que puede tener una habitacion */

typedef struct {
	u1 number;
	u1 dark;
	int r,c;
	int rs,cs;
} habitacion_t;

static habitacion_t habitacion[HABA];
static u1 habitaciones=0;

static void locini() {
	/* inicia todas las localidades a obstaculo */
	localidad_t* p=mapa;
	while(p!=mapa+MAPAA) *p++=(localidad_t){0,VACIO,0};
}

static habitacion_t habnew(u1 n,u1 r,u1 c) {
	/* define una habitacion */
	habitacion_t h;
	h.number=n;
	h.rs=rnd(MHR/2,MHR-1);
	h.cs=rnd(MHC/2,MHC-1);
	h.r=rnd(r*MHR,r*MHR+MHR-h.rs);
	h.c=rnd(c*MHC,c*MHC+MHC-h.cs);
	h.dark=(rnd(0,PHO)==0);
	return h;
}

static void habdef() {
	/* define todas las habitaciones del mapa */
	int done[HABA];
	while(habitaciones<NHM) {
		for(int r=0;r<HABR;r++) {
			for(int c=0;c<HABC;c++) {
				if(rnd(0,PEH)==0) {
					u1 free=1;
					for(u1 c=0;c<habitaciones && free;c++) {
						if(done[c]==c+r*HABC) free=0;
					}
					if(free) {
						done[habitaciones]=c+r*HABC;
						habitacion[habitaciones]=habnew(habitaciones+1,r,c);
						habitaciones++;
					}
				}
			}
		}
	}
}

static void dolab() {
	/* creacion de un laberinto con las dimesiones HABR HABC */




					




