/*
============================================================
  Fichero: mapa.c
  Creado: 01-12-2025
  Ultima Modificacion: dimecres, 3 de desembre de 2025, 21:03:33
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
	while(p!=mapa+MAPAA) *p++=(localidad_t){VACIO,0,0,0};
}

static habitacion_t habnew(u1 n,u1 r,u1 c) {
	/* define una habitacion */
	habitacion_t h;
	h.number=n;
	h.rs=rnd(MHR/2,MHR-1);
	h.cs=rnd(MHC/2,MHC-1);
	h.r=rnd(r*MHR,r*MHR+MHR-h.rs-1);
	h.c=rnd(c*MHC,c*MHC+MHC-h.cs-1);
	h.dark=(rnd(0,PHO)==0);
	return h;
}

static void habprtdbg() {
	for(int n=0;n<habitaciones;n++) {
		habitacion_t h=habitacion[n];
		printf("n=%i (%i,%i) r=%i c=%i\n",h.number,h.r,h.c,h.rs,h.cs);
	}
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

#define locpos(R,C) (mapa+((C)+(R)*MAPAC))

static void habinloc() {
	/* coloca todas las habitaciones en las localidades */
	for(int k=0;k<habitaciones;k++) {
		habitacion_t h=habitacion[k];
		for(int r=h.r;r<h.r+h.rs;r++) {
			for(int c=h.c;c<h.c+h.cs;c++) {
				localidad_t* l=locpos(r,c);
				l->visible=0;
				l->oscuro=h.dark;
				l->habitacion=h.number;
				if(r==h.r || r==h.r+h.rs-1 || c==h.c || c==h.c+h.cs-1) {
					l->tipo=OBSTACULO;
				} else {
					l->tipo=TRANSITABLE;
				}
			}
		}
	}
}

#define labhab(R,C) (laberinto + ((C)+(R)*HABC))
#define sign(I,F) (((I)>(F))?-1:((I)<(F))?1:0)

static void doway(int way,int ro,int co,int rf,int cf) {
	/* une dos habitaciones con un camino */
	hablab_t* l=labhab(ro,co);
	if(l->cam==0) {
		l->cam=way;
		int dir=0;
		if(co!=cf) dir|=1;
		if(ro!=rf) dir|=2;
		if(dir) {
			int dal=0;
			do {
				dal=rnd(1,2);
			} while((dal & dir)==0);
			dir=dal;
			int rn=ro;
			int cn=co;
			int s=0;
			if(dir==1) {
				s=sign(co,cf);
				cn+=s;
				dir=(s==1)?EST:WST;
			} else if(dir==2) {
				s=sign(ro,rf);
				rn+=s;
				dir=(s==1)?SUR:NOR;
			}
			int cdir=8/dir;
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

static void fndrndocup(int* r,int* c) {
	hablab_t* l=NULL;
	do {
		*r=rnd(0,HABR-1);
		*c=rnd(0,HABC-1);
		l=labhab(*r,*c);
	} while(l->cam==0);
}

static void labini() {
	/* inicia el laberinto con un camino inicial */
	hablab_t* p=laberinto;
	while(p!=laberinto+HABA) *p++=(hablab_t){0,0};
	int ri=rnd(0,HABR-1);
	int ci=rnd(0,HABC-1);
	int rf,cf;
	do {
		rf=rnd(0,HABR-1);
		cf=rnd(0,HABC-1);
	}while(ri==rf && ci==cf);
	doway(1,ri,ci,rf,cf);
}

static void dolab() {
	/* creacion de un laberinto con las dimesiones HABR HABC */
	labini();
	int ri,ci;
	int way=2;
	while(fndfree(&ri,&ci)) {
		int rf,cf;
		fndrndocup(&rf,&cf);
		doway(way++,ri,ci,rf,cf);
	}
}

#define ntp(A,M) ((A)*(M)+((M)/2))

static void makeway(u1 ro,u1 co,u1 rf,u1 cf) {
	int pro=ntp(ro,MHR);
	int pco=ntp(co,MHC);
	int prf=ntp(rf,MHR);
	int pcf=ntp(cf,MHR);
	//TODO Programar un camino de pro,pco a prf,pcf
	
}




static void locshwdbg() {
	/* hace un debug para ver como queda el mapa */
	ink(WHITE);
	for(int f=0;f<MAPAR;f++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t l=mapa[c+f*MAPAC];
			char chr=0;
			u1 t=l.tipo;
			if(t==OBSTACULO || t==OCULTA) {
				attr(REVERSE);
				chr=' ';
			} else if(t==TRANSITABLE) {
				if(l.habitacion==0) chr='#';
				else chr='.';
			} else if(t==PUERTA) {
				attr(REVERSE);
				chr='?';
			}
			if(chr!=0) {
				at(f,c);
				printc(chr);
				attr(0);
			}
		}
	}
	show();
}

void map_new() {
	locini();
	habdef();
	habinloc();
}

/* prueba */

void begin() {
	randomize(-1);
	map_new();
	//habprtdbg();	
	locshwdbg(); //dbg
	dolab();
	while(!inkey('q')) listen();
}
