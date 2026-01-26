/* ROGUE 27/11/25 */

#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

#include "curse.h"

//CONSTANTES
#define BUFSIZE 1024

//MACROS
#define TERMDIM getmaxyx(stdscr,ROWS,COLS) //actualiza las dimensiones de filas y columnas del terminal
#define CPAIR(I,B) (1+(I)+(B)*8) //da el par del color actual
#define LOCATE move(ROW,COL) //coloca el cursor ncurses en la posicion ROW,COL
#define INTERM (ROW>=0 && ROW<ROWS && COL>=0 && COL<COLS) //dice si las filas y columnas estan en la pantalla

//VARIABLES

//externas

int ROW,COL,ATR,INK,BKG,ROWS,COLS;

//estaticas
static char _buffer[BUFSIZE];
static bool _ended=true;

//FUNCIONES

//privadas

static void __atributos() {
	//conecta los atributos y el color, desconectando los previos
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int FCL=7;
	static int atr=0;
	static short clp=0;
	int atrn=0;
	for(int k=0;k<FCL;k++) {
		if(ATR & INT[k]) atrn|=FLG[k];
	};
	short colpair=CPAIR(INK,BKG);
	if(atrn!=atr || clp!=colpair) {
		attroff(atr);
		attron(atrn | COLOR_PAIR(colpair));
		atr=atrn;
		clp=colpair;
	}
}

static void __end() {
	//final del ncurses
	if(!_ended) {
		endwin();
		_ended=true;
		printf(" and finished\n");//dbg
	}
}

static void __hand_signal(int sig) {
	//final en caso de error
	__end();
	exit(sig);
}

static void __init_pairs() {
	for(int t=BLACK;t<=WHITE;t++) {
		for(int f=BLACK;f<=WHITE;f++) {
			init_pair(CPAIR(t,f),t,f);
		}
	}
}

static void _modein(int f) {
	//determina el modo de entrada del curses
	if((f & CURSOR)) curs_set(1);
	else curs_set(0);
	if((f & ECHO)) echo();
	else noecho();
	nodelay(stdscr,(f & DELAY)?FALSE:TRUE);
	if((f & ENTER)) noraw();
	else raw();
}

static bool _prtchr(char c) {
	if(INTERM) {
		LOCATE;
		addch(c);
		COL++;
		return true;
	}
	return false;
}

static void _init() {
	//inicio del ncurses
	signal(SIGINT,__hand_signal);
	signal(SIGSEGV,__hand_signal);
	initscr();
	noecho();
	raw();
	nodelay(stdscr,TRUE);
	curs_set(0);
	keypad(stdscr,TRUE);
	start_color();
	palette(BRIGHT);
	__init_pairs();
	atexit(__end);
	_ended=false;
	ROW=COL=0;
	LOCATE;
	TERMDIM;
	INK=BKG=BLACK;
	__atributos();
	printf("CURSES inited");
}

//publicas

int bufget(int l,char* s) {
	l=(l>BUFSIZE-1)?BUFSIZE-1:l;
	char* pb=_buffer;
	char* ps=s;
	while(ps-s<l && *pb!='\0') {
		*ps++=*pb++;
	}
	*ps='\0';
	return ps-s;
}

void cls() {
	TERMDIM;
	__atributos();
	for(ROW=0;ROW<ROWS;ROW++) {
		for(COL=0;COL<COLS;COL++) {
			LOCATE;
			addch(' ');
		}
	}
	ROW=COL=0;
}	

int inkey(char c) {
	int count=0;
	char* p=_buffer;
	while(*p!='\0') {
		if(*p++==c) ++count;
	}
	return count;
}

int listen(int modein) {
	static int modeflag=0;
	if(modeflag!=modein) {
		_modein(modein);
		modeflag=modein;
	}
	LOCATE;
	__atributos();
	char* p=_buffer;
	char c=0;
	bool entmod=(modeflag & ENTER)?true:false;
	do {
		c=getch();
		if(c!=ERR) {
			if(c=='\n' && entmod) entmod=false;
			else {
				*p++=c;
			}
		}
	} while(entmod);
	*p='\0';
	return p-_buffer;
}

#define hascmp(A,D) ((((A)&(D))==(D))?1:0)

void palette(int t) {
	const short COLS[]={COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW,COLOR_BLUE,COLOR_MAGENTA,COLOR_CYAN,COLOR_WHITE};
	const short DIF=142;
	short red,green,blue;
	short value=1000;
	if(t==MEDIUM) value=600;
	else if(t==LOW) value=200;
	for(int col=BLACK;col<=WHITE;col++) {
		if(t==GREYS) {
			red=green=blue=DIF*col;
		} else { 
			red=green=blue=0;
			if(hascmp(col,RED)) red=value;
			if(hascmp(col,GREEN)) green=value;
			if(hascmp(col,BLUE)) blue=value;
		}
		init_color(COLS[col],red,green,blue);
	}
}

#undef hascmp

void pause(double s) {
	clock_t limit=clock()+s*CLOCKS_PER_SEC;
	while(clock()<limit);
}

int posget(char* c,int* a,int* i,int* b) {
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int FCL=7;
	if(INTERM) {
		LOCATE;
		chtype cht=inch();
		if(c) *c=(cht & A_CHARTEXT);
		if(a) {
			int atra=(cht & A_ATTRIBUTES);
			*a=0;
			for(int k=0;k<FCL;k++) {
				if(atra & FLG[k]) *a|=INT[k];
			}
		}
		short pc=(cht & A_COLOR);
		if(i) *i=(pc-1)%8;
		if(b) *b=(pc-1)/8;
		return 1;
	}
	return 0;
}

void printc(char ch) {
	prints("%c",ch);
}

void prints(const char* s,...) {
	va_list list;
	va_start(list,s);
	vsprintf(_buffer,s,list);
	va_end(list);
	char* ptr=_buffer;
	LOCATE;
	__atributos();
	while(*ptr!='\0' && _prtchr(*ptr++));
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
	TERMDIM;
	refresh();
}

int main() {
	_init();
	curse();
}
