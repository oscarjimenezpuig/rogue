/*
============================================================
  Fichero: rogue.h
  Creado: 30-11-2025
  Ultima Modificacion: jue 15 ene 2026 11:22:09
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef ROGUE_H
#define ROGUE_H

/* INCLUDES */

#include <stdio.h>

#include "curse.h"

/* CONSTANTES */

/* generales */
#define EOS '\0' /* final de string */
#define SLEN 25 /* longitud de los string definidos */

/* mapa */
#define MAPAC 159 /* columnas totales del mapa */
#define MAPAR 48 /* filas totales del mapa */
#define NHMI 3 /* numero de habitaciones minimas */
#define NHMA 7 /* numero de habitaciones maximas */

/* definicion de booleanos */
#define TRUE 1
#define FALSE 0

/* dados */
#define DAT (rnd(1,4)) /* dado de 4 caras */
#define DAC (rnd(1,6)) /* dado de 6 caras */
#define DAO (rnd(1,8)) /* dado de 8 caras */
#define DAD (rnd(1,12)) /* dado de 12 caras */
#define DAI (rnd(1,20)) /* dado de 20 caras */

/* nivel */
#define NIN 1 /* nivel inicial */
#define NFI 15 /* nivel final */

/* items */
#define VMC 15 /* valor maximo de cualquier caracteristica */
#define OMO 65535 /* oro maximo que puede contener un objeto */
#define OXD 1 /* oro por cada 10 cuadrados de transitable */
#define OML 9 /* maximo de sacos de oro por nivel */
#define OmL 1 /* minimo de sacos por nivel */
#define PLR 4 /* factor de las llaves (relacionado con el dado que se usa para que se rompa la llave) */
#define ANT 4 /* numeros de tipos de armas */
#define ATI 5 /* numero de armas de cada tipo */
#define MAL 4 /* multiplicador al numero de armas ligeras */
#define MAE 3 /* multiplicador al numero de armas equilibradas */
#define MAP 2 /* multiplicador al numero de armas pesadas */
#define MAM 1 /* multiplicador al numero de armas magicas */
#define ATO (ATI*(MAL+MAE+MAP+MAM)) /* numero de armas totales */
#define APN (ATO/(NFI-NIN+1)) /* numero de armas medio por nivel */
#define ADM 2 /* desviacion de la media del numero de armas por nivel */
#define ADm -2 /* desviacion de la media minima */
#define NTP 20 /* numero de tipos de armas de proteccion */
#define MPB 4 /* multiplicadores de proteccion bajo */
#define MPR 3 /* multiplicador de proteccion medio */
#define MPA 2 /* multiplicador de proteccion alto */
#define MPM 1 /* multiplicador de proteccion magica */
#define PPN APN /* numero de protecciones por nivel */
#define PDM 2 /* desviacion maxima del numero de protecciones */
#define PDm -2 /* desviacion minima del numero de protecciones */

/* enemigos */
#define CEE 4 /* numero de caracteristicas evaluables de enemigo */
#define PIE 3 /* puntos iniciales de enemigo por caracteristica */
#define PFE VMC /* puntos finales de enemigo por caracteristica */
#define RZS 30 /* numero maximo de razas de enemigos */
#define RNm -2 /* diferencia minima por nivel aceptada */
#define RNM 2 /* diferencia maxima por nivel aceptada */
#define EmH 1 /* numero minimo de enemigos por nivel por habitacion */
#define EMH 3 /* numero maximo de enemigos por nivel por habitacion */
#define ATS 100 /* ataques simulados */
#define CDR DAI /* dado que da el cambio de direccion random */

/* jugador */
#define JHI 1 /* habitacion inicial del jugador */

/* MACROS */

#define MAX(A,B) (((A)>(B))?(A):(B))
#define MIN(A,B) (((A)>(B))?(B):(A))
#define ABS(A) (((A)>0)?(A):(-(A)))
#define SGN(A,B) ((A)==(B))?0:((A)>(B))?1:-1;

/* REGLAS */
#define CFP (DAI<=jugador->hab) && (DAI<=jugador->fue) /* condicion de jugador para forzar puerta */
#define CDP (DAO<=jugador->hab) /* condicion para descubrir una puerta */
#define CRL (DAT==1) /* condicion para que se rompa la llave */
#define PFA 5 /* puntos extras de fuerza por fuerza, solo para decidir el ataque */
#define PHA 15 /* puntos de habilidad extras si peleamos con el puño */
#define PFU 15 /* puntos de fuerza extras si peleamos con el puño */
#define PDA 1 /* dado de daño (1: DAT, 2: DAC, 3: DAO, 4: DAD, 5: DAI) */
#define NDA 1 /* numero de dados de daño de puñetazo */
#define CAR 5 /* puntos de armadura por cada punto de fuerza (siempre se cuentan) */
#define HAT(A,D) regla_ataque((A),(D)) /* condicion de ataque fructifero A: atacante, D: defensor */
#define DAN(A) regla_dano(A) /* daño que inflinge un atacante */
#define PIC 25 /* precio inicial de venta de una caracteristica */
#define PAC 10 /* aumento de precio de la caracteristica cada vez que se realiza una compra */
#define VmE num_nivel /* la vidad minima posible de cualquier enemigo es el nivel en el que se encuentra */
#define VME VMC /* la vida maxima de un enemigo es el valor maximo de la caracteristica */
#define DRA(A) regla_arma_rotura(A) /* regla para la rotura de armas */
#define DRP(P) regla_proteccion_rotura(P) /* regla para la rotura de la proteccion */
#define RDS regla_descanso() /* regla de descanso del jugador */

/* Reglas lucha: Se lanza 1D20 +(fuerza_ataque/PFA) , si este supera
 * a la defensa (fuerza/CAR) + armadura, el ataque es positivo. Entonces se
 * usa el arma con los dados de daño mas los plus de habilidad y fuerza, en caso de no
 * tener arma se usan los puños */
/* Regla de descanso: Se descansa ganando puntos de vida si estamos en una habitacion sin
 * enemigos. Tiene que ser habitacion clara */


/* TIPOS */

typedef unsigned int uint;
typedef unsigned char Bool;

typedef struct {
	uint chr : 7; /* caracter */
	uint atr : 7; /* atributo */
	uint ink : 3; /* tinta */
	uint bkg : 3; /* fondo */
} atributo_t;

typedef struct {
	uint obs : 1; /* 1: obstaculo */
	uint trs : 2; /* 1: transitable normal, 2 : puerta, 3: oculta */
	uint vis : 2; /* 1: visibilizado 2: visible actual */
	uint osc : 1; /* 1: oscura */
	uint hab : 4; /* numero habitacion */
	int esc : 2; /* 1: escalera descenso, -1: escalera ascenso */
} localidad_t;

struct objeto_s {
	char nom[SLEN+1];
	atributo_t atr;
	int r,c;
	uint npc : 1; /* 1: es npc 0: es item */
	union {
		struct {
			uint jug : 1; /* 1: es jugador */
			uint anm : 1; /* 1: es animal */
			uint fue : 4; /* fuerza */
			uint hab : 4; /* habilidad */
			uint vel : 4; /* velocidad */
			uint cap : 4; /* capacidad */
			uint cve : 4; /* contador velocidad */
			uint vid : 4; /* contador de vida */
			uint oro : 12; /* oro */
			uint ata : 1; /* indicador de estar siendo atacado */
			int dr,dc; /* destino del npc no jugador */
		};
		struct {
			uint cog : 1; /* 1: es cogible */
			uint ior : 1; /* 1: es oro */
			uint arm : 1; /* 1: es arma */
			uint lla : 1; /* 1: es llave */
			uint ani : 1; /* 1: es anillo */
			uint prt : 1; /* 1: es armadura de proteccion */
			uint ves : 1; /* 1: esta vestido */
			struct objeto_s* con; /* da el contenedor */
			union {
				struct { /* caracteristica del tesoro */
					uint cor : 8; /* cantidad de oro del tesoro */
				};
				struct { /* caracteristicas del arma */
					uint tia : 2; /* tipo del arma: 0:normal 1:equilibrada 2:pesada 3:magica */
					uint dad : 3; /* tipo de dado daño (1: 1D4, 2: 1D6, 3: 1D8, 4: 1D12, 5: 1D20) */
					uint nad : 2; /* numero de dados daño */
					uint pfu : 4; /* cada cuantos puntos de fuerza nos de un plus de fuerza */
					uint pha : 4; /* cada cuantos puntos de habilidad nos da un plus de habilidad */
				};
				struct { /* caracteristica de armadura */
					uint tip : 2; /* tipo de proteccion: 0:baja 1:media 2:alta 3:magica */
					uint nar: 4; /* valor de la armadura (extra) */
				};
			};
		};
		
	};
};

typedef struct objeto_s objeto_t;

typedef Bool (*Condicion)(objeto_t*);

typedef struct {
	uint def : 1; /* dice si el nivel ha sido o no definido */
	uint sem : 16; /* semilla para la creacion del mapa */
	uint num : 4; /* nivel (max 15) */
	uint lls : 4; /* llaves (max 15) */ 
	uint ars : 4; /* armas (max 15) */
	uint prs : 4; /* protecciones (max 15) */
	uint ens : 8; /* enemigos (max 256) */
	uint ani : 1; /* anillo o no */
	uint oro : 16; /* cantidad total de oro restante */
} nivel_t;

/* VARIABLES */

extern objeto_t* jugador; /* variable que guarda la direccion del jugador */

extern uint num_nivel; /* planta en la que se encuentra el jugador */

/* FUNCIONES */

/* debug.c */

void dbgprt(const char* s,...);
/* impresion de un debug */

void dbgcls();
/* borrado de pantalla del debug */

/* map.c */

void mapnew(Bool stair_up,Bool stair_down);
/* creacion del mapa stair_up tiene escalera para subir, stair_down para bajar */

localidad_t* mapget(int r,int c);
/* consigue las caracteristicas de la localidad en la columna fila pedidas */

Bool maprndpos(int* r,int* c,Bool pasadizo);
/* busca una posicion transitable, si pasadizo=true->habitacion y pasadizo, sino solo pasadizo */

uint mapngh(int r,int c,localidad_t* ln[4]);
/* da las cuatro localidades vecinas a la posicion dada N,S,E,O
 * en caso de que este fuera del mapa, la localidad sera NULL
 * se devuelve el numero de localidades diferentes a NULL*/

int mapdis(int ro,int co,int fr,int cf);
/* calcula la distancia Manhattan entre dos puntos del mapa, si alguno de ellos esta fuera del mapa, resultado -1 */

/* pantalla.c */

void panshw(int mri,int mci,int mrs,int mcs,int ro,int co);
/*muestra un sector del mapa que empieza en las coordenadas mri,mci
 * con dimensiones mrs,mcs y la posicion inicial en la pantalla terminal es ro,co */

/* mensaje.c */

void menin(const char* men,...);
/* se introduce un mensaje */

void menout();
/* se obtienen y imprimen por pantalla todos los mensajes guardados */

/* menu.c */

uint menu(char* cabecera,uint opciones,char* opcion[]);
/* se crea un menu, funciona con las teclas i: subir j: bajar q: quitar s: seleccionar.
 * Devuelve la opcion selecconada o opciones en caso de no selecionar nada */

/* objeto.c */

void objclr();
/* borra todos los objetos */

uint objsiz();
/* dice el numero de objetos que se han definido */

objeto_t* objnew(char* nom,atributo_t atr,Bool npc,Bool jug);
/* definimos objeto nuevo poniendo el nombre y diciendo si es o no un npc */

objeto_t* objcpy(objeto_t objcar);
/* copia las caracteristicas del objeto en un nuevo objeto */

int objdis(objeto_t* a,objeto_t* b);
/* da la distancia Manhattan entre dos objetos, si uno de ellos esta fuera de mapa, la distancia sera -1 */

Bool objinipos(objeto_t* obj,int r,int c);
/* damos posicion inicial a un objeto 
 * no puede existir dos npc en la misma posicion y debe ser zona transitable del mapa */

uint objfnd(objeto_t* obj[],Condicion cond);
/* se busca una serie de objetos que cumplan una determinada condicion */

Bool objmov(objeto_t* obj,int dr,int dc);
/* movemos un objeto haciendo este desplazamiento (solo npc's moviles) */

uint objinv(objeto_t* obj,objeto_t* con[]);
/* se da el inventario de un objeto npc */

Bool objcog(objeto_t* obj,objeto_t* itm);
/* un objeto npc coge un objeto */

Bool objdej(objeto_t* obj,objeto_t* itm);
/* un objeto npc deja un objeto */

objeto_t* objisprt(objeto_t* obj);
/* nos dice si un objeto tiene armadura de proteccion (cada npc solo puede llevar una) */

objeto_t* objisves(objeto_t* obj);
/* Da el objeto que es vestido por obj */

Bool objves(objeto_t* obj,objeto_t* itm);
/* viste un objeto (arma) que es poseido (solo se puede poner un arma) */

Bool objdes(objeto_t* obj,objeto_t* itm);
/* desvistes un objeto poseido y vestido */

Bool objata(objeto_t* obj,objeto_t* npc);
/* un objeto ataca a otro */

Bool objmue(objeto_t* obj);
/* se declara la muerte de un objeto */

Bool objcanact(objeto_t* obj);
/* dice si un objeto npc esta en disposicion de actuar */

/* jugador.c */

Bool jugnew();
/* definicion del jugador */

Bool jugpos(int dir);
/* posicion del jugador en el mapa. dir=0 random, dir=1: escalera de subida dir=-1:escalera de bajada */

Bool jugact();
/* accion del jugador determinada por el teclado */

Bool jugshw();
/* se muestra la pantalla cogiendo como centro la posicion del jugador */

/* item.c */

void llvlev(uint num);
/* crea todas las llaves de un nivel */

void orolev(uint oro);
/* crea el oro por nivel, oro es la cantidad de oro que meteremos en un nivel */

void anilev(uint anillo);
/* creacion del anillo, inicialmente en el ultimo nivel */

void armlev(uint num);
/* creacion de todos las armas del nivel */

void prtlev(uint num);
/* creacion de todas las armaduras del nivel */

void itmrmp(objeto_t* item);
/* rotura de un item */

/* enemigo.c */

void enelev(uint num);
/* creacion de enemigos por nivel. Si nuevo_nivel TRUE, el nivel se acaba de definir y la vida del enemigo va desde el nivel al maximo */

Bool eneact();
/* actuacion de todos los enemigos del nivel */

/* nivel.c */

void nivprm();
/* establece por primera vez el primer nivel */

Bool nivchg(int dir);
/* cambio de nivel, donde dir es la direccion (+1 bajada,-1 subida) */

/* regla.c */

Bool regla_ataque(objeto_t* atacante,objeto_t* defensor);
/* regla que determina si el atacante ataca al defensor (TRUE) */

int regla_dano(objeto_t* atacante);
/* regla que inflinge el dano al defensor, devuelve el dano */

Bool regla_arma_rotura(objeto_t* arma);
/* se decide si despues de un ataque el arma */

Bool regla_proteccion_rotura(objeto_t* proteccion);
/* se decide en que momento se rompe una armadura */

int regla_descanso();
/* descanso del jugador -1: error 0: descansa 1: no descansa porque maximo 2: no descansa porque oscuro 3: no descansa porque no esta solo */

/* rogue.c */


#endif /* ROGUE_H */
