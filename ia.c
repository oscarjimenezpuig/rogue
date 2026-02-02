/* ROGUE 2/2/25 */

#include "rogue.h"

#define DRC {0,0,-1,1}
#define DRCS 4

typedef unsigned char uchr; 

/* visual */

typedef struct {
    uchr fre : 1; /* la posicion esta libre */
    uchr jug : 1; /* la posicion tiene jugador */
    uchr obj : 1; /* la posicion tiene tesoro */
    uchr osc : 1; /* la posicion tiene frontera oscura */
} visual_t;

typedef struct {
    visual_t vis[MAPAR][MAPAC]; //guarda todos los datos de la visualizacion
    int ri,ci; //extremo superior izquierdo de la parte visual
    int rf,cf;  //extremo inverior derecho de la parte visual
    objeto_t* obj;//objeto mas cercano de la visual 
    Bool jug; //el jugador existe en la visual
} vismap_t;

/* funciones auxiliares de ia */

vismap_t vismap;
static Bool isobjnpc(objeto_t* o) {
    if(o && o->npc && o->vid>0 && vismap.vis[o->r][o->c].fre==1) return TRUE;
    return FALSE;
}
static Bool isobjnonpc(objeto_t* o) {
    if(o && o->npc==0 && vismap.vis[o->r][o->c].fre==1) return TRUE;
    return FALSE;
}

static vismap_t _visset(objeto_t* e) {
    /* determina la visual del enemigo */
    /* inicia el visual */
    vismap_t vm;
    for(int rr=0;rr<MAPAR;rr++) {
        for(int cc=0;cc<MAPAC;cc++) {
            vm.vis[rr][cc]=(visual_t){0,0,0,0};
        }
    }
    vm.rf=vm.cf=0;
    vm.ri=MAPAR-1;
    vm.ci=MAPAC-1;
    vm.obj=NULL;
    vm.jug=FALSE;
    /* coloca las partes transitables del mapa de visibilidad */
    localidad_t* le=mapget(e->r,e->c);
    if(le && le->osc) {
        for(int rr=e->r-1;rr<=e->r+1;rr++) {
            for(int cc=e->c-1;cc<=e->c+1;cc++) {
                localidad_t* l=mapget(rr,cc);
                if(l->trs==1 && mapdis(rr,cc,e->r,e->c)==1) vm.vis[rr][cc].fre=1;
            }
        }
        vm.ri=e->r-1;
        vm.ci=e->c-1;
        vm.rf=e->r+1;
        vm.cf=e->c+1;
    } else if(le) {
        for(int rr=0;rr<MAPAR;rr++) {
            for(int cc=0;cc<MAPAC;cc++) {
                localidad_t* l=mapget(rr,cc);
                if(l->trs==1 && l->hab==e->hab) {
                    vm.vis[rr][cc].fre=1;
                    if(rr<vm.ri) vm.ri=rr;
                    if(rr>vm.rf) vm.rf=rr;
                    if(cc<vm.ci) vm.ci=cc;
                    if(cc>vm.cf) vm.cf=cc;
}
            }
        }
    }
    /* busca fronteras oscuras del mapa de visibilidad */
    for(int rr=vm.ri;rr<=vm.rf;rr++) {
        for(int cc=vm.ci;cc<=vm.cf;cc++) {
            if(rr!=e->r || cc!=e->c) {
                if(vm.vis[rr][cc].fre==1) {
                    localidad_t* n[4];
                    mapngh(rr,cc,n);
                    for(int k=0;k<4;k++) {
                        localidad_t* le=n[k];
                        if(le && le->trs==1 && le->osc==1) {
                            vm.vis[rr][cc].osc=1;
                            break;
                        }
                    }
                }
            }
        }
    }
    /* coloca el jugador */
    if(le->vis==2) {
        vm.jug=TRUE;
        vm.vis[jugador->r][jugador->c].jug=TRUE;
        vm.vis[jugador->r][jugador->c].fre=0;
    }
    /* coloca todos los objetos npc */
    objeto_t* obj[objsiz()];
    vismap=vm;
    uint npcs=objfnd(obj,isobjnpc);
    for(int k=0;k<npcs;k++) {
        objeto_t* npc=obj[k];
        vm.vis[npc->r][npc->c].fre=0;
    }
    /* da el objeto mas cercano (comprueba si esta vestido con armadura) */
    objeto_t* eprt=objisprt(e);
    uint nits=objfnd(obj,isobjnonpc);
    int dr=-1;
    objeto_t* ite=NULL;
    for(int k=0;k<nits;k++) {
        objeto_t* itm=obj[k];
        if(itm->prt==0 || (eprt==NULL)) {
            int ditm=objdis(e,itm);
            if(dr==-1 || (ditm<dr)) {
                ite=itm;
                dr=ditm;
            }
        }
    }
    if(ite) {
        vm.obj=ite;
        vm.vis[ite->r][ite->c].obj=1;
    }
    /* retorno del resultado */
    return vm;
}

static void _moveini(objeto_t* e) {
    /* inicia el movimiento de un objeto */
    e->dr=e->dc=-1;
}

#define con(A) (DRCS-1-(A))

static Bool _moveto(objeto_t* e,vismap_t vm,int rf,int cf) {
    const int DR[]=DRC;
    int ri=e->r;
    int ci=e->c;
    int ra=e->dr;
    int ca=e->dc;
    int dis=-1;
    int par=-1;
    for(int k=0;k<DRCS;k++) {
        int dr=ri+DR[k];
        int dc=ci+DR[con(k)];
        if(vm.vis[dr][dc].fre==1 && (dr!=ra || dc!=ca)) {
            int die=mapdis(dr,dc,rf,cf);
            if(dis==-1 || die<dis || (die==dis && rnd(0,1))) {
                dis=die;
                par=k;
            }
        }
    }
    int desr,desc;
    desr=desc=-1;
    if(par!=-1) {
        desr=DR[par];
        desc=DR[con(par)];
    } else if(ra!=-1 && ca!=-1) {
        desr=SGN(ra,ri);
        desc=SGN(ca,ci);
    }
    if(desr!=-1 && desc!=-1) {
        if(objmov(e,desr,desc)) {
            e->dr=ri;
            e->dc=ci;
            return TRUE;
        }
    }
    return FALSE;
}

static Bool _nearjug(vismap_t vm,int* r,int* c) {
    /* da las coordenadas del punto libre mas cercano al jugador si existiese */
    int dis=-1;
    for(int rr=vm.ri;rr<=vm.rf;rr++) {
        for(int cc=vm.ci;cc<=vm.cf;cc++) {
            if(vm.vis[rr][cc].fre==1) {
                int die=mapdis(jugador->r,jugador->c,rr,cc);
                if(dis==-1 || die<dis) {
                    dis=die;
                    *r=rr;
                    *c=cc;
                }
            }
        }
    }
    return (dis!=-1);
}

static Bool _farjug(objeto_t* e,vismap_t vm,int* r,int* c) {
    /* da las coordenadas de un punto alejado del jugador y si puede ser, fronterizo a oscuridad */
    int dis=-1;
    for(int rr=vm.ri;rr<=vm.rf;rr++) {
        for(int cc=vm.ci;cc<=vm.cf;cc++) {
            visual_t ve=vm.vis[rr][cc];
            if(ve.fre==1) {
                int dae=mapdis(e->r,e->c,rr,cc);
                int daj=mapdis(jugador->r,jugador->c,rr,cc);
                if(dae<=daj) {
                    if(ve.osc==1) {
                        *r=rr;
                        *c=cc;
                        return TRUE;
                    } else if(daj>dis) {
                        dis=daj;
                        *r=rr;
                        *c=cc;
                    }
                }
            }
        }
    }
    return (dis!=-1);
}  

/* acciones de ia */



    






     


            



