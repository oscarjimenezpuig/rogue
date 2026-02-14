/* ROGUE 29/12/25 */

#include <stdlib.h>

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
	const double IN=(NFI-NIN+1);
	const double IP=(PFE-PIE+1);
	const double M=IN/(IP*CEE);
	const double N=(NIN*IP-IN*PIE)/IP;
	double x=r.fue+r.hab+r.vel+r.cap;
	double y=M*x+N;
	return (int)y;
}

static Bool raznew(raza_t r) {
	if(razas<RZS) {
		raza_t* pr=raza+razas++;
		char* nd=pr->nom;
		char* no=r.nom;
		while(*no!=EOS) *nd++=*no++;
		*nd=EOS;
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
	char chr=*(r.nom);
	if(r.anm==0) chr=chr-'a'+'A';
	atributo_t a={chr,BOLD,WHITE,BLACK};
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
		int nr=razniv(*r);
		if((maxnivget==TRUE && num_nivel!=NFI) || (nr>=nniv+RNm && nr<=nniv+RNM)) {
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

Bool eneact(objeto_t* e) {
	if(e && e->anm) return iaanm(e);
	else if(e) return iahum(e);
	else return FALSE;
}
