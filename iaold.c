/* ROGUE 30/10/25 */

#include <stdlib.h>

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
} visual_t;

static visual_t vismap[MAPAR][MAPAC]; /* mapa de visibilidad */
static uint ntes; /* numero de tesoros en visualizacion */
static uint nata; /* numero de armas de ataque en visualizacion */
static uint ndef; /* numero de armas de defensa en visualizacion */
static uint nene; /* numero de enemigos en visualizacion */
static Bool hjug; /* esta el jugador presente */

#define fr(A,L) for((A)=0;(A)<(L);(A)++)

static void _visini() {
    /* inicia el mapa visual poniendolo todo en ocupado */
    int r,c;
    fr(c,MAPAC) {
        fr(r,MAPAR) {
            vismap[r][c]=(visual_t){1,0,0,0,0};
        }
    }
    ntes=nata=ndef=nene=0;
    hjug=FALSE;
}

static void _visosc(int r,int c) {
    /* visibilidad de la parte oscura */
    const int DI[]=DRC;
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
    /* condicion de visualizacion de objetos */
    if(o) {
        visual_t* v=&(vismap[o->r][o->c]);
        if(v->ocp==0) {
            if(o->npc && o->vid>0) {
                if(o->jug) {
                    v->jug=1;
                    hjug=TRUE;
                } else {
                    v->ocp=1;
                    nene++;
                }
            } else {
                if(o->ior) {
                    v->tes=1;
                    ntes++;
                } else if(o->arm) {
                    v->ata=1;
                    nata++;
                } else if(o->prt) {
                    v->def=1;
                    ndef++;
                }
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
    _visini();
    localidad_t* l=mapget(npc->r,npc->c);
    if(l && l->trs) {
        if(l->osc) _visosc(npc->r,npc->c);
        else _visclr(npc->r,npc->c,l->hab);
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
    /* creacion de paso inicial, solo si el lugar no esta ocupado */
    struct paso_s* p=NULL;
    if(vismap[r][c].ocp==0) {
        p=malloc(sizeof(struct paso_s));
        if(p) {
            p->r=r;
            p->c=c;
            p->num=0;
            p->prv=NULL;
            int k;
            fr(k,DRCS) p->nxt[k]=NULL;
        }
    }
    return p;
}

static void _pasdel(struct paso_s* p) {
    if(p) {
        int k;
        fr(k,DRCS) {
            _pasdel(p->nxt[k]);
        }
        free(p);
    }
}

static Bool _pasalr(int r,int c,struct paso_s* p) {
    /* mira si en algun momento del arbol ya tenemos este paso */
    struct paso_s* act=p;
    while(act) {
        if(act->r==r && act->c==c) return TRUE;
        act=act->prv;
    }
    return FALSE;
}

static struct paso_s* _pashij(struct paso_s* padre,int nh,int nr,int nc) {
    /* creacion del paso hijo a partir del padre y de la direccion de movimiento */
    struct paso_s* p=NULL;
    if(padre && nh<DRCS) {
        p=_pasnew(nr,nc);
        if(p) {
            p->num=padre->num+1;
            p->prv=padre;
            padre->nxt[nh]=p;
        }
    }
    return p;
}

static void _pasrec(struct paso_s** retorno,struct paso_s* padre,int rf,int cf) {
    /* funcion recursiva que crea el arbol de pasos hasta que llega al destino */
    const int DRM[]=DRC;
    if(padre) {
        int pc=padre->c;
        int pr=padre->r;
        if(pr==rf && pc==cf) {
            *retorno=padre;
        } else if((*retorno)==NULL || (*retorno)->num>padre->num){
            int k;
            fr(k,DRCS) {
                int fc=pc+DRM[k];
                int fr=pr+DRM[DRCS-1-k];
                if(!_pasalr(fr,fc,padre)) {
                    struct paso_s* hijo=NULL;
                    if((hijo=_pashij(padre,k,fr,fc))) {
                        _pasrec(retorno,hijo,rf,cf);
                    }
                }
            }
        } else {
            _pasdel(padre);
        }
    }
}

/* funciones secundarias */

static Bool _iamoveto(objeto_t* e,int rf,int cf) {
    /* funcion que decide de las cuatro direcciones que tiene un objeto una de ellas que nos lleva al destino */
    const int DRM[]=DRC;
    int ri=e->r;
    int ci=e->c;
    if(rf!=ri || cf!=ci) {
        int dr,dc;
        dr=dc=-1;
        int npas=-1;
        struct paso_s *root,*rslt;
        int k;
        fr(k,DRCS) {
            int der=DRM[k];
            int dec=DRM[DRCS-1-k];
            rslt=NULL;
            root=_pasnew(ri+der,ci+dec);
            _pasrec(&rslt,root,rf,cf);
            if(rslt && (npas==-1 || rslt->num<npas)) {
                dr=der;
                dc=dec;
                npas=rslt->num;
            }
            _pasdel(rslt);
        }
        if(npas!=-1) {
            return objmov(e,dr,dc);
        }
    }
    return FALSE;
}

static Bool _iamovtojug(objeto_t* e) {
    /* decide una posicion al lado del jugador para moverse */
    const int DRM[]=DRC;
    int rf,cf;
    int dis=-1;
    if(objdis(e,jugador)>1) {
        int k;
        fr(k,DRCS) {
            int r=jugador->r+DRM[k];
            int c=jugador->c+DRM[DRCS-1-k];
            if(vismap[r][c].ocp==0) {
                int de=mapdis(e->r,e->c,r,c);
                if(dis==-1 || de<dis) {
                    rf=r;
                    cf=c;
                    dis=de;
                }
            }
        }
        if(dis!=-1) {
            return _iamoveto(e,rf,cf);
        }
    }
    return FALSE;
}

static Bool _iamovnojug(objeto_t* e) {
    /* nos intentamos alejar del jugador */
    int rf,cf;
    int dis=-1;
    int rr,cc;
    fr(rr,MAPAR) {
        fr(cc,MAPAC) {
            if(vismap[rr][cc].ocp==0) {
                int de=objdis(e,jugador);
                if(de>dis || (de==dis && rnd(0,1))) {
                    rf=rr;
                    cf=cc;
                    dis=de;
                }
            }
        }
    }
    if(dis!=-1) return _iamoveto(e,rf,cf);
    return FALSE;
}

static int _iaefeata(objeto_t* e) {
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

static Bool _iafndobj(objeto_t* e,objeto_t* prt,int* rf,int* cf) {
    /* localiza un objeto cercano si lo hay*/
    int dis=-1;
    Bool controla=(prt==NULL)?(ntes || nata || ndef):(ntes || nata);
    if(controla) {
        int rr,cc;
        fr(rr,MAPAR) {
            fr(cc,MAPAC) {
                visual_t v=vismap[rr][cc];
                int controlb=(prt==NULL)?v.tes+v.ata+v.def:v.tes+v.ata;
                if(controlb>0) {
                    int de=mapdis(e->r,e->c,rr,cc);
                    if(dis==-1 || de<dis) {
                        *rf=rr;
                        *cf=cc;
                        dis=de;
                    }
                }
            }
        }
    }
    return (dis!=-1);
}

static objeto_t* estudiado=NULL;
static Bool acpprt=TRUE;
static Bool isoip(objeto_t* o) {
    if (o && o->npc==0 && o->r==estudiado->r && o->c==estudiado->c) {
        if(acpprt || (!acpprt && o->prt==0)) return TRUE;
    }
    return FALSE;
}

static objeto_t* _iaobjip(objeto_t* e,objeto_t* prt) {
    /* dice si hay algun objeto en la posicion del enemigo y quien es */
    visual_t v=vismap[e->r][e->c];
    uint control=(prt==NULL)?v.tes+v.ata:v.tes+v.ata+v.def;
    acpprt=(prt==NULL)?TRUE:FALSE;
    if(control>0) {
        estudiado=e;
        objeto_t* oip[objsiz()];
        if(objfnd(oip,isoip)) {
            return *oip;
        }
    }
    return NULL;
}  

static double _iaarmclc(objeto_t* e,objeto_t* a) {
	/*da un valor del arma de ataque a partir de las caracteristicas */
	const double MD[]={0.0,2.5,3.5,4.5,6.5,10.5};
	double hab=e->hab+(a->pha==0)?0:(e->hab/a->pha);
	double fue=e->fue+(a->pfu==0)?0:(e->fue/a->pfu);
	double dad=MD[a->dad]*(double)a->nad;
	return hab+fue+dad;
}

/* funciones principales */

static Bool iaatc(objeto_t* e) {
/* gestiona el ataque de un enemigo si el jugador esta presente */
    int res=0; /* 0: nada, 1: acercarse 2: alejarse 3: atacar */
    if(hjug) {
        uint dje=objdis(e,jugador);
        if(e->anm) {
            if(dje==1) res=3;
            else res=1;
        } else {
            int atsim=_iaefeata(e);
            if(atsim>0 || nene>1) {
                if(dje==1) res=3;
                else res=1;
            } else {
                if(dje>1 || (dje==1 && e->vel>jugador->vel)) res=2;
                else res=3;
            }
        }
        switch(res) {
            case 1:
                return _iamovtojug(e);
            case 2:
                return _iamovnojug(e);
            case 3:
                return objata(e,jugador);
        }
    }
    return FALSE;
}

static Bool iacog(objeto_t* e) {
    /* mira si el enemigo puede coger algun objeto */
    objeto_t* oin[objsiz()];
    uint oins=objinv(e,oin);
    objeto_t* prt=objisprt(e);
    if(oins<e->cap) {
        objeto_t* ip=_iaobjip(e,prt);
        if(ip) {
            return objcog(e,ip);
        } else {
            int rf,cf;
            if(_iafndobj(e,prt,&rf,&cf)) {
                return _iamoveto(e,rf,cf);
            }
        }
    }
    return FALSE;
}

static Bool iavst(objeto_t* e) {
    /* busca si hay objeto para vestir mejor que el que tiene puesto */
    objeto_t* inv[objsiz()];
    uint invs=objinv(e,inv);
    double ppoav=0;
    double ppoad=0;
    objeto_t* poad=NULL;
    objeto_t* poav=NULL;
    int k;
    fr(k,invs) {
        objeto_t* oe=inv[k];
        if(oe->arm) {
            double pun=_iaarmclc(e,oe);
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

static Bool iawlk(objeto_t* e) {
    /* hace un random walk si puede */
     const int DRM[]=DRC;
     int rdir=rnd(0,DRCS-1);
     return objmov(e,DRM[rdir],DRM[DRCS-1-rdir]);
}

/* funciones publicas */

Bool iaanm(objeto_t* e) {
    visset(e);
    if(!iaatc(e)) {
        return iawlk(e);
    }
    return TRUE;
}

Bool iahum(objeto_t* e) {
    visset(e);
    if(!iaatc(e)) {
        if(!iacog(e)) {
            if(!iavst(e)) {
                return iawlk(e);
            }
        }
    }
    return TRUE;
}
         
    
    
        

    

                

    


    
    
