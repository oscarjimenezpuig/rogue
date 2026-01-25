/*
============================================================
  Fichero: memoria.c
  Creado: 23-01-2026
  Ultima Modificacion: vie 23 ene 2026 12:21:15
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

static memoria_t memoria[MSIZ];

static void copyname(char* d,char* o) {
    /* copia los nombres */
    char* pd=d;
    char* po=o;
    while(!*po!=EOS && pd-d<SLEN) *pd++=*po++;
    *pd=EOS;
}

static initname(char* name) {
    *name=EOS;
}

static void onememini(memoria_t* m) {
    initname(m->nme);
    initname(m->nem);
    initname(m->nom);
    m->pme=0;
    m->ani=m->scp=0;
    m->oro=0;
}

static void memini() {
    for(int k=0;k<MSIZ;k++) {
        onememini(m+k);
    }
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

    


