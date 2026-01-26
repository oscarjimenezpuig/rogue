/* ROGUE 1/12/25 */

#include "rogue.h"

#define MAPAA (MAPAR*MAPAC) /* area del mapa */

#define HABR 3 /* numero maximo de habitaciones en fila */
#define HABC 3 /* numero maximo de habitaciones en columna */
#define HABA (HABR*HABC)

#define PEH 2 /* posibilidad de que se forme una habitacion */
#define PHO 4 /* posibilidad de habitacion oscura */
#define MHR (MAPAR/HABR) /*maximo de filas que puede tener una habitacion */
#define MHC (MAPAC/HABC) /*maximo de columnas que puede tener una habitacion */

#define PWO 4 /* probabilidad de camino oculto */

/* direcciones */
#define NOR 1
#define EST 2
#define WST 4
#define SUR 8

typedef struct {
	uint number;
	uint dark;
	int r,c;
	int rs,cs;
} habitacion_t;

typedef struct {
	uint cam : 4;
	uint sal : 4;
} hablab_t;

static habitacion_t habitacion[HABA];
static uint habitaciones=0;
static hablab_t laberinto[HABA];
static int escalera_baja=-1;

localidad_t mapa[MAPAA];

static void locini() {
	/* inicia todas las localidades a obstaculo */
	localidad_t* p=mapa;
	while(p!=mapa+MAPAA) *p++=(localidad_t){0,0,0,0,0,0};
}

#define ntp(A,M) ((A)*(M)+((M)/2))

static habitacion_t habnew(uint n,uint r,uint c) {
	/* define una habitacion */
	const int MLC=MHC/2;
	const int MLR=MHR/2;
	int rm=ntp(r,MHR);
	int cm=ntp(c,MHC);
	int ri=rnd(2,MLR-2);
	int ci=rnd(2,MLC-2);
	int lr=rnd(2,MLR-2)+ri;
	int lc=rnd(2,MLC-2)+ci;
	habitacion_t h={n,(rnd(0,PHO)==0),rm-ri,cm-ci,lr,lc};
	return h;
}

static void habdef() {
	/* define todas las habitaciones del mapa */
	uint done[HABA];
	uint* pd=done;
	while(pd!=done+HABA) *pd++=0;
	while(habitaciones<NHMI) {
		for(int r=0;r<HABR;r++) {
			for(int c=0;c<HABC;c++) {
				if(!done[c+r*HABC] && !rnd(0,PEH) && habitaciones<NHMA){
					habitacion[habitaciones]=habnew(habitaciones+1,r,c);
					++habitaciones;
					done[c+r*HABC]=1;
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
				l->osc=h.dark;
				l->hab=h.number;
				if(r==h.r || r==h.r+h.rs-1 || c==h.c || c==h.c+h.cs-1) {
					l->obs=1;
				} else {
					l->trs=1;
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

static uint fndfree(int* r,int* c) {
	/* da una habitacion por la que no pasa un camino, 1 si la encuentra, 0 si no */
	for(uint y=0;y<HABR;y++) {
		for(uint x=0;x<HABC;x++) {
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


static void chckloc(int r,int c) {
	localidad_t* l=locpos(r,c);
	if(l->obs) {
		Bool hco=((rnd(0,PWO))==0)?TRUE:FALSE;
		l->obs=0;
		l->trs=(hco)?3:2;
	} else if(!l->obs && !l->trs) {
		l->trs=1;
		l->osc=1;
	}
}	

static void makeway(uint rri,uint rci,uint rrf,uint rcf) {
	/* hace un camino entre habitaciones */
	int ri=ntp(rri,MHR);
	int ci=ntp(rci,MHC);
	int rf=ntp(rrf,MHR);
	int cf=ntp(rcf,MHC);
	int sr=sign(ri,rf);
	int sc=sign(ci,cf);
	int c=ci;
	int r=ri;
	chckloc(r,c);
	if(sr!=0) {
		do {
			r+=sr;
			chckloc(r,c);
		}while(r!=rf);
	} else if(sc!=0) {
		do {
			c+=sc;
			chckloc(r,c);
		}while(c!=cf);
	}
}

static void makeways() {
	/* hace todos los caminos entre las habitaciones */
	dolab();
	for(int r=0;r<HABR;r++) {
		for(int c=0;c<HABC;c++) {
			hablab_t* h=labhab(r,c);
			if(h->sal & EST) {
				makeway(r,c,r,c+1);
			}
			if(h->sal & SUR) {
				makeway(r,c,r+1,c);
			}
		}
	}
}

static uint vecinos(int r,int c) {
	const int DDS[]={1,-1,0,0};
	const int SIZ=4;
	int cuenta=0;
	for(int k=0;k<SIZ;k++) {
		int rr=r+DDS[k];
		int cc=c+DDS[SIZ-k-1];
		localidad_t* l=locpos(rr,cc);
		uint t=l->trs;
		uint nh=l->hab;
		if(t==2 || (t==3) || (t==1 && nh==0)) ++cuenta;
	}
	return cuenta;
}

static uint analiza(int r,int c) {
	localidad_t* l=locpos(r,c);
	if(l->trs==1 && l->hab==0) {
		uint vec=vecinos(r,c);
		if(vec==1) {
			l->obs=l->trs=0;
			return 1;
		}
	}
	return 0;
}

static void huerfanas() {
	/* elimina todos los caminos que tienen un vecino */
	uint cambio;
	do {
		cambio=0;
		for(int r=0;r<MAPAR;r++) {
			for(int c=0;c<MAPAC;c++) {
				cambio+=analiza(r,c);
			}
		}
	}while(cambio);
}

static uint tiene_camino(int r,int c) {
	const int DDS[]={1,-1,0,0};
	const int SIZ=4;
	for(int k=0;k<SIZ;k++) {
		int rr=r+DDS[k];
		int cc=c+DDS[SIZ-k-1];
		localidad_t* l=locpos(rr,cc);
		if(l->trs==1 && l->hab==0) return 1;
	}
	return 0;
}

static void limpia_puertas() {
	/* elimina las puertas y los ocultos que han quedado sin camino */
	for(int r=0;r<MAPAR;r++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t* l=locpos(r,c);
			if(l->trs==2 || l->trs==3) {
				if(!tiene_camino(r,c)) {
					l->trs=0;
					l->obs=1;
				}
			}
		}
	}
}

static void coloca_escalera(int escalera) {
	localidad_t* l=NULL;
	int r,c;
	do {
		r=rnd(0,MAPAR-1);
		c=rnd(0,MAPAC-1);
		l=mapget(r,c);
	} while(l->trs!=1 || (l->trs==1 && l->hab==0) || (l->trs==1 && num_nivel==NIN && l->hab==JHI) || (escalera==-1 && escalera_baja==l->hab));
	l->esc=escalera;
	if(escalera==1) escalera_baja=l->hab;
}

static void coloca_escaleras(Bool u,Bool d) {
	if(d) coloca_escalera(1);
	if(u) coloca_escalera(-1);	
}

void mapnew(Bool u,Bool d) {
	habitaciones=0;
	escalera_baja=-1;
	locini();
	habdef();
	habinloc();
	makeways();
	huerfanas();
	limpia_puertas();
	coloca_escaleras(u,d);
}

localidad_t* mapget(int r,int c) {
	if(r>=0 && r<MAPAR && c>=0 && c<MAPAC) return locpos(r,c);
	return NULL;
}

Bool maprndpos(int* r,int* c,Bool p) {
	const uint TRIES=MAPAA;
	uint tries=TRIES;
	while(tries--) {
		*r=rnd(0,MAPAR-1);
		*c=rnd(0,MAPAC-1);
		localidad_t* l=locpos(*r,*c);
		if(l->trs==1 && (l->hab!=0 || p)) return TRUE;
	}
	return FALSE;
}

uint mapngh(int r,int c,localidad_t* l[4]) {
	const int DR[]={-1,1,0,0};
	uint ret=4;
	for(int k=0;k<4;k++) {
		int re=r+DR[k];
		int ce=c+DR[3-k];
		localidad_t* le=mapget(re,ce);
		if(!le) --ret;
		l[k]=le;
	}
	return ret;
}

int mapdis(int ro,int co,int rf,int cf) {
	if(mapget(ro,co) && mapget(rf,cf)) {
		return ABS(ro-rf) + ABS(co-cf);
	}
	return -1;
}
