/* ROGUE 30/10/25 */

#include "rogue.h"

#define DRC {0,0,-1,1}
#define DRCS 4

typedef unsigned char uchr; 

/* visual */

typedef struct {
    uchr ocp : 1; /* la posicion esta ocupada */
    uchr jug : 1; /* la posicion tiene jugador */
    uchr tes : 1; /* la posicion tiene tesoro */
    uchr ata : 1; /* la posicion tiene arma de ataque */
    uchr def : 1; /* la posicion tiene arma de defensa */
} visual_t

visual_t vismap[MAPAR][MAPAC]; /* mapa de visibilidad */

#define fr(A,L) for((A)=0;(A)<(L);(A)++)

static void _visini() {
    /* inicia el mapa visual poniendolo todo en ocupado */
    int r,c;
    fr(c,MAPAC) {
        fr(r,MAPAR) {
            vismap[r][c]=(visual_t){1,0,0,0,0};
        }
    }
}

static void _visosc(int r,int c) {
    /* visibilidad de la parte oscura */
    const int DI[]=DSC;
    int k;
    fr(k,DRCS) {
        int re=r+DI[k];
        int ce=c+DI[DRCS-1-k];
        localidad_t* l=mapget(re,ce);
        if(l->trs==1) vismap[re][ce].ocp=0;
    }
}

static void _visclr(int r,int c,uint hab) {
    /* visibilidad de la parte clara */
    int rr,cc;
    fr(rr,MAPAR) {
        fr(cc,MAPAC) {
            if(rr!=r || cc!=c) {
                localidad_t* l=mapget(rr,cc);
                if(l->trs==1 && l->hab==hab) {
                    vismap[rr][cc].ocp=0;
                }
            }
        }
    }
}

static Bool _convisobj(objeto_t* o) {
    if(o) {
        visual_t* v=&(vismap[o->r][o->c]);
        if(v->ocp==0) {
            if(o->npc && o->vid>0) {
                if(o->jug) v->jug=1;
                else v->ocp=1;
            } else {
                if(o->ior) v->tes=1;
                else if(o->arm) v->ata=1;
                else if(o->prt) v->def=1;
            }
        }
    }
    return FALSE;
}

static void _visobj() {
    objfnd(NULL,_convisobj);
}          

static void visset(objeto_t* npc) {
    /* determina la parte visual del mapa */
    const int DI[]=DRC;
    _visini();
    localidad_t* l=mapget(npc->r,npc->c);
    if(l && l->trs) {
        if(l->osc) _visosc(npc->r,npc->c);
        else _visosc(npc->r,npc->c,l->hab);
        _visobj();
    }
}

/* camino */

struct paso_s {
    int r,c;
    uint num;
    struct paso_s* prv;
    struct paso_s* nxt[DRCS];
};

struct paso_s* _pasnew(int r,int c) {
    /* creacion de paso inicial */
    struct paso_s* p=malloc(sizeof(struct paso_s));
    if(p) {
        p->r=r;
        p->c=c;
        p->num=0;
        p->prv=NULL;
        int k;
        fr(k,DRCS) p->nxt[k]=NULL;
    }
}

struct void _pasdel(struct paso_s* p) {
    if(p) {
        int k;
        fr(k,DRCS) {
            _pasdel(p->nxt[k]);
        }
        free(p);
    }
}

static Bool _pasalr(int r,int c,struct paso_t* p) {
    /* mira si en algun momento del arbol ya tenemos este paso */
    struct paso_t* act=p;
    while(act) {
        if(act->r==r && act->c==c) return TRUE;
        act=act->prv;
    }
    return FALSE;
}

static struct paso_s* _pashij(struct paso_s* padre,int nh,int nr,int nc) {
    /* creacion del paso hijo a partir del padre y de la direccion de movimiento */
    struct paso_s* p=NULL;
    if(padre && (dr!=0 || dc!=0) && nh<DRCS) {
        p=_pasnew(nr,nc);
        if(p) {
            p->num=padre->num+1;
            p->prv=padre;
            padre->nxt[nh]=p;
        }
    }
    return p;
}

static struct paso_s* _pasrec(struct paso_t* padre,int rf,int cf) {
    /* funcion recursiva que crea el arbol de pasos hasta que llega al destino */
    const int DRM[]=DRC;
    struct paso_s* ret=NULL;
    if(padre) {
        int pc=padre->c;
        int pr=padre->r;
        if(pr==rf && pc==cf) {
            ret=padre;
        } else {
            int k;
            fr(k,DRCS) {
                int fc=pc+DRM[k];
                int fr=pr+DRM[DRCS-1-k];
                if(vismap[fr][fc].ocp==0 && !_pasalr(fr,fc,padre)) {
                    struct paso_s* pret=NULL;
                    if((pret=_pasrec(_pashij(padre,k,fr,fc)))) {
                        if(!ret || ret->num>pret->num) {
                            ret=pret;
                        }
                    }
                }
            }
        }
    }
    return r;
    //TODO: Se debe devolver la mas pequeño de todas las longitudes
}

static struct paso_s* _camnew(int ri,int ci,int rf,int cf) {
    /* se hace camino de ri,ci a rf,cf, si no llega, devuelve NULL */
    struct paso_s* root=_pasnew(ri,ci);
    struct paso_s* result=_pasrec(root,rf,cf);
    if(!result) {
        _pasdel(root);
        root=NULL;





    
    
