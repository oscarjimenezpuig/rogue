/*
============================================================
  Fichero: curse.c
  Creado: 27-11-2025
  Ultima Modificacion: diumenge, 30 de novembre de 2025, 20:27:34
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "curse.h"

//constantes privadas

#define BUFSIZE 256

//tipos privados

//variables estaticas

static int _rows=0; //numero de filas de la terminal
static int _columns=0; //numero de columnas de la terminal
static int _cursor_r=0; //row del cursor
static int _cursor_c=0; //column del cursor
static int _atrflg=0; //bandera de atributos
static u1 _ink=BLACK; //tinta
static u1 _background=BLACK; //fondo
static struct {
	u1 cur : 1;
	u1 col : 1;
	u1 atr : 1;
	u1 end : 1;
	u1 min : 4;
} _flag={0,0,0,1,NOECHO|NOCURSOR|NOENTER|NODELAY}; 
//banderas:
//cur: cursor, col: color, atr: atributo -> indican si hay cambio
//end: indica si ha sido finalizado el curses
//min: guarda las banderas del mode in
static char _buffer[BUFSIZE]={'\0'};

//privadas

static void __end() {
	if(_flag.end==0) {
		endwin();
		_flag.end=1;
	}
	puts("...and ended"); //dbg
}

static void _attron() {
	attron(_atrflg | COLOR_PAIR(_ink+8*_background));
}

#define CDD 8

static void _color() {
	static u1 coldefs[CDD];
	static u1 inited=0;
	if(!inited) {
		u1* p=coldefs;
		while(p!=coldefs+CDD) *p++=0;
		inited=1;
	}
	if(_ink!=BLACK) {
		u1 fi=1<<_ink;
		if((coldefs[_background] & fi)==0) {
			init_pair(_ink+8*_background,_ink,_background);
			coldefs[_background]|=fi;
		}
	}
}

#undef CDD

static void _init() {
	puts("NCURSES started...");//dbg
	initscr();
	noecho();
	raw();
	nodelay(stdscr,TRUE);
	curs_set(0);
	keypad(stdscr,TRUE);
	start_color();
	for(u1 k=BLACK;k<=WHITE;k++) init_pair(k*8,BLACK,k);
	attron(COLOR_PAIR(BLACK+8*BLACK));
	atexit(__end);
	getmaxyx(stdscr,_rows,_columns);
	_flag.end=0;
}

static chtype _chkpos() {
	//da las caracteristicas de una posicion de la pantalla
	return mvinch(_cursor_r,_cursor_c);
}

//publicas

void at(int r,int c) {
	if(_cursor_r!=r || _cursor_c!=c) {
		_cursor_r=r;
		_cursor_c=c;
		_flag.cur=1;
	}
}

void atget(int* r,int* c) {
	*r=_cursor_r;
	*c=_cursor_c;
}

void attr(u1 t,u1 a) {
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int SIZ=7;
	int flag=0;
	for(u1 f=0;f<SIZ;f++) if(a & INT[f]) flag|=FLG[f];
	if(flag) {
		if(t==ON) _atrflg|=flag;
		else _atrflg&=~(flag);
		_flag.atr=1;
	}
}

u1 attrget() {
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int SIZ=7;
	chtype ch=_chkpos();
	int attrs=ch & A_ATTRIBUTES;
	u1 flag=0;
	for(u1 k=0;k<SIZ;k++) {
		if(attrs & FLG[k]) flag|=INT[k];
	}
	return flag;
}

void background(u1 c) {
	if(_background!=c) {
		_background=c;
		_flag.atr=_flag.col=1;
	}
}

char chrget() {
	chtype ch=_chkpos();
	return ch & A_CHARTEXT;
}

void cls() {
	at(0,0);
	do {
		printc(' ');
		if(_cursor_c==_columns) {
			_cursor_r++;
			_cursor_c=0;
		}
	}while(_cursor_r<_rows);
	_cursor_r=_cursor_c=0;
}

void colget(u1* i,u1* b) {
	chtype ch=_chkpos();
	int pair=PAIR_NUMBER(ch);
	*i=pair%8;
	*b=pair/8;
}

void dimget(int* r,int* c) {
	*r=_rows;
	*c=_columns;
}


void inmode(u1 flag) {
	_flag.min=flag;
}

void ink(u1 c) {
	if(_ink!=c) {
		_ink=c;
		_flag.atr=_flag.col=1;
	}
}

u1 inkey(char c) {
	u1 count=0;
	char* p=_buffer;
	while(*p!='\0') {
		if(*p++==c) ++count;
	}
	return count;
}

#define nfic(A) ((_flag.min & (A))==0)

u1 listen() {
	if(nfic(NOCURSOR)) curs_set(1);
	char c=ERR;	
	char* p=_buffer;
	u1 end=(nfic(NOENTER) || nfic(NODELAY))?0:1;
	do {
		c=getch();
		if(c!=ERR) {
			*p++=c;
			end=(nfic(NOENTER))?0:1;
			if(c=='\n') end=1;
			else if(nfic(NOECHO)) printc(c);
			if(nfic(NOECHO) && c!='\n') printc(c);
		}
	}while(!end);
	*p='\0';
	if(nfic(NOCURSOR)) curs_set(0);
	return p-_buffer;
}

#undef nfic

void palette(u1 n) {
	const int COLS[]={COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW,COLOR_BLUE,COLOR_MAGENTA,COLOR_CYAN,COLOR_WHITE};
	const int DIF=125;
	if(n!=GREYS) {
		int value=1000;
		if(n==MEDIUM) value=600;
		else if(n==LOW) value=200;
		init_color(COLOR_BLACK,0,0,0);
		init_color(COLOR_RED,value,0,0);
		init_color(COLOR_GREEN,0,value,0);
		init_color(COLOR_YELLOW,value,value,0);
		init_color(COLOR_BLUE,0,0,value);
		init_color(COLOR_MAGENTA,value,0,value);
		init_color(COLOR_CYAN,0,value,value);
		init_color(COLOR_WHITE,value,value,value);
	} else for(int k=0;k<8;k++) init_color(COLS[k],DIF*k,DIF*k,DIF*k);
}

void pause(double s) {
	clock_t limit=clock()+s*CLOCKS_PER_SEC;
	while(clock()<limit);
}

void printc(char c) {
	if(_flag.cur) {
		move(_cursor_r,_cursor_c);
		_flag.cur=0;
	}
	if(_flag.atr) {
		if(_flag.col) {
			_color();
			_flag.col=0;
		}
		_attron();
		_flag.atr=0;
	}
	addch(c);
	_cursor_c++;
	move(_cursor_r,_cursor_c);
}

void prints(const char* s,...) {
	char str[1024];
	va_list list;
	va_start(list,s);
	vsprintf(str,s,list);
	va_end(list);
	char* c=str;
	while(*c!='\0') printc(*c++);
}

void randomize(int s) {
	unsigned int ss=(s<0)?time(NULL):s;
	srand(ss);
}

int rnd(int a,int b) {
	int max=(a>b)?a:b;
	int min=(a<b)?a:b;
	int dif=max-min+1;
	return min+(rand()%dif);
}

void show() {
	int ar=_rows;
	int ac=_columns;
	getmaxyx(stdscr,_rows,_columns);
	if(ar!=_rows || ac!=_columns) cls();
	else refresh();
}

u1 strbuf(u1 l,char* s) {
	char* pb=_buffer;
	char* ps=s;
	while(*pb!='\0' && ps-s<l) {
		*ps++=*pb++;
	}
	*ps='\0';
	return ps-s;
}

int main() {
	_init();
	begin();
	return 0;
}


