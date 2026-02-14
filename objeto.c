/* ROGUE 9/12/25 */

#include "rogue.h"

#define OBJETOS 64 /* numero de objetos maximos en un nivel */

#define ATRCAD (atributo_t){'%',BOLD,WHITE,BLACK} /* atributo de un cadaver */
#define NOMCAD "CADAVER" /* nombre del cadaver */

objeto_t* jugador=NULL;
objeto_t* asesino=NULL;

static objeto_t objeto[OBJETOS];
static uint objetos=1;

void objclr() {
	objetos=1;
}

uint objsiz() {
	return objetos;
}

objeto_t* objnew(char* n,atributo_t a,Bool npc,Bool jug) {
	objeto_t* new=NULL;
	if(objetos<OBJETOS) {
		new=(npc && jug)?objeto:objeto+objetos++;
		char* nd=new->nom;
		char* no=n;
		while(*no!=EOS) {
			if(*no>='a' && *no<='z') *nd=*no-'a'+'A';
			else *nd=*no;
			nd++;
			no++;
		}
		*nd=EOS;
		new->atr=a;
		new->r=new->c=-1;
		if(npc) {
			new->npc=1;
			new->fan=0;
			new->anm=0;
            new->jug=0;
            new->dr=new->dc=0;
			if(jug) {
				jugador=new;
				new->jug=1;
			}
			new->fue=new->hab=new->vel=new->cap=new->cve=0;
			new->ata=0;
			new->oro=0;
			new->vid=VMC;
		} else {
			new->npc=0;
			new->cog=new->ior=new->arm=new->lla=new->ani=new->ves=0;
			new->con=NULL;
		}
	}
	return new;
}

objeto_t* objcpy(struct objeto_s objcar) {
	Bool inpc=objcar.npc;
	Bool ijug=(inpc)?objcar.jug:FALSE;
	objeto_t* o=objnew(objcar.nom,objcar.atr,inpc,ijug);
	if(o) *o=objcar;
	return o;
}

int objdis(objeto_t* a,objeto_t* b) {
	if(a && b) return mapdis(a->r,a->c,b->r,b->c);
	else return -1;
}

Bool objinipos(objeto_t* o,int r,int c) {
    Bool col=FALSE;
    if(o) {
        localidad_t* l=mapget(r,c);
        Bool loc=(l && l->trs==1);
        if(loc) {
            Bool ene=(o->npc && o->jug==0);
            Bool jug=(o->jug);
            Bool esc=(l->esc!=0); 
            col=!ene && !jug;
            if(!col) {
                col=!ene || !esc;
                if(col) {
                    objeto_t* po=objeto;
                    while(po!=objeto+objetos && col) {
                        if(po->npc && po->r==r && po->c==c) col=FALSE;
                        po++;
                    }
                }
            }
        }
        if(col) {
            o->r=r;
            o->c=c;
        }
    }
    return col;
}
				
uint objfnd(objeto_t* o[],Condicion c) {
	objeto_t* p=objeto;
	objeto_t** po=o;
	while(p!=objeto+objetos) {
		if(c(p)) {
			*po=p;
			po++;
		}
		p++;
	}
	return po-o;
}

static int onc=-1;
static int onr=-1;
static Bool isnpcinp(objeto_t* o) {
	/* condicion de busqueda de npc en una posicion */
	return (o->npc && o->r==onr && o->c==onc);
}

Bool objmov(objeto_t* o,int dr,int dc) {
	if(o && o->npc) {
		localidad_t* l=mapget(o->r,o->c);
		if(l) {
onr=o->r+dr;
			onc=o->c+dc;
			localidad_t* nl=mapget(onr,onc);
			if(nl && nl->trs==1) {
				objeto_t* npc[objetos];
				uint npcs=objfnd(npc,isnpcinp);
				if(!npcs) {
					o->r=onr;
					o->c=onc;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

static objeto_t* npce=NULL;
static Bool isinv(objeto_t* o) {
	/* objetos poseidos por npce */
	return (o->npc==0 && o->con==npce);
}

uint objinv(objeto_t* o,objeto_t* c[]) {
	if(o && o->npc) {
		npce=o;
		return objfnd(c,isinv);
	}
	return 0;
}

#define oij (o==jugador)

Bool objcog(objeto_t* o,objeto_t* itm) {
    if(o && o->npc && itm && !itm->npc && itm->cog && itm->r==o->r && itm->c==o->c && !itm->con) {
        objeto_t* inv[objetos];
        uint cinv=objinv(o,inv);
        if(cinv<o->cap || (o->jug && itm->ior)) {
            if(!itm->prt || !objisprt(o)) {
                itm->r=itm->c=-1;
                if(itm->ior) {
					if(oij) menin("Coges %i monedas de oro...",itm->cor);
					o->oro+=itm->cor;
				} else if(itm->prt) {
					if(oij) menin("Coges %s y te la pones...",itm->nom);
					itm->con=o;
				} else {
					if(oij) menin("Coges %s...",itm->nom);
					itm->con=o;
				}
				return TRUE;
            } else {
                if(oij) menin("No puedes coger una armadura, ya llevas una...");
			}
        } else {
            if(oij) menin("Llevas demasiadas cosas...");
        }
    } else {
        if(oij) menin("No puedes cogerlo...");
    }
    return FALSE;
}

Bool objdej(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		localidad_t* l=mapget(o->r,o->c);
		if(l && itm && itm->npc==0 && itm->con==o) {
			if(!itm->ves) {
				itm->con=NULL;
				itm->r=o->r;
				itm->c=o->c;
				if(oij) menin("Dejas %s...",itm->nom);
				return TRUE;
			} else {
				if(oij) menin("Debes quitartelo antes de dejarlo...");
			}
		}
	}
	if(oij) menin("No puedes dejarlo...");
	return FALSE;
}

objeto_t* objisprt(objeto_t* o) {
	objeto_t* inv[objsiz()];
	uint invs=objinv(o,inv);
	for(int k=0;k<invs;k++) {
		if(inv[k]->prt) return inv[k];
	}
	return NULL;
}

objeto_t* objisves(objeto_t* o) {
	objeto_t* inv[objsiz()];
	uint invs=objinv(o,inv);
	for(int k=0;k<invs;k++) {
		if(inv[k]->ves) return inv[k];
	}
	return NULL;
}

Bool objves(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		if(itm && !itm->npc && itm->con==o && itm->arm && !itm->ves) {
			if(!objisves(o)) {
				itm->ves=1;
				if(oij) menin("Te pones %s...",itm->nom);
				return TRUE;
			} else {
				if(oij) menin("No puedes llevar puesto nada mas...");
			}
		} else {
			if(oij) menin("No te puedes poner eso...");
		}
	}
	return FALSE;
}

Bool objdes(objeto_t* o,objeto_t* itm) {
	if(o && o->npc) {
		objeto_t* op=objisves(o);
		if(op==itm) {
			itm->ves=0;
			if(oij) menin("Te quitas %s...",itm->nom);
			return TRUE;
		} else if(oij) menin("No llevas eso puesto...");
	}
	return FALSE;
}

Bool objata(objeto_t* o,objeto_t* ene) {
	if(o && ene && o->npc && ene->npc && (o->jug || ene->jug)) {
		ene->ata=1;
		objeto_t* oata=NULL; //objeto de ataque
		objeto_t* odef=NULL; //armadura de defensa
		if(o->anm) {
			menin("%s muerde a %s...",o->nom,ene->nom);
		} else {
			oata=objisves(o);
			if(oata) menin("%s ataca a %s con %s...",o->nom,ene->nom,oata->nom);
			else menin("%s ataca a %s a mano descubierta...",o->nom,ene->nom);
		}
		if(HAT(o,ene)) {
			int dano=DAN(o);
			menin("%s da un golpe de %i puntos a %s...",o->nom,dano,ene->nom);
			jugshw(jugador);
			if(odef && DRP(odef)) {
				menin("A %s se le rompe %s...",ene->nom,odef->nom);
				itmrmp(odef);
			}
			if(dano>=ene->vid) {
                if(o==jugador) meminsmat(ene);
                else asesino=o;
                objmue(ene);
            }
			else ene->vid-=dano;
		} else {
			odef=objisprt(ene);
			if(odef) menin("%s lleva puesta %s, que le protege del ataque...",ene->nom,odef->nom);
			else menin("%s repele el ataque...",ene->nom);
			if(oata && DRA(oata)) {
				menin("A %s se le rompe %s...",o->nom,oata->nom);
				itmrmp(oata);
			}
			jugshw(jugador);
		}
		return TRUE;
	}
	return FALSE;
}

static void trscad(objeto_t* o) {
	/* transfoma un npc en un cadaver siempre y cuando no este en escalera (no al jugador) */
	if(o->jug==0) {
        localidad_t* l=mapget(o->r,o->c);
        if(l->esc==0) {
            char* on=NOMCAD;
            char* dn=o->nom;
            while(*on!=EOS) *dn++=*on++;
            *dn=EOS;
            o->atr=ATRCAD;
            o->npc=0;
            o->cog=o->ior=o->arm=o->lla=o->ani=o->prt=o->ves=0;
        } else {
            o->r=o->c=-1;
        }
	}
}

static Bool isooi(objeto_t* o) {
	/* condicion de objeto oro no disponible */
	return (o && o->npc==0 && o->ior && o->r==-1 && o->c==-1);
}

static void disalr(int or,int oc,int* r,int* c) {
    /* los objetos de los muertos se distribuyen alrededor del cadaver */
    const int MAXDIS=RCM;
    localidad_t* l=NULL;
    do {
        *r=or+rnd(-MAXDIS,MAXDIS);
        *c=oc+rnd(-MAXDIS,MAXDIS);
        l=mapget(*r,*c);
    }while(!l || l->trs!=1);
}

static void reporo(int r,int c,int oro) {
	if(oro>0) {
		objeto_t* tsd[objsiz()];
		int tsds=objfnd(tsd,isooi);
		int cout=0;
		while(cout<tsds && oro) {
			int coro=0;
			if(oro>OMO) coro=OMO;
			else coro=oro;
			oro-=coro;
			tsd[cout]->cor=coro;
            int pr,pc;
            disalr(r,c,&pr,&pc);
			tsd[cout]->r=pr;
			tsd[cout]->c=pc;
			cout++;
		}
	}
}

Bool objmue(objeto_t* o) {
	if(o && o->npc && o->vid>0 && o->r!=-1 && o->c!=-1) {
		localidad_t* l=mapget(o->r,o->c);
		if(l->vis==2) menin("%s ha muerto...",o->nom);
		o->vid=0;
		if(o->jug) {
			end_game=1;
		} else {
			objeto_t* inv[objsiz()];
			int invs=objinv(o,inv);
			for(int k=0;k<invs;k++) {
				objeto_t* oe=inv[k];
                oe->ves=0;
				oe->con=NULL;
                int r,c;
                disalr(o->r,o->c,&r,&c);
				oe->r=r;
				oe->c=c;
			}
			reporo(o->r,o->c,o->oro);
			trscad(o);
		}
		return TRUE;
	}
	return FALSE;
}

#undef oij

Bool objcanact(objeto_t* o) {
	if(o && o->npc && o->vid>0) {
		localidad_t* l=mapget(o->r,o->c);
		if(l) {
			if(o->cve==(VMC-o->vel)) {
				o->cve=0;
				return TRUE;
			} else {
				o->cve++;
			}
		}
	}
	return FALSE;
}

Bool objdsc(objeto_t* obj) {
	if(obj) {
		COL=0;
		prints("%s ",obj->nom);
		if(obj->npc && obj->jug==0) {
			if(obj->vid>0) {
				if(obj->anm) prints("(animal)");
				prints("F:%i H:%i V:%i",obj->fue,obj->hab,obj->vel);
				objeto_t* aa=objisves(obj);
				if(aa) {
					COL=2;
					ROW++;
					prints("Arma de ataque: ");
					objdsc(aa);
				}
				objeto_t* dd=objisprt(obj);
				if(dd) {
					COL=2;
					ROW++;
					prints("Arma de defensa: ");
					objdsc(dd);
				}
			} else prints("seco como la mojama...");
		} else if(obj->npc==0) {
			if(obj->ior) {
				prints("contiene %i monedas de oro...",obj->cor);
			} else if(obj->lla) {
				prints("para abrir las puertas...");
			} else if(obj->arm) {
				prints("arma de ataque D:%iD",obj->nad);
				int td=obj->dad;
				prints("%i ",(td==1)?4:(td==2)?6:(td==3)?8:(td==4)?12:20);
				prints("F:+1c%i H:+1c%i",obj->pfu,obj->pha);
			} else if(obj->prt) {
				prints("arma de defensa E:+%i",obj->nar);
			} else if(obj->ani) {
				prints("EL ANILLO UNICO!!!");
			}
		}
		return TRUE;
	}
	return FALSE;
}


	






	
