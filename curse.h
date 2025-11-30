/*
============================================================
  Fichero: curse.h
  Creado: 27-11-2025
  Ultima Modificacion: diumenge, 30 de novembre de 2025, 12:48:18
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef CURSE_H
#define CURSE_H

#include <stdarg.h>

//CONSTANTES

//atributos
#define BOLD 1 //caracter en resaltado
#define UNDERLINE 2 //caracter subrayado
#define REVERSE 4 //caracter inverso
#define BLINK 8 //caracter parpadeante
#define PROTECT 16 //caracter protegido
#define INVIS 32 //caracter invisible
#define DIM 64 //caracter en resaltado medio

//palette
#define BRIGHT 1 //paleta de maximo brillante
#define MEDIUM 2 //paleta de brillo medio
#define LOW 3 //paleta de brillo bajo
#define GREYS 4 //paleta de grises

//colors
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

//on/off
#define OFF 0
#define ON 1

//mode in
#define NORMAL 0 //En este caso hay ECHO,se espera a la introduccion de las teclas y espera al enter
#define NOECHO 1 //las teclas no se reproducen en pantalla
#define NODELAY 2 //no se espera a la introduccion de teclas
#define NOENTER 4//no espera al enter
#define NOCURSOR 8 //quita el cursor

//TIPOS
typedef unsigned char u1;

//VARIABLES

//FUNCIONES

void at(int r,int c);
//situa el cursor en la posicion especificada

void atget(int* r,int* c);
//da la posicion actual del cursor

void attr(u1 status,u1 attributes);
//define los atributos, status es on o off

void background(u1 color);
//establecer color del fondo

u1 chrchk(char* in,char c);
//comprueba que el caracter c esta en la entrada in (devuelve el numero de veces)

char chrget();
//da el caracter que se encuentra en la posicion señalada por los cursores

void cls();
//borra toda la pantalla, se substituye por espacios con el fondo especificado. Hace refresh

void colget(u1* ink,u1* background);
//devuelve el color de una determinada zona de la pantalla

void dimget(int* rows,int* cols);
//consigue la dimension del terminal

void inmode(u1 flags);
//define el inmode poniendo las flags necesarias (NOECHO,NODELAY,NORAW)

void ink(u1 color);
//establecer color de la tinta

u1 inkey(char c);
//comprueba si el caracter c esta en el buffer (devuelve el numero de veces)

u1 listen();
//escucha de las teclas y las guarda en el buffer siempre que la salida no sea error

void palette(u1 number);
//define la paleta de colores

void pause(double seconds);
//pausa de segundos

void printc(char c);
//imprime un caracter

void prints(const char* s,...);
//imprime un string (tipo printf)

void randomize(int seed);
//introduce la semilla, si es negativa en funcion del tiempo

int rnd(int a,int b);
//numero aleatorio del intervalo cerrado a,b

void show();
//hace el refresh

u1 strbuf(u1 len,char* str);
//copia la cadena del buffer en str

void begin();
//condiciones iniciales que establecemos

#endif /* CURSE_H */



