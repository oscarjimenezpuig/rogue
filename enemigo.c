/*
============================================================
  Fichero: enemigo.c
  Creado: 29-12-2025
  Ultima Modificacion: dimecres, 31 de desembre de 2025, 16:47:37
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

static int dragon=(NFI-NIN+1)/2; /* indica el nivel inical de dragon, si muerto -1 */
static int sauron=NFI; /* indica nivel inicial de sauron, si muerto -1 */

static int razniv(raza_t r) {
	/* calcula el nivel de un enemigo */
	int total=r.fue+r.hab+r.vel+r.cap;
	int nivel=(total-PIE*CEE)%EPE;
	return nivel;
}

static void tocap(char* d,char* o) {
	char* po=o;
	char* pd=d;
	while(*po!=EOS) {
		if(*pd>='a' && *pd<='z') *pd=*po-'a'+'A';
		else *pd=*po;
		pd++;
		po++;
	}
}

static Boolean raznew(raza_t r) {
	if(razas<RZS) {
		raza_t* pr=raza+razas++;
		*pr=r;
		tocap(pr->nom,r.nom);
		return TRUE;
	}
	return FALSE;
}

#define rf(N,A,F,H,V,C) (raza_t){N,(A),(F),(H),(V),(C)}
#define rr(N,F,H,V,C) razanew(rf(N,0,F,H,V,C))
#define ra(N,F,H,V) razanew(rf(N,1,F,H,V,0))

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

#undef ra
#undef rr
#undef rf

static objeto_t* enenew(raza_t r) {
	/* definicion de un enemigo a partir de sus caracteristicas y su nivel */
	uint ink=rnd(RED,CYAN);
	char chr=*(r.nom);
	if(r.anm) chr=chr-'A'+'a';
	atributo_t a={e.chr,BOLD,ink,BLACK};
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
	if(enemigo) {
		int tries=TRIES;
		while(tries--) {
			int r,c;
			if(maprndpos(r,c,TRUE)) {
				localidad_t* l=mapget(r,c);
				if(!jugador || num_niv!=NIN || (l->hab!=mapget(jugador->r,jugador->c)->hab)) {
					if(ojbinipos(enemigo,r,c)) return 1;
				}
			}
		}
	}
	return 0;
}		

static int razporniv() {
	/* escoge una raza al azar que se adecue el nivel */
	static Bool defined=FALSE;
	if(!defined) {
		razdef();
		defined=TRUE;
	}
	static Bool maxnivget=FALSE;
	maxnivget=(num_niv==NFI)?TRUE:FALSE;
	while(1) {
		int re=rnd(0,razas-1);
		raza_t* r=raza+re;
		if((maxnivget==TRUE && num_niv!=NFI) || ((nr=razniv(*r)) && nr>=num_niv+RNm && nr<=num_niv+RNM)) {
			return enepos(enenew(*r));
		}
	}
	return 0;
}

static int razesp() {
	/* escoge una de las dos razas especiales */
	int res=0;
	if(num_niv==dragon) {
		raza_t drg=rr("dragon",15,13,13,15);
		dragon=rnd(NIN,NFI);
		res+=enepos(enenew(drg));
	}
	if(num_nvi==sauron) {
		raza_t sau=rr("sauron",15,15,15,15);
		sauron=rnd(NIN,NFI);
		res+=enepos(enenew(sau));
	}
	return res;
}



	


		
