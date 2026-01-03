/*
============================================================
  Fichero: enemigo.c
  Creado: 29-12-2025
  Ultima Modificacion: dissabte, 3 de gener de 2026, 20:46:45
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

typedef struct {
	char nom[SLEN+1]; /* nombre */
	uint anm : 1; /* es animal */
	uint fue : 4; /* fuerza */
	uint hab : 4; /* habilidad */
	uint vel : 4; /* velocidad */
	uint cap : 4; /* capacidad */
} raza_t;

static raza_t raza[RZS];
static uint razas=0;

static objeto_t* dragon=NULL;
static objeto_t* sauron=NULL;

static int razniv(raza_t r) {
	/* calcula el nivel de un enemigo */
	const double IN=(NFI-NIN);
	const double IP=(PFE-PIE);
	const double M=IN/(IP*CEE);
	const double N=(NIN*IP-IN*PIE)/IP;
	double x=r.fue+r.hab+r.vel+r.cap;
	double y=M*x+N;
	return (int)y;
}

static void tocap(char* d,char* o) {
	char* po=o;
	char* pd=d;
	while(*po!=EOS) {
		if(*po>='a' && *po<='z') *pd=*po-'a'+'A';
		else *pd=*po;
		pd++;
		po++;
	}
}

static Bool raznew(raza_t r) {
	if(razas<RZS) {
		raza_t* pr=raza+razas++;
		tocap(pr->nom,r.nom);
		pr->anm=r.anm;
		pr->fue=r.fue;
		pr->hab=r.hab;
		pr->vel=r.vel;
		pr->cap=r.cap;
		return TRUE;
	}
	return FALSE;
}

#define rf(N,A,F,H,V,C) (raza_t){N,(A),(F),(H),(V),(C)}
#define rr(N,F,H,V,C) raznew(rf(N,0,F,H,V,C))
#define ra(N,F,H,V) raznew(rf(N,1,F,H,V,0))

static void razdef() {
	/* definicion de todas las razas excepto las dos especiales */
	rr("aeralin caidos",4,5,6,4);
	rr("bramh igneos",6,3,2,7);
	rr("cyrn sombrios",3,6,5,2);
	rr("elyth espectrales",2,6,5,5);
	rr("feralis salvajes",6,6,5,5);
	rr("gromnil abismal",8,4,3,9);
	rr("horror hylari",3,7,4,6);
	rr("ishkar desertico",5,4,5,6);
	rr("jorveth helado",7,5,4,8);
	rr("khaarn demoniaco",10,6,7,9);
	rr("lunari corrupto",4,7,5,6);
	rr("mordrek nomuerto",8,5,3,10);
	rr("nyxar sombrio",5,9,8,4);
	rr("orkan marino",14,6,7,13);
	rr("pyron igneo",9,6,7,8);
	rr("quorim psiquico",3,10,5,10);
	rr("rhazak invasor",9,7,7,9);
	rr("thyrimm gigantesco",15,6,4,15);
	rr("umbrael corruptor",6,9,7,8);
	rr("valyrr caido",7,7,6,6);
	rr("wyrn alado",6,5,8,5);
	rr("xeloth aberrante",6,10,6,8);
	rr("ydrasil ancestral",14,14,5,15);
	rr("zarkun del vacio",12,13,10,15);
	ra("rata",3,0,9);
	ra("serpiente",2,3,7);
	ra("tarantula",5,5,2);
}


static objeto_t* enenew(raza_t r) {
	/* definicion de un enemigo a partir de sus caracteristicas y su nivel */
	uint ink=rnd(RED,CYAN);
	char chr=*(r.nom);
	if(r.anm) chr=chr-'A'+'a';
	atributo_t a={chr,BOLD,ink,BLACK};
	objeto_t* o=objnew(r.nom,a,TRUE,FALSE);
	if(o) {
		o->anm=r.anm;
		o->fue=r.fue;
		o->hab=r.hab;
		o->vel=r.vel;
		o->cap=r.cap;
		o->cve=o->vel;
	}
	return o;
}

static int enepos(objeto_t* enemigo) {
	/* coloca a los enemigos en una habitacion diferente a la del jugador */
	const int TRIES=10000;
	int ret=0;
	if(enemigo) {
		int tries=TRIES;
		while(tries-- && ret==0) {
			int r,c;
			if(maprndpos(&r,&c,TRUE)) {
				localidad_t* l=mapget(r,c);
				if(num_nivel!=NIN || NHMI==1 || l->hab!=JHI) {
					if(objinipos(enemigo,r,c)) ret=1;
				}
			}
		}
	}
	return ret;
}		

static int razporniv() {
	/* escoge una raza al azar que se adecue el nivel */
	const int TRIES=10000;
	static Bool defined=FALSE;
	static Bool maxnivget=FALSE;
	if(!defined) {
		razdef();
		defined=TRUE;
	}
	maxnivget=(num_nivel==NFI)?TRUE:FALSE;
	int tries=TRIES;
	int nniv=(int)num_nivel;
	while(tries--) {
		int re=rnd(0,razas-1);
		raza_t* r=raza+re;
		int nr=0;
		if((maxnivget==TRUE && num_nivel!=NFI) || ((nr=razniv(*r)) && nr>=nniv+RNm && nr<=nniv+RNM)) {
			return enepos(enenew(*r));
		}
	}
	return 0;
}

static int razesp() {
	/* escoge una de las dos razas especiales */
	static int posdragon=(NFI-NIN+1)/2; 
	static int possauron=NFI;
	int res=0;
	if(num_nivel==posdragon && (!dragon || dragon->vid>0)) {
		if(!dragon) {
			raza_t drg=rf("dragon",0,15,13,13,15);
			dragon=enenew(drg);
		}
		res+=enepos(dragon);
		posdragon=rnd(NIN,posdragon);
	}
	if(num_nivel==possauron && (!sauron || sauron->vid>0)) {
		if(!sauron) {
			raza_t sau=rf("sauron",0,15,15,15,15);
			sauron=enenew(sau);
		}
		res+=enepos(sauron);
		possauron=rnd(NIN,possauron);
	}
	return res;
}

#undef ra
#undef rr
#undef rf

void enelev(uint es) {
	int res=es;
	res-=razesp();
	for(int k=0;k<res;k++) razporniv();
}

/* inteligencia artificial */

static int nhab=-1;
static Bool iaivno(objeto_t* o) {
	/* condicion de item visto en zona no oscura */
	if(o && o->npc==0) {
		localidad_t* l=mapget(o->r,o->c);
		if(l && l->hab==nhab) return TRUE;
	}
	return FALSE;
}
static objeto_t* oene=NULL;
static Bool iaivo(objeto_t* o) {
	/* condicion de item visto en zona oscura */
	return (objdis(oene,o)==1);
}	

static uint iaitvi(objeto_t* e,objeto_t* vis[]) {
	/* determina los objetos visibles por un enemigo */
	if(e) {
		localidad_t* l=mapget(e->r,e->c);
		if(l) {
			Condicion c=NULL;
			if(l->osc) {
				c=iaivo;
				oene=e;
			} else {
				nhab=l->hab;
				c=iaivno;
			}
			return objfnd(vis,c);
		}
	}
	return 0;
}

static Bool iajugvis(objeto_t* e) {
	/* dice si el jugador es visible por el enemigo */
	if(e) {
		localidad_t* l=mapget(e->r,e->c);
		if(l && l->vis==2) return TRUE;
	}
	return FALSE;
}

static int pr=-1;
static int pc=-1;
static Bool eninpos(objeto_t* o) {
	return (o && o->npc==1 && o->r==pr && o->c==pc);
}

static Bool iamoveto(objeto_t* e,int r,int c) {
	/* intenta mover el objeto a la posicion r,c */
	if(e) {
		int er=e->r;
		int ec=e->c;
		int dr=SGN(r,er);
		int dc=SGN(c,ec);
		if(rnd(0,1)) {
			if(dr!=0 && objmov(e,dr,0)) return TRUE;
			else if (dc!=0 && objmov(e,0,dc)) return TRUE;
		} else {
			if(dc!=0 && objmov(e,0,dc)) return TRUE;
			else if(dr!=0 && objmov(e,dr,0)) return TRUE;
		}
	}
	return FALSE;
}

static void posord(int ro,int co,int rf,int cf,int* r,int* c,int* d) {
	/* ordena los vecinos de ro,co por distancia creciente a rf,cf dando tambien la distancia */
	const int DR[]={0,0,-1,1};
	for(int k=0;k<4;k++) {
		d[k]=-1;
	}
	for(int k=0;k<4;k++) {
		int nr=ro+DR[k];
		int nc=co+DR[3-k];
		int dis=mapdis(rf,cf,ro,co);
		for(int n=0;n<4;n++) {
			if(d[n]==-1 || d[n]>dis) {
				for(int m=n;m<3;m++) {
					d[m+1]=d[m];
					r[m+1]=r[m];
					c[m+1]=c[m];
				}
				d[n]=dis;
				r[n]=nr;
				c[n]=nc;
			}
		}
	}
}

static Bool iamovlej(objeto_t* e,int rf,int cf) {
	/* intentamos alejarnos de la posicion rf,cf*/
	int r[4];
	int c[4];
	int d[4];
	posord(e->r,e->c,rf,cf,r,c,d);
	int disa=mapdis(e->r,e->c,rf,cf);
	for(int k=3;k>=0 && d[k]>disa;k++) {
		if(iamoveto(e,r[k],c[k])) return TRUE;
	}
	return FALSE;
}

static Bool iamovcer(objeto_t* e,int rf,int cf) {
	/* intentamos acercarnos a la posicion rf,cf */
	int r[4];
	int c[4];
	int d[4];
	posord(e->r,e->c,rf,cf,r,c,d);
	int disa=mapdis(e->r,e->c,rf,cf);
	for(int k=0;k<4 && d[k]<disa;k++) {
		if(iamoveto(e,r[k],c[k])) return TRUE;
	}
	return FALSE;
}

//TODO Programar una rutina que evalue la efectividad del ataque cuerpo a cuerpo
		
