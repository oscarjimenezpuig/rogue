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
    uchr fnv : 1; /* la posicion es frontera con un no visible */
} visual_t;

typedef struct {
    visual_t vis[MAPAR][MAPAC]; //guarda todos los datos de la visualizacion
    int ri,ci; //extremo superior izquierdo de la parte visual
    int rf,cf;  //extremo inverior derecho de la parte visual
    uint npc;
    objeto_t* obj;//objeto mas cercano de la visual 
    Bool jug; //el jugador existe en la visual
} vismap_t;

/* funciones auxiliares de ia */

vismap_t vismap;
static Bool isobjnpc(objeto_t* o) {
    if(o && o->npc && o->vid>0 && o->jug==0 && vismap.vis[o->r][o->c].fre==1) {
        return TRUE;
    }
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
    vm.npc=0;
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
                if(l->trs==1 && l->hab==le->hab) {
                    vm.vis[rr][cc].fre=1;
                    if(rr<vm.ri) vm.ri=rr;
                    if(rr>vm.rf) vm.rf=rr;
                    if(cc<vm.ci) vm.ci=cc;
                    if(cc>vm.cf) vm.cf=cc;
}
            }
        }
    }
     /* coloca el jugador */
    if(le->vis==2 && jugador->vid>0) {
        vm.jug=TRUE;
        vm.vis[jugador->r][jugador->c].jug=1;
        vm.vis[jugador->r][jugador->c].fre=0;
        /* busca fronteras a no visibles del mapa de visibilidad */
        for(int rr=vm.ri;rr<=vm.rf;rr++) {
            for(int cc=vm.ci;cc<=vm.cf;cc++) {
                if(rr!=e->r || cc!=e->c) {
                    if(vm.vis[rr][cc].fre==1) {
                        localidad_t* n[4];
                        mapngh(rr,cc,n);
                        for(int k=0;k<4;k++) {
                            localidad_t* le=n[k];
                            if(le && le->trs==1 && le->vis!=2) {
                                vm.vis[rr][cc].fnv=1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    /* coloca todos los objetos npc */
    objeto_t* obj[objsiz()];
    vismap=vm;
    vm.npc=objfnd(obj,isobjnpc);
    for(int k=0;k<vm.npc;k++) {
        objeto_t* npc=obj[k];
        vm.vis[npc->r][npc->c].fre=0;
    }
    /* da el objeto mas cercano (comprueba si esta vestido con armadura y si tiene espacio) */
    uint invs=objinv(e,obj);
    if(invs<e->cap) {
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
    }
    /* retorno del resultado */
    return vm;
}

#define con(A) (DRCS-1-(A))

static Bool _moveto(objeto_t* e,vismap_t vm,int rf,int cf) {
    const int DR[]=DRC;
    int ri=e->r;
    int ci=e->c;
    int ra=e->dr;
    int ca=e->dc;
    int disini=mapdis(ri,ci,rf,cf);
    int dis=disini;
    int par=-1;
    for(int k=0;k<DRCS;k++) {
        int dr=ri+DR[k];
        int dc=ci+DR[con(k)];
        if(vm.vis[dr][dc].fre==1 && (dr!=ra || dc!=ca)) {
            int die=mapdis(dr,dc,rf,cf);
            if(die<dis || (die==dis && rnd(0,1))) {
                dis=die;
                par=k;
            }
        }
    }
    int desr,desc;
    desr=desc=0;
    if(par!=-1) {
        desr=DR[par];
        desc=DR[con(par)];
    } else if(ra!=-1 && ca!=-1) {
        int disra=mapdis(ra,ca,rf,cf);
        if(disra<disini) {
            desr=SGN(ra,ri);
            desc=SGN(ca,ci);
        }
    }
    if(desr!=0 || desc!=0) {
        if(objmov(e,desr,desc)) {
            e->dr=ri;
            e->dc=ci;
            return TRUE;
        }
    }
    return FALSE;
}

static Bool _acercarse(objeto_t* e,vismap_t vm) {
    /* da las coordenadas del punto libre mas cercano al jugador si existiese */
    int r,c;
    int dis=-1;
    for(int rr=vm.ri;rr<=vm.rf;rr++) {
        for(int cc=vm.ci;cc<=vm.cf;cc++) {
            if(vm.vis[rr][cc].fre==1) {
                int die=mapdis(jugador->r,jugador->c,rr,cc);
                if(dis==-1 || die<dis) {
                    dis=die;
                    r=rr;
                    c=cc;
                }
            }
        }
    }
    if(dis!=-1) return _moveto(e,vm,r,c);
    else return FALSE;
}

static Bool _esconderse(objeto_t* e,vismap_t vm) {
    /* comprueba si el enemigo esta en frontera de parte no visible y se mueve a la parte nov visible */
    const int DRP[]=DRC;
    int er=e->r;
    int ec=e->c;
    visual_t ve=vm.vis[er][ec];
    if(ve.fnv) {
        dbgprt("%s en frontera no visible");//dbg
        for(int k=0;k<4;k++) {
            int dr=DRP[k];
            int dc=DRP[con(k)];
            localidad_t* l=mapget(er+dr,ec+dc);
            if(l && l->trs==1 && l->vis!=2 && objmov(e,dr,dc)) {
                e->dr=er;
                e->dc=ec;
                return TRUE;
            }
        }
    }
    return FALSE;
}

static Bool _alejarse(objeto_t* e,vismap_t vm) {
    int rlej,clej,rfnv,cfnv;
    int dlej=-1;
    int dfnv=-1;
    for(int rr=vm.ri;rr<=vm.rf;rr++) {
        for(int cc=vm.ci;cc<=vm.cf;cc++) {
            visual_t ve=vm.vis[rr][cc];
            if(ve.fre==1) {
                int de=mapdis(e->r,e->c,rr,cc);
                if(ve.fnv==1) {
                    if(dfnv==-1 || de<dfnv) {
                        dfnv=de;
                        rfnv=rr;
                        cfnv=cc;
                    } else if(dlej==-1|| dlej<de) {
                        dlej=de;
                        rlej=rr;
                        clej=cc;
                    }
                }
            }
        }
    }
    int djlej=(dlej==-1)?-1:mapdis(jugador->r,jugador->c,rlej,clej);
    int djfnv=(dfnv==-1)?-1:mapdis(jugador->r,jugador->c,rfnv,cfnv);
    int desr,desc;
    desr=desc=-1;
    int try=0;
    if(djfnv!=-1 && djlej!=-1) {
        if(djfnv>djlej) try=-1;
        else try=1;
    } else if(djlej!=-1) try=1;
    else if(djfnv!=-1) try=-1;
    if(try==1) {
        desr=rlej;
        desc=clej;
    } else if(try==-1) {
        desr=rfnv;
        desc=cfnv;
    }
    if(try) return _moveto(e,vm,desr,desc);
    return FALSE;
}

static Bool _toobj(vismap_t vm,int* r,int* c) {
    /* da las coordenadas del objeto mas cercano (si existe) */
    if(vm.obj) {
        *r=vm.obj->r;
        *c=vm.obj->c;
        return TRUE;
    }
    return FALSE;
}

static int _atabal(objeto_t* e) {
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

static double _armval(objeto_t* e,objeto_t*a) {
    /* da una puntuacion al arma de ataque a partir de las caracteristicas */
	const double MD[]={0.0,2.5,3.5,4.5,6.5,10.5};
	double hab=e->hab+(a->pha==0)?0:(e->hab/a->pha);
	double fue=e->fue+(a->pfu==0)?0:(e->fue/a->pfu);
	double dad=MD[a->dad]*(double)a->nad;
	return hab+fue+dad;
}

/* funciones principales de ia */

static Bool iaatc(vismap_t vm,objeto_t* e) {
    /* gestiona el ataque de un enemigo si el jugador esta presente */
    /* res={0: nada, 1: acercarse 2: alejarse 3: atacar} */
    int res=0;
    if(vm.jug) {
        uint dje=objdis(e,jugador);
        if(e->anm) {
            if(dje==1) res=3;
            else res=1;
        } else {
            int atsim=_atabal(e);
            if(atsim>0 || vm.npc>1) {
               if(dje==1) res=3;
                else res=1;
            } else {
                if(dje>1 || (dje==1 && e->vel>jugador->vel)) res=2;
                else res=3;
            }
        }
        if(res==3) {
            return objata(e,jugador);
        } else {
            if(res==2) {
                if(_esconderse(e,vm)) return TRUE;
                else return _alejarse(e,vm);
            } else if(res==1) {
                return _acercarse(e,vm);
            }
        }
    }
    return FALSE;
}
  
static Bool iacog(vismap_t vm,objeto_t* e) {
    /* intenta coger un objeto */
    objeto_t* oc=vm.obj;
    if(oc) {
        if(oc->r==e->r && oc->c==e->c) return objcog(e,oc);
        else {
            int r,c;
            if(_toobj(vm,&r,&c)) return _moveto(e,vm,r,c);
        }
    }
    return FALSE;
}

static Bool iavst(objeto_t* e) {
    /* intenta vestir un objeto */
    objeto_t* inv[objsiz()];
    uint invs=objinv(e,inv);
    double ppoav=0;
    double ppoad=0;
    objeto_t* poad=NULL;
    objeto_t* poav=NULL;
    for(int k=0;k<invs;k++) {
        objeto_t* oe=inv[k];
        if(oe->arm) {
            double pun=_armval(e,oe);
            if(oe->ves) {
                poad=oe;
                ppoad=pun;
            } else {
                if(ppoav<pun) {
                    ppoav=pun;;
                    poav=oe;
                }
            }
        }
    }
    if(poad && ppoad<ppoav) {
        return objdes(e,poad);
    } else if(!poad && poav) {
        return objves(e,poav);
    }
    return FALSE;
}
    
static Bool iawlk(vismap_t vm,objeto_t* e) {
    static int rndr=-1;
    static int rndc=-1;
    if(rndr==-1 && rndc==-1) {
        maprndpos(&rndr,&rndc,TRUE);
    }
    if((e->r==rndr && e->c==rndc) || (!_moveto(e,vm,rndr,rndc))) {
        rndr=rndc=-1;
        return FALSE;
    }
    return TRUE;
}

/* acciones de ia */

Bool iaanm(objeto_t* e) {
    vismap_t vm=_visset(e);
    if(!iaatc(vm,e)) {
        return iawlk(vm,e);
    }
    return TRUE;
}

Bool iahum(objeto_t* e) {
    vismap_t vm=_visset(e);
    if(!iaatc(vm,e)) {
        if(!iacog(vm,e)) {
            if(!iavst(e)) {
                return iawlk(vm,e);
            }
        }
    }
    return TRUE;
}
         
    

    






     


            



