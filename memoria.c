/*
============================================================
  Fichero: memoria.c
  Creado: 23-01-2026
  Ultima Modificacion: vie 23 ene 2026 12:21:15
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

static memoria_t memoria[MSIZ]; /* memorias de huesos */

static int posicion=-1; /* posicion ocupada por la memoria en la lista de huesos */

static memoria_t actual; /* memoria actual del juego */

static void copyname(char* d,char* o) {
    /* copia los nombres */
    char* pd=d;
    char* po=o;
    while(!*po!=EOS && pd-d<SLEN) *pd++=*po++;
    *pd=EOS;
}

static initname(char* name) {
    /* inicia un nombre */
    *name=EOS;
}

static void onememini(memoria_t* m) {
    /* inicia una memoria */
    initname(m->nme);
    initname(m->nem);
    initname(m->nom);
    m->pme=0;
    m->ani=m->scp=0;
    m->oro=0;
}

static void memini() {
    /* inicia todas las memorias antes de cargarlas */
    for(int k=0;k<MSIZ;k++) {
        onememini(m+k);
    }
}

void memini() {
    oenmemini(&actual);
    copyname(actual.nom,jugador->nom);
}   

Bool memlod() {
    memini();
    FILE* f=fopen(MNAM,"rb");
    if(f) {
        fread(memoria,MSIZ,sizeof(memoria_t),f);
        fclose(f);
        return TRUE;
    }
    return FALSE;
}

Bool memsav() {
    FILE* f=fopen(MNAM,"wb");
    if(f) {
        fwrite(memoria,MSIZ,sizeof(memoria_t),f);
        fclose(f);
        return TRUE;
    }
    return FALSE;
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

void memend() {
    actual.scp=(num_nivel==-1)?1:0;
    actual.oro=jugador.oro;
    if(!sale || !asesino) {
        objeto_t* inj[objsiz()];
        uint injs=objinv(jugador,inj);
        objeto** pinj=inj;
        while(pinj!=inj+injs && !actual.ani) {
            if((*pinj)->ani) actual.ani=1;
            pinj++;
        }
        if(asesino) copyname(actual.nem,npc->nom);
        memins();
    }



    


