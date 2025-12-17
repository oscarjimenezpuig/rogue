/*
============================================================
  Fichero: curse.h
  Creado: 27-11-2025
  Ultima Modificacion: mié 17 dic 2025 14:24:53
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef CURSE_H
#define CURSE_H


//CONSTANTES

#define NONE 0 // valor generico para el 0

//palette
#define BRIGHT 1 //paleta de maximo brillante
#define MEDIUM 2 //paleta de brillo medio
#define LOW 3 //paleta de brillo bajo
#define GREYS 4 //paleta de grises

//atributos
#define BOLD 1 //caracter en resaltado
#define UNDERLINE 2 //caracter subrayado
#define REVERSE 4 //caracter inverso
#define BLINK 8 //caracter parpadeante
#define PROTECT 16 //caracter protegido
#define INVIS 32 //caracter invisible
#define DIM 64 //caracter en resaltado medio

//colors
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

//mode in
#define ECHO 1 //las teclas se reproducen en pantalla
#define DELAY 2 //se espera a la introduccion de teclas
#define ENTER 4// se espera al enter
#define CURSOR 8 //pone el cursor

#define INKEY NONE //mode generico para pulsacion de teclas
#define INPUT ECHO|DELAY|ENTER|CURSOR //modo generico para input tradicional

//METODOS

//VARIABLES
//variable cambiables
extern int ROW; //fila del cursor
extern int COL; //columna del cursor
extern int ATR; //atributo de impresion (banderas de atributos)
extern int INK; //tinta
extern int BKG; //fondo

//variables estaticas
extern int ROWS; //dimension en filas de la terminal
extern int COLS; //dimension en columnas de la terminal


//FUNCIONES

//principales

//secundarias (acceso mediante metodos)

int bufget(int len,char* str);
//se obtiene lo que hay en el buffer y se coloca en str

void cls();
//hace un cls de toda la pantalla en el fondo elegido, coloca los cursores en 0,0

void curse();
//funcion donde se alojara todo el programa que usa curses

int inkey(char chr);
//comprueba si el caracter c esta en el buffer (devuelve el numero de veces)

int listen(int modein_flags);
//escuchamos el teclado, introduciendo las banderas indicando como queremos que sea la escucha

void palette(int type);
//se entra un tipo de paleta 

void pause(double seconds);
//pausa de segundos

int posget(char* chr,int* atr,int* ink,int* bkg);
//da el caracter y atributos de la posicion marcada por los cursores, si no necesitamos un valor, ponemos NONE

void printc(char chr);
//impresion de un solo caracter

void prints(const char* str,...);
//impresion de un string

void randomize(int seed);
//introduce la semilla, si es negativa en funcion del tiempo

int rnd(int a,int b);
//numero aleatorio del intervalo cerrado a,b

void show();
//muestra el contenido de la pantalla


#endif /* CURSE_H */



