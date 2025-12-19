/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: vie 19 dic 2025 12:14:43
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

/* atributos */
#define JATR (atributo_t){'@',BOLD,WHITE,BLACK}

/* teclado */
#define TARR 'i' /* arriba 0*/
#define TABJ 'k' /* abajo 1*/
#define TIZQ 'j' /* izquierda 2*/
#define TDER 'l' /* derecha 3*/
#define TCOG 'c' /* coger 4*/
#define TDEJ 'd' /* dejar 5*/
#define TINV 'I' /* inventario 6*/
#define TABR 'a' /* abrir una puerta 7*/
#define TFRZ 'f' /* forzar la puerta 8*/
#define TQUT 'Q' /* tecla finalizar juego 9*/
#define TMIR 'm' /* mirar 10 */

#define KEYN {TARR,TABJ,TIZQ,TDER,TCOG,TDEJ,TINV,TABR,TFRZ,TQUT,TMIR};
#define KEYS 11

/* posicion inicial pantalla */
#define RO 1
#define CO 0

static void recjug(int* ri,int* ci,int* rs,int* cs) { 
	/* rectangulo de visibilidad donde el centro es la posicion del jugador */
	int jr=jugador->r;
	int jc=jugador->c;
	*rs=ROWS-3;
	*ri=jr-*rs/2;
	*cs=COLS;
	*ci=jc-*cs/2;
}	

static void visset() {
	/* determinamos la visibilidad del jugador */
	int ri,ci,rs,cs;
	recjug(&ri,&ci,&rs,&cs);
	for(int r=0;r<rs;r++) {
		for(int c=0;c<cs;c++) {
			localidad_t* l=mapget(r+ri,c+ci);
			if(l) l->vis=(l->vis)?1:0;
		}
	}
	localidad_t* lj=mapget(jugador->r,jugador->c);
	if(lj && lj->trs==1) {
		lj->vis=2;
		int hab=lj->hab;
		if(lj->osc==0) {
			for(int r=0;r<rs;r++) {
				for(int c=0;c<cs;c++) {
					localidad_t* l=mapget(r+ri,c+ci);
					if(l && l->hab==hab) l->vis=2;
				}
			}
		} else {
			localidad_t* n[4];
			mapngh(jugador->r,jugador->c,n);
			for(int k=0;k<4;k++) {
				if(n[k]) n[k]->vis=2;
			}
		}
	}
}

static void nomset() {
	/*funcion que escoge el nombre del jugador */
	INK=WHITE;
	ROW=COL=0;
	ATR=BOLD;
	prints("Cual es tu nombre? ");
	ATR=NONE;
	listen(INPUT);
	bufget(SLEN,jugador->nom);
}

static void carset() {
	/* funcion que da las caracteristicas de jugador */
	jugador->fue=4;
	jugador->hab=5;
	jugador->cve=jugador->vel=3;
	jugador->cap=3;
}

static Bool jugpos() {
/* funcion que da la posicion del jugador */
	int r,c;
	if(maprndpos(&r,&c,FALSE)) {
		jugador->r=r;
		jugador->c=c;
		localidad_t* l=mapget(r,c);
		return (l!=NULL)?TRUE:FALSE;
	}
	return FALSE;
}

Bool jugnew() {
	objnew("",JATR,TRUE,TRUE);
	if(jugador) {
		nomset();
		carset();
		if(jugpos()) {
			visset();
			return TRUE;
		}
	}
	return FALSE;
}

static int chkkey() {
	/* funcion que detecta el caracter que se pulsa y lo pasa a un numero */
	const char KECH[]=KEYN;
	for(int k=0;k<KEYS;k++) {
		if(inkey(KECH[k])) return k;
	}
	return -1;
}

static Bool jugmov(int ckey) {
	/* funcion que mueve el jugador si se puede */
	const int DR[]={-1,1,0,0};
	const int DC[]={0,0,-1,1};
	int dr=DR[ckey];
	int dc=DC[ckey];
	if(objmov(jugador,dr,dc)) {
		visset();
		return TRUE;
	}
	return FALSE;
}

static Bool jugdsc() {
	/* busca las celdas vecinas y descubre las puertas ocultas */
	localidad_t* n[4];
	mapngh(jugador->r,jugador->c,n);
	localidad_t* pe=NULL;
	for(int k=0;k<4 && !pe;k++) {
		pe=n[k];
		if(pe && (pe->obs==1 || pe->trs!=3)) pe=NULL;
	}
	if(pe && rnd(HmO+nivel,VMC)<jugador->hab) {
		mensaje("Has descubierto una puerta oculta...");
		pe->trs=1;
		return TRUE;
	}
	return FALSE;
}

static Bool jugfrp() {
	/* accion de forzar la puerta de un jugador */
	localidad_t* n[4];
	mapngh(jugador->r,jugador->c,n);
	localidad_t* pe=NULL;
	for(int k=0;k<4 && !pe;k++) {
		pe=n[k];
		if(pe && (pe->trs!=2)) pe=NULL;
	}
	if(pe) {
		if(rnd(HmF+nivel,VMC)<jugador->hab && rnd(FmF+nivel,VMC)<jugador->fue) {
			mensaje("Has podido forzar la puerta...");
			pe->trs=1;
		} else mensaje("No has podido forzar la puerta...");
		return TRUE;
	} else mensaje("No veo cerca ninguna puerta para forzar...");
	return FALSE;
}

static Bool isitijp(objeto_t* o) {
	return (o && o->npc==0 && mapget(o->r,o->c) && o->r==jugador->r && o->c==jugador->c);
}

static Bool jugmir() {
	/* mira la posicion para examinar si hay algun objeto */
	objeto_t* itm[objsiz()];
	uint itms=objfnd(itm,isitijp);
	if(itms==1) mensaje("Aqui puedes ver %s...",(*itm)->nom);
	else if(itms>1) mensaje("Aqui puedes ver, a parte de %s, algunas cosas mas...",(*itm)->nom);
	else mensaje("No veo nada aqui...");
	return (itms>0)?TRUE:FALSE;
}

static void jugitc() {
	/* dice todos los items que estan bajo el jugador */
	objeto_t* itm[objsiz()];
	uint itms=objfnd(itm,isitijp);
	static int conected=0;
	if(itms) {
		INK=WHITE;
		ATR=NONE;
		ROW=MAPAR-1;
		COL=0;
		prints("Aqui puedes ver %s",(*itm)->nom);
		if(itms>1) prints(" y algunas cosas mas");
		prints("...");
		conected=1;
	} else if(conected) {
		ROW=MAPAR-1;
		for(COL=0;COL<MAPAC;COL++) printc(' ');
		conected=0;
	}
}

static Bool jugcog() {
	objeto_t* itm[objsiz()];
	uint itms=objfnd(itm,isitijp);
	if(itms) {
		objeto_t* cog=*itm;
		if(itms>1) {
			char* itno[itms];
			for(int k=0;k<itms;k++) {
				itno[k]=itm[k]->nom;
			}
			uint ne=menu("Que quieres coger?",itms,itno);
			if(ne<itms) cog=itm[ne];
			else cog=NULL;
		}
		if(cog) return objcog(jugador,cog);
	}
	mensaje("No hay nada aqui que puedas coger...");
	return FALSE;
}

static Bool jugdej() {
	objeto_t* inv[objsiz()];
	uint invs=objinv(jugador,inv);
	if(invs) {
		char* inno[invs];
		for(int k=0;k<invs;k++) {
			inno[k]=inv[k]->nom;
		}
		uint ne=menu("Que quieres dejar?",invs,inno);
		if(ne<invs) return objdej(jugador,inv[ne]);
	} else {
		mensaje("No tienes nada para dejar...");
	}
	return FALSE;
}

static Bool juginv() {
	cls();
	INK=WHITE;
	ATR=BOLD;
	ROW=COL=0;
	prints("INVENTARIO");
	ATR=NONE;
	COL=0;
	ROW++;
	objeto_t* inv[objsiz()];
	uint invs=objinv(jugador,inv);
	uint resto=jugador->cap-invs;
	if(invs) {
		for(int k=0;k<invs;k++) {
			prints(" -%s",inv[k]->nom);
			ROW++;
			COL=0;
		}
	} else {
		prints("No llevas nada...");
		ROW++;
		COL=0;
	}
	if(resto==0) prints("No puedes llevar nada mas...");
	else prints("Todavia puedes llevar %i objetos mas...",resto);
	while(listen(INKEY)==0);
	cls();
	return TRUE;
}

static Bool jugabr() {
	localidad_t* n[4];
	mapngh(jugador->r,jugador->c,n);
	localidad_t* p=NULL;
	for(int k=0;k<4 && !p;k++) {
		if(n[k] && n[k]->trs==2) p=n[k];
	}
	if(p) {
		objeto_t* inv[objsiz()];
		uint invs=objinv(jugador,inv);
		objeto_t* ll=NULL;
		for(int k=0;k<invs && !ll;k++) {
			if(inv[k]->lla) ll=inv[k];
		}
		if(ll) {
			mensaje("Abres la puerta...");
			p->trs=1;
			if(rnd(0,PLR)==0) {
				mensaje("... pero la llave se ha roto...");
				ll->con=NULL;
			}
			return TRUE;
		} else mensaje("No tienes una llave adecuada para esta puerta...");
	} else mensaje("No hay nada para abrir aqui cerca...");
	return FALSE;
}

static Bool jugqut() {
	jugador=NULL;
	return TRUE;
}

Bool jugact() {
	if(objcanact(jugador) && !jugdsc()) {
		listen(INKEY);
		int ckey=chkkey();
		switch(ckey) {
			case 0:
			case 1:
			case 2:
			case 3:
				return jugmov(ckey);
			case 4:
				return jugcog();
			case 5:
				return jugdej();
			case 6:
				return juginv();
			case 7:
				return jugabr();
			case 8:
				return jugfrp();
			case 9:
				return jugqut();
			case 10:
				return jugmir();
		}
	}
	return FALSE;
}

Bool jugshw() {
	localidad_t* l=NULL;
	if(jugador && (l=mapget(jugador->r,jugador->c))) {
		int ri,ci,rs,cs;
		jugitc();
		recjug(&ri,&ci,&rs,&cs);
		panshw(ri,ci,rs,cs,RO,CO);
		return TRUE;
	}
	return FALSE;
}

	
				




	
