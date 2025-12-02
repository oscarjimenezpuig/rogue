/*
============================================================
  Fichero: mapa.c
  Creado: 01-12-2025
  Ultima Modificacion: mar 02 dic 2025 14:30:01
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

#define NOR 1
#define EST 2
#define WST 4
#define SUR 8

typedef struct {
	u1 number;
	u1 dark;
	int r,c;
	int rs,cs;
} habitacion_t;

typedef struct {
	u1 cam : 4;
	u1 sal : 4;
} hablab_t;

static habitacion_t habitacion[HABA];
static u1 habitaciones=0;
static hablab_t laberinto[HABA];

localidad_t mapa[MAPAA];

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

#define labhab(R,C) (laberinto + ((C)+(R)*HABC))
#define sign(I,F) ((I>F)?-1:(I<F)?1:0)

static void doway(int way,int ro,int co,int rf,int cf) {
	/* une dos habitaciones con un camino */
	hablab_t* l=labhab(ro,co);
	if(l->cam==0) {
		l->cam=way;
		int dir=0;
		if(co!=cf) dir=1;
		if(ro!=rf) dir|=2;
		if(dir) {
			int dal=0;
			while(((dal=rnd(1,2)) & dir)==0);
			dir=dal;
			int rn,cn;
			u1 cdir=0;
			if(dir==1) {
				rn=ro;
				int s=sign(co,cf);
				cn=co+s;
				dir=(s==1)?EST:WST;
				cdir=(dir==EST)?WST:EST;
			} else if(dir==2) {
				int s=sign(ro,rf);
				rn=ro+s;
				cn=co;
				dir=(s==1)?SUR:NOR;
				cdir=(dir==SUR)?NOR:SUR;
			}
			l->sal|=dir;
			hablab_t* d=labhab(rn,cn);
			d->sal|=cdir;
			doway(way,rn,cn,rf,cf);
		}
	}
}

static u1 fndfree(int* r,int* c) {
	/* da una habitacion por la que no pasa un camino, 1 si la encuentra, 0 si no */
	for(u1 y=0;y<HABR;y++) {
		for(u1 x=0;x<HABC;x++) {
			hablab_t* l=labhab(y,x);
			if(l->cam==0) {
				*r=y;
				*c=x;
				return 1;
			}
		}
	}
	return 0;
}

static u1 fndrndocup(int* r,int* c) {
	/* da una habitacion random por la que pasa un camino */
	int fi=rnd(0,HABR-1);
	int ci=rnd(0,HABC-1);
	hablab_t* ini=labhab(fi,ci);
	hablab_t* l=ini;
	do {
		if(l->cam!=0) break;
		else {
			l++;
			if(l==laberinto+HABA) l=laberinto;
		}
	}while(l!=ini);
	if(l==ini) return 0;
	else {
		int pos=l-laberinto;
		*r=pos/HABC;
		*c=pos%HABC;
		return 1;
	}
}

static void labini() {
	/* inicia el laberinto con un camino inicial */
	hablab_t* p=laberinto;
	while(p!=laberinto+HABA) *p++=(hablab_t){0,0};
	int ri=rnd(0,HABR-1);
	int ci=rnd(0,HABC-1);
	int rf,cf;
	while((rf=rnd(0,HABR-1)) && ((cf=rnd(0,HABC-1))) && (ri==rf) && (ci==cf));
	doway(1,ri,ci,rf,cf);
}

static void dolab() {
	/* creacion de un laberinto con las dimesiones HABR HABC */
	labini();
	int ri,ci;
	int way=2;
	/*
	while(fndfree(&ri,&ci)) {
		int rf,cf;
		fndrndocup(&rf,&cf);
		doway(way++,ri,ci,rf,cf);
	}
	*/
}

static void prtlab() {
	/* impresion del lab dbg */
	ink(WHITE);
	for(int r=0;r<HABR;r++) {
		for(int c=0;c<HABC;c++) {
			hablab_t* l=labhab(r,c);
			at(r,c);
			prints("%i",l->sal);
		}
	}
}

/* prueba */

void begin() {
	dolab();
	prtlab();
	while(!inkey('q')) listen();
}




					




