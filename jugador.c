/*
============================================================
  Fichero: jugador.c
  Creado: 05-12-2025
  Ultima Modificacion: dimarts, 6 de gener de 2026, 11:25:43
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
#define TESC 's' /* escalera 11 */
#define TATQ 'A' /* atacar 12 */

#define KEYN {TARR,TABJ,TIZQ,TDER,TCOG,TDEJ,TINV,TABR,TFRZ,TQUT,TMIR,TESC,TATQ};
#define KEYS 13

/* posicion inicial pantalla */
#define RO 0
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
	cls();
	INK=WHITE;
	ROW=COL=0;
	ATR=BOLD;
	prints("Cual es tu nombre? ");
	show();
	ATR=NONE;
	listen(INPUT);
	bufget(SLEN,jugador->nom);
}

static void carrndset(int points) {
	jugador->cap=1;
	--points;
	for(int k=0;k<points;k++) {
		int cn=rnd(1,CEE);
		switch(cn) {
			case 1:
				jugador->fue++;
				break;
			case 2:
				jugador->hab++;
				break;
			case 3:
				jugador->vel++;
				break;
			case 4:
				jugador->cap++;
				break;
		}
	}
}

static void carequset(int points) {
	jugador->fue=jugador->hab=jugador->vel=jugador->cap=points/CEE;
}

static void carownset(int points) {
	cls();
	char* str[]={"fuerza","habilidad","velocidad","capacidad"};
	for(int k=0;k<CEE && points;k++) {
		cls();
otra:
		ATR=BOLD;
		INK=WHITE;
		prints("Caracteristica %s",str[k]);
		ATR=NONE;
		ROW++;
		COL=0;
		prints("Quedan %i puntos, asignas: ",points);
		ATR=NONE;
		listen(INPUT);
		char resp[3];
		bufget(2,resp);
		ROW++;
		COL=0;
		prints("Has introducido: %s",resp);
		int value=0;
		sscanf(resp,"%i",&value);
		if(value<0 || value>MIN(points,VMC)) {
			COL=0;
			ROW++;
			INK=RED;
			prints("El valor de la caracteristica debe estar entre 0 i %i",MIN(points,VMC));
			goto otra;
		} else {
			switch(k) {
				case 0:
					jugador->fue=value;
					break;
				case 1:
					jugador->hab=value;
					break;
				case 2:
					jugador->vel=value;
					break;
				case 3:
					jugador->cap=value;
					break;
			}
			points-=value;
		}
	}
}

static Bool carres() {
	char* str[]={"FUE","HAB","VEL","CAP"};
	int val[]={jugador->fue,jugador->hab,jugador->vel,jugador->cap};
	cls();
	ATR=BOLD;
	prints("En resumen, las caracteristicas escogidas son: ");
	ROW++;
	COL=0;
	for(int k=0;k<CEE;k++) {
		ATR=BOLD;
		prints("%s: ",str[k]);
		ATR=NONE;
		prints("%i   ",val[k]);
	}
	COL=0;
	ROW+=2;
	ATR=BOLD;
	INK=YELLOW;
	prints("Pulsa 0 si NO estas de acuerdo");
	ATR=NONE;
	INK=WHITE;
	while(listen(INKEY)==0);
	if(inkey('0')) return FALSE;
	return TRUE;
}		

static void carset() {
	/* funcion que da las caracteristicas de jugador */
	const int NOPS=3;
	char* OPS[]={"Igualitaria","Aleatoria","Individualmente"};
	cls();
	do {
		cls();
		jugador->fue=jugador->hab=jugador->vel=jugador->cap=0;
		int res=menu("Escoge las caracteristicas de tu jugador",NOPS,OPS);
		if(res==0 || res>NOPS) carequset(CEE*PIE);
		else if(res==1) carrndset(CEE*PIE);
		else carownset(CEE*PIE);
	} while(!carres());
}

Bool jugnew() {
	objnew("",JATR,TRUE,TRUE);
	if(jugador) {
		nomset();
		carset();
		return TRUE;
	}
	return FALSE;
}

static Bool strfnd(int* er ,int* ec,int dir) {
	/* busca escalera en la localidad, si dir=1 escalera de subida, si dir=-1 escalera de bajada */
	for(int r=0;r<MAPAR;r++) {
		for(int c=0;c<MAPAC;c++) {
			localidad_t* l=mapget(r,c);
			if(l && l->esc==-dir) {
				*er=r;
				*ec=c;
				return TRUE;
			}
		}
	}
	return FALSE;
}

static Bool jugposini(int* r,int* c) {
	/* determina la posicion inicial del  jugador que tiene que estar en la habitacion dada por JHI */
	const int TRIES=10000;
	int tries=TRIES;
	while(tries--) {
		maprndpos(r,c,FALSE);
		localidad_t * l=mapget(*r,*c);
		if(l && l->hab==JHI) return TRUE;
	}
	return FALSE;
}

Bool jugpos(int dir) {
	if(jugador) {
		int r,c;
		r=c=-1;
		if(dir==0) jugposini(&r,&c);
		else strfnd(&r,&c,dir);
		if(r!=-1 && c!=-1) {
			if(objinipos(jugador,r,c)) {
				visset();
				return TRUE;
			}
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
	if(pe && CDP) {
		menin("Has descubierto una puerta oculta...");
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
		if(CFP) {
			menin("Has podido forzar la puerta...");
			pe->trs=1;
		} else menin("No has podido forzar la puerta...");
		return TRUE;
	} else menin("No veo cerca ninguna puerta para forzar...");
	return FALSE;
}

static Bool isitijp(objeto_t* o) {
	return (o && o->npc==0 && mapget(o->r,o->c) && o->r==jugador->r && o->c==jugador->c);
}

static Bool jugmir() {
	/* mira la posicion para examinar si hay algun objeto */
	objeto_t* itm[objsiz()];
	uint itms=objfnd(itm,isitijp);
	if(itms==1) menin("Aqui puedes ver %s...",(*itm)->nom);
	else if(itms>1) menin("Aqui puedes ver, a parte de %s, algunas cosas mas...",(*itm)->nom);
	else menin("No veo nada aqui...");
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
	menin("No hay nada aqui que puedas coger...");
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
		menin("No tienes nada para dejar...");
	}
	return FALSE;
}

static Bool juginv() {
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
			menin("Abres la puerta...");
			p->trs=1;
			if(CRL) {
				menin("... pero la llave se ha roto...");
				ll->con=NULL;
			}
			return TRUE;
		} else menin("No tienes una llave adecuada para esta puerta...");
	} else menin("No hay nada para abrir aqui cerca...");
	return FALSE;
}

static Bool jugues() {
	/* funcion de usar la escalera del jugador */
	localidad_t* l=mapget(jugador->r,jugador->c);
	int dir=(l && l->esc)?l->esc:0;
	if(dir) {
		if(dir==1) menin("Bajaras por la escalera...");
		else menin("Subiras por la escalera...");
		return nivchg(dir);
	} else menin("No veo aqui ninguna escalera...");
	return FALSE;
}

static Bool isnpcvec(objeto_t* o) {
	return (o->npc && o->vid>0 && objdis(o,jugador)==1);
}

static Bool jugata() {
	/* funcion de ataque de jugador */
	objeto_t* vcn[4];
	int vcns=objfnd(vcn,isnpcvec);
	if(vcns==1) {
		return objata(jugador,vcn[0]);
	} else if(vcns){
		char* enenom[vcns];
		for(int k=0;k<vcns;k++) {
			enenom[k]=vcn[k]->nom;
		}
		uint nene=menu("A quien atacas?",vcns,enenom);
		if(nene<vcns) return objata(jugador,vcn[nene]);
	} else menin("Nadie a quien puedas atacar aqui cerca...");
	return FALSE;
}

static Bool jugqut() {
	jugador=NULL;
	return TRUE;
}

Bool jugact() {
	if(objcanact(jugador) && !jugdsc()) {
listen:
		listen(DELAY);
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
			case 11:
				return jugues();
			case 12:
				return jugata();
			default:
				goto listen;
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

	
				




	
