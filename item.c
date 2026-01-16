/*
============================================================
  Fichero: item.c
  Creado: 18-12-2025
  Ultima Modificacion: vie 16 ene 2026 11:56:29
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define TRIES 1000 /* numero de tries para situar el objeto */

#define LLPA (atributo_t){'?',BOLD,CYAN,BLACK} /* atributo de una llave de puerta */
#define OROA (atributo_t){'$',BOLD,YELLOW,BLACK} /* atributo de tesoro */
#define ANIA (atributo_t){'o',BOLD,YELLOW,BLACK} /* atributo de anillo */

/* colores de los diferentes tipos de armas */
#define CAL MAGENTA
#define CAE BLUE
#define CAP RED
#define CAM YELLOW

/* colores de los diferentes tipos de proteccion */
#define CPB MAGENTA
#define CPR BLUE
#define CPA RED
#define CPM YELLOW

typedef struct {
	char nom[SLEN+1]; /* nombre */
	char chr; /* caracter asociado al arma */
	uint ink; /* color asociado al arma */
	uint tia: 2; /* tipo del arma */
	uint dad: 3; /* tipo del dado de daño */
	uint nad: 2; /* numero de dados de daño */
	uint pfu: 4; /* puntos de fuerza necesarios para darnos extra */
	uint pha: 4; /* puntos de habilidad necesarios para darnos extra */
	uint aus: 1; /* este bit dice si un arma magica ha salido o no */
} arma_t;

typedef struct {
	char nom[SLEN+1]; /* nombre */
	char chr; /* caracter asociado */
	uint ink; /* color asociado a la proteccion */
	uint tip : 2; /* tipo del arma */
	uint nar : 4; /* valor de la armadura */
	uint aus : 1; /* este bit dice si la proteccion magica ha salido o no */
} proteccion_t;


static arma_t arma[ANT*ATI];
static proteccion_t proteccion[NTP];

static Bool itmplc(objeto_t* item,Bool pasadizo) {
	int r,c;
	maprndpos(&r,&c,pasadizo);
	for(int k=0;k<TRIES;k++) {
		if(objinipos(item,r,c)) return TRUE;
	}
	return FALSE;
}

static objeto_t* itmnew(char* nom,atributo_t a) {
	objeto_t* it=objnew(nom,a,FALSE,FALSE);
	if(it) {
		it->cog=it->ior=it->arm=it->lla=it->ani=it->prt=it->ves=0;
		it->con=NULL;
	}
	return it;
}

static objeto_t* llvnew() {
	objeto_t* ll=itmnew("LLAVE",LLPA);
	if(ll) {
		ll->lla=1;
		ll->cog=1;
	}
	return ll;
}

void llvlev(uint n) {
	for(int k=0;k<n;k++) {
		objeto_t* ll=llvnew();
		if(!ll || !itmplc(ll,TRUE)) break; 
	}
}

static uint ororep(uint oro,uint* c) {
	/* reparte el oro en montones */
	int montones=rnd(OmL,OML);
	for(int k=0;k<montones;k++) c[k]=0;
	for(int k=0;k<oro;k++) {
		c[rnd(0,montones-1)]+=1;
	}
	return montones;
}

static objeto_t* oronew(uint oro) {
	/* crea un nuevo objeto oro */
	objeto_t* o=itmnew("TESORO",OROA);
	if(o) {
		o->cog=1;
		o->ior=1;
		o->cor=oro;
	}
	return o;
}

void orolev(uint oro) {
	uint monton[OML+1];
	uint montones=ororep(oro,monton);
	for(int k=0;k<montones;k++) {
		if(monton[k]>0) {
			objeto_t* o=oronew(monton[k]);
			if(!o || !itmplc(o,FALSE)) break;
		}
	}
}

void anilev(uint a) {
	/* crea el anillo */
	if(a) {
		objeto_t* o=itmnew("ANILLO",ANIA);
		if(o) {
			o->cog=1;
			o->ani=1;
			itmplc(o,FALSE);
		}
	}
}

static void arftin(char* name,char chr,uint tipo,uint dado,uint dados,uint fuex,uint haex) {
	/* introduccion de las caracteristicas del arma */
	static uint armas=0;
	arma_t a;
	char* pn=name;
	char* pa=a.nom;
	while(*pn!=EOS) *pa++=*pn++;
	*pa=EOS;
	a.chr=chr;
	a.tia=tipo;
	a.dad=dado;
	a.nad=dados;
	a.pfu=fuex;
	a.pha=haex;
	a.aus=0;
	switch(tipo) {
		case 0:
			a.ink=CAL;
			break;
		case 1:
			a.ink=CAE;
			break;
		case 2:
			a.ink=CAP;
			break;
		case 3:
			a.ink=CAM;
			break;
	}
	arma[armas++]=a;
}

#define nta(T,C,N,D,S,F,H) arftin(N,(C),(T),(D),(S),(F),(H)) 
/* T: Tipo, C: Caracter, N: Nombre, D: Dado, S: Numero de dados, F: Puntos extras de fuerza, H: Puntos extra de habilidad */

static void artoti() {
	/* define todos los tipos de armas */
	nta(0,'-',"daga ritual",1,2,6,3);
	nta(0,'~',"daga curva",1,3,7,3);
	nta(0,';',"cuchilla elfica",2,1,6,2);
	nta(0,'/',"espada corta",2,2,5,4);
	nta(0,'=',"cuchillas gemelas",1,3,6,3);
	nta(1,'_',"espada larga",3,1,4,4);
	nta(1,'(',"sable de mercenario",2,2,5,4);
	nta(1,'|',"lanza de guerra",3,2,4,5);
	nta(1,'&',"maza de hierro",4,1,3,6);
	nta(1,'/',"baston de combate",2,3,5,4);
	nta(2,'=',"mandoble",4,1,3,6);
	nta(2,']',"hacha de dos manos",4,2,2,7);
	nta(2,'!',"martillo de guerra",4,2,3,7);
	nta(2,'&',"gran maza",5,1,2,8);
	nta(2,'|',"rompecraneos",4,3,2,8);
	nta(3,')',"latigo de sombras",2,2,6,3);
	nta(3,'~',"guanteletes runicos",3,2,4,5);
	nta(3,'/',"hoja espectral",4,1,5,3);
	nta(3,'*',"garra demoniaca",3,3,4,4);
	nta(3,'!',"martillo de titan",5,1,2,9);
}

#undef nta

static int seltip(int* pesos) {
	/* selecciona tipo de arma y armadura dependiendo del nivel que esta */
	const int NUMTIPS=4;
	const int LEVELS=NFI-NIN+1;
	const int VARLEV=1+(LEVELS/NUMTIPS);
	const int PRB[]={100,25,10,1}; //probabilidad segun la diferencia de nivel
	int total=0;
	for(int k=0;k<NUMTIPS;k++) total+=pesos[k];
	int tip=-1;
	while(tip==-1) {
		int val=rnd(0,total-1);
		int lim=0;
		int pretip=-1;
		for(int k=0;k<NUMTIPS && pretip==-1;k++) {
			lim+=pesos[k];
			if(val<=lim) pretip=k;
		}
		int levtip=num_nivel/VARLEV;
		int dif=ABS(levtip-pretip);
		int prob=PRB[dif];
		if(rnd(0,99)<prob) tip=pretip;
	}
	return tip;
}

static arma_t* seltipar() {
	arma_t* res=NULL;
	while(!res) {
		int pesos[]={MAL,MAE,MAP,MAM};
		int tip=seltip(pesos);
		arma_t* tiarm[ATI];
		int tiarms=0;
		for(int k=0;k<ATI*ANT;k++) {
			if(arma[k].tia==tip) tiarm[tiarms++]=arma+k;
		}
		int tiar=rnd(0,tiarms-1);
		res=tiarm[tiar];
		if(tip==3 && res->aus==1) res=NULL;
		else if(tip==3) res->aus=1;
	}
	return res;
}

static Bool armnew(){
	/* se crea un arma nueva */
	static Bool tad=FALSE;
	if(!tad) {
		artoti();
		tad=TRUE;
	}
	arma_t* at=seltipar();
	if(at) {
		atributo_t ata={at->chr,BOLD,at->ink,BLACK};
		objeto_t* no=itmnew(at->nom,ata);
		if(no) {
			no->cog=1;
			no->arm=1;
			no->tia=at->tia;
			no->dad=at->dad;
			no->nad=at->nad;
			no->pfu=at->pfu;
			no->pha=at->pha;
			itmplc(no,FALSE);
		} else return FALSE;
	}
	return TRUE;
}

void armlev(uint num) {
	Bool yes=TRUE;
	for(int k=0;k<num && yes;k++) {
		yes=armnew();
	}
}

static void prftin(char* name,char chr,uint tipo,uint valor) {
	/* define un tipo de proteccion */
	static int protecciones=0;
	proteccion_t p;
	char* nd=p.nom;
	char* no=name;
	while(*no!=EOS) *nd++=*no++;
	*nd=EOS;
	p.chr=chr;
	p.tip=tipo;
	p.nar=valor;
	switch(tipo) {
		case 0:
			p.ink=CPB;
			break;
		case 1:
			p.ink=CPR;
			break;
		case 2:
			p.ink=CPA;
			break;
		default:
			p.ink=CPM;
	}
	proteccion[protecciones++]=p;
}

#define ntp(T,C,N,V) prftin(N,(C),(T),(V))

static void prtoti() {
	/* define todos los tipos de proteccion */
	ntp(0,'"',"ropas andrajosas",1);
	ntp(0,'"',"tunica de mendigo",1);
	ntp(0,'^',"chaleco de cuero",2);
	ntp(0,')',"coselete de cuero",3);
	ntp(0,'}',"armadura de cuero",4);
	ntp(0,']',"cota ligera",5);
	ntp(1,'^',"armadura de explorador",6);
	ntp(1,'^',"pechera de bronce",6);
	ntp(1,')',"cota de escamas",7);
	ntp(1,'}',"armadura de soldado",8);
	ntp(1,']',"cota de malla",9);
	ntp(1,']',"pechera de acero",9);
	ntp(2,'"',"armadura de caballero",10);
	ntp(2,'^',"armadura del dragon",11);
	ntp(2,')',"cota negra",12);
	ntp(2,'}',"armadura del bastion",13);
	ntp(3,')',"armadura del campeon",14);
	ntp(3,')',"placas del titan",14);
	ntp(3,']',"armadura celestial",15);
	ntp(3,']',"coraza infinita",15);
}

#undef ntp


static proteccion_t* seltipro() {
	/* escoge un tipo de proteccion */
	proteccion_t* res=NULL;
	while(!res) {
		int pesos[]={MPB,MPR,MPA,MPM};
		int tip=seltip(pesos);
		proteccion_t* prt[NTP];
		int prts=0;
		for(int k=0;k<NTP;k++) {
			if(proteccion[k].tip==tip) {
				prt[prts++]=proteccion+k;
			}
		}
		int npe=rnd(0,prts-1);
		res=prt[npe];
		if(tip==3 && res->aus==1) res=NULL;
		else if(tip==3) res->aus=1;
	}
	return res;
}

static Bool prtnew() {
	/* se crea una proteccion nueva */
	static Bool tpd=FALSE;
	if(!tpd) {
		prtoti();
		tpd=TRUE;
	}
	proteccion_t* pt=seltipro();
	if(pt) {
		atributo_t ata={pt->chr,REVERSE|BOLD,pt->ink,WHITE};
		objeto_t* no=itmnew(pt->nom,ata);
		if(no) {
			no->cog=1;
			no->prt=1;
			no->tip=pt->tip;
			no->nar=pt->nar;
			itmplc(no,FALSE);
		} else return FALSE;
	}
	return TRUE;
}

void prtlev(uint num) {
	Bool yes=TRUE;
	for(int k=0;k<num && yes;k++) {
		yes=prtnew();
	}
}

void itmrmp(objeto_t* o) {
	if(o && o->npc==0 && o->ior==0) {
		o->r=o->c=-1;
		o->con=NULL;
		o->ves=0;
	}
}










