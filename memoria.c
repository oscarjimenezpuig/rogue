/* ROGUE 23/1/26 */

#include <time.h>

#include "rogue.h"

static memoria_t memoria[MSIZ]; /* memorias de huesos */

static memoria_t actual; /* memoria actual del juego */

static void copyname(char* d,char* o) {
    /* copia los nombres */
    char* pd=d;
    char* po=o;
    while(*po!=EOS && pd-d<SLEN) *pd++=*po++;
    *pd=EOS;
}

static void initname(char* name) {
    /* inicia un nombre */
    *name=EOS;
}

static void onememclr(memoria_t* m) {
    /* inicia una memoria */
    initname(m->nme);
    initname(m->nem);
    initname(m->nom);
    initname(m->tim);
    m->pme=0;
    m->ani=m->scp=0;
    m->oro=0;
    m->niv=0;
}

static void memclr() {
    /* inicia todas las memorias antes de cargarlas */
    for(int k=0;k<MSIZ;k++) {
        onememclr(memoria+k);
    }
}

static Bool memlod() {
    /* carga las memoria de huesos guardada */
    memclr();
    FILE* f=fopen(MNAM,"rb");
    if(f) {
        fread(memoria,MSIZ,sizeof(memoria_t),f);
        fclose(f);
        return TRUE;
    }
    return FALSE;
}

static Bool memsav() {
    /* guarda el archivo de la memoria de huesos */
    FILE* f=fopen(MNAM,"wb");
    if(f) {
        fwrite(memoria,MSIZ,sizeof(memoria_t),f);
        fclose(f);
        return TRUE;
    }
    return FALSE;
}

void memini() {
    onememclr(&actual);
    copyname(actual.nom,jugador->nom);
    actual.niv=1;
    memlod();
}   



void meminsmat(objeto_t* npc) {
    if(npc) {
        uint points=npc->fue+npc->hab+npc->vel+npc->cap;
        if(points>actual.pme) {
            actual.pme=points;
            copyname(actual.nme,npc->nom);
        }
    }
}

void memactniv() {
    if(actual.niv<num_nivel) actual.niv=num_nivel;
}

#define cmp(A,B) (((A)>(B))?1:((A)<(B))?-1:0)

static int abetb(memoria_t a,memoria_t b) {
    int r=cmp(a.scp,b.scp);
    if(r==0) {
        r=cmp(a.ani,b.ani);
        if(r==0) {
            r=cmp(a.pme,b.pme);
            if(r==0) {
                r=cmp(a.niv,b.niv);
                if(r==0) r=cmp(a.oro,b.oro);
            }
        }
    }
    return r;
}

#undef cmp

static void memcopy(memoria_t* d,memoria_t o) {
    copyname(d->nom,o.nom);
    copyname(d->nme,o.nme);
    copyname(d->nem,o.nem);
    copyname(d->tim,o.tim);
    d->pme=o.pme;
    d->ani=o.ani;
    d->scp=o.scp;
    d->oro=o.oro;
    d->niv=o.niv;
}

static int memord() {
    /* coloca la memoria actual dentro del array siguiendo el orden */   
    for(int k=0;k<MSIZ;k++) {
        memoria_t* pm=memoria+k;
        if(*(pm->nom)==EOS || abetb(actual,*pm)==1) {
            for(int n=MSIZ-1;n>=k;n--) {
                memcopy(memoria+(n),memoria[n-1]);
            }
            memcopy(pm,actual);
            return k;
        }
    }
    return -1;
}

#define NLN COL=0;ROW++

static void onememprt(Bool reverse,int pos,memoria_t m) {
    /* imprime una memoria */
    ATR=(reverse)?BOLD|REVERSE:BOLD;
    prints("%i %s (%s)",pos,m.nom,m.tim);
    ATR=(reverse)?REVERSE:NONE;
    NLN;
    prints("    Llego al nivel %i.",m.niv);
    if(m.scp) {
        NLN;
        prints("    Logro escapar con el anillo!!!");
    } else {
        if(m.ani) {
            NLN;
            prints("    Consiguio el anillo, pero no pudo huir...");
        }
        NLN;
        prints("    Fue muerto por %s.",m.nem);
    }
    if(m.pme>0) {
        NLN;
        prints("    Consiguio matar a %s.",m.nme);
    }
    if(m.oro==1) {
        NLN;
        prints("    Acabo con una moneda de oro.");
    } else if(m.oro>1) {
        NLN;
        prints("    Acabo con %i monedas de oro.",m.oro);
    }
}

static void memprt(int posicion) {
    /* impresion del archivo */
    const int ITI=YELLOW;
    const char* TIT="ARCHIVO DE HUESOS";
    const int IFR=WHITE;
    static int titlen=-1;
    if(titlen==-1) {
        const char* p=TIT;
        while(*p!=EOS) p++;
        titlen=p-TIT;
    }
    cls();
    ROW=0;
    COL=(COLS-titlen)/2;
    INK=ITI;
    ATR=BOLD;
    prints(TIT);
    ROW+=1;
    INK=IFR;
    int ndm=MIN(((ROWS-4)/6),MSIZ);
    for(int k=0;k<ndm;k++) {
        if(*(memoria[k].nom)!=EOS) {
            NLN;
            onememprt((k==posicion),k+1,memoria[k]);
        } else break;
    }
    ROW=ROWS-1;
    COL=0;
    ATR=BLINK;
    prints("Pulsa Q para finalizar");
}

static void timset() {
    /* establece el tiempo actual */
    time_t ti;
    time(&ti);
    struct tm* tm=localtime(&ti);
    strftime(actual.tim,SLEN,"%d/%m/%Y",tm);
}

void memend() {
    actual.scp=(end_game==2)?1:0;
    actual.oro=jugador->oro;
    timset();
    if(!actual.scp || !asesino) {
        objeto_t* inj[objsiz()];
        uint injs=objinv(jugador,inj);
        objeto_t** pinj=inj;
        while(pinj!=inj+injs && !actual.ani) {
            if((*pinj)->ani) actual.ani=1;
            pinj++;
        }
        if(asesino) copyname(actual.nem,asesino->nom);
        int posicion=memord();
        memprt(posicion);
        if(posicion!=-1) memsav();
    }
}





    


