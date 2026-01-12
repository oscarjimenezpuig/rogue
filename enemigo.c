/*
============================================================
  Fichero: enemigo.c
  Creado: 29-12-2025
  Ultima Modificacion: lun 12 ene 2026 14:15:40
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
	ra("rata",2,0,7);
	ra("serpiente",2,2,5);
	ra("tarantula",4,4,1);
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
			objeto_t* en=enenew(*r);
			if(en) {
				if(!maxnivget) en->vid=rnd(VmE,VME);
				return enepos(en);
			}
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

/* funciones ia auxiliares */

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

static void swap(int* a,int* b) {
	int c=*a;
	*a=*b;
	*b=c;
}

static void posord(int ro,int co,int rf,int cf,int* r,int* c,int* d) {
	/* ordena los vecinos de ro,co por distancia creciente a rf,cf, dando tambien la distancia a rf,cf */
	const int DR[]={0,0,-1,1};
	for(int k=0;k<4;k++) {
		r[k]=ro+DR[k];
		c[k]=co+DR[3-k];
		d[k]=mapdis(rf,cf,r[k],c[k]);
	}
	Bool changes;
	do {
		changes=FALSE;
		for(int k=0;k<3;k++) {
			if(d[k]>d[k+1]) {
				swap(d+k,d+k+1);
				swap(r+k,r+k+1);
				swap(c+k,c+k+1);
				changes=TRUE;
			}
		}
	} while(changes);
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
	return TRUE;
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

static Bool iamovrnd(objeto_t* e) {
	/* se mueve aleatoriamente */
	if(e->dr==-1 || e->dc==-1 || CDR==1) maprndpos(&e->dr,&e->dc,TRUE);
	return iamovcer(e,e->dr,e->dc);
}

static int iaefeata(objeto_t* e) {
	/* evalua las probabilidades de lucha cuerpo a cuerpo con el jugador */
	/* si el resultado es positivo, es favorable a e, si es negativo, es favorable a jugador */
	/* se tiene en cuenta la velocidad añadiendo un factor de proporcionalidad */
	int danj=0;
	int dane=0;
	int fj,fe;
	fj=fe=1;
	if(e->vel>jugador->vel) fe=(e->vel+1)/(jugador->vel+1);
	else if(e->vel<jugador->vel && fe) fj=(jugador->vel+1)/(e->vel+1);
	for(int k=0;k<ATS;k++) {
		if(HAT(e,jugador)) dane+=fe*DAN(e);
		if(HAT(jugador,e)) danj+=fj*DAN(jugador);
	}
	return (dane-danj);
}

/* funciones ia importantes */

static Bool iaatacac(objeto_t* e) {
	/* ia que rige el ataque cuerpo a cuerpo */
	/* en el caso de animal, siempre ataca, en el caso de no animal, se evalua */
	if(jugador && jugador->vid>0 && iajugvis(e)) {
		if(e->anm) {
			if(objdis(jugador,e)==1) return objata(e,jugador);
			else return iamovcer(e,jugador->r,jugador->c);
		} else {
			int ra=iaefeata(e);
			if(ra>0) {
				if(objdis(jugador,e)==1) return objata(e,jugador);
				else return iamovcer(e,jugador->r,jugador->c);
			} else return iamovlej(e,jugador->r,jugador->c);
		}
	}
	return FALSE;
}

static Bool iaacoger(objeto_t* e) {
	/* funcion que se encarga de coger objetos si se puede */
	/* los enemigos no cogen ni llaves ni el anillo */
	objeto_t* inv[objsiz()];
	uint invs=objinv(e,inv);
	objeto_t* vis[objsiz()];
	uint viss=iaitvi(e,vis);
	if(invs<e->cap && viss>0) {
		int dis[viss];
		int chk[viss];
		int chkd=0;
		for(int k=0;k<viss;k++) {
			objeto_t* oe=vis[k];
			dis[k]=objdis(e,oe);
			if(oe->cog==0 || oe->lla || oe->ani || (oe->prt && objisprt(e))) {
				chk[k]=1;
				chkd++;
			} else chk[k]=0;
		}
		while(chkd<viss) {
			int omd=-1;
			uint domd=0;
			for(int k=0;k<viss;k++) {
				if(chk[k]==0) {
					if(omd==-1 || (domd>dis[k])) {
						omd=k;
						domd=dis[k];
					}
				}
			}
			if(omd!=-1) {
				objeto_t* oe=vis[omd];
				if(!objcog(e,oe)) {
					if(!iamovcer(e,oe->r,oe->c)) {
						chk[omd]=1;
						chkd++;
					} else return TRUE;
				} else return TRUE;
			}
		}
	}
	return FALSE;
}

/* funciones ia principales */

static Bool iaanimal(objeto_t* e) {
	if(!iaatacac(e)) {
		return iamovrnd(e);
	}
	return TRUE;
}

static Bool iahumano(objeto_t* e) {
	if(!iaatacac(e)) {
		if(!iaacoger(e)) {
			return iamovrnd(e);
		}
	}
	return TRUE;
}

/* funciones de actuacion */

static Bool oneeneact(objeto_t* e) {
	if(e && e->anm) return iaanimal(e);
	else if(e) return iahumano(e);
	else return FALSE;
}

static Bool iseneact(objeto_t* o) {
	return (o && o->npc && !o->jug && objcanact(o));
}

Bool eneact() {
	objeto_t* oe[objsiz()];
	uint oes=objfnd(oe,iseneact);
	Bool ret=FALSE;
	for(int k=0;k<oes;k++) {
		ret|=oneeneact(oe[k]);
	}
	return ret;
}








		


