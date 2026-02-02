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
    uchr osc : 1; /* la posicion tiene arma de ataque */
} visual_t;

typedef struct {
    visual_t vis[MAPAR][MAPAC]; //guarda todos los datos de la visualizacion
    int ri,ci; //extremo superior izquierdo de la parte visual
    int rf,cf;  //extremo inverior derecho de la parte visual
    objeto_t* obj;//objeto mas cercano de la visual 
    Bool jug; //el jugador existe en la visual
} vismap_t;

static vismap_t _visset(objeto_t* e) {
    /* determina la visual del enemigo */
    vismap_t vm;
    for(int rr=0;rr<MAPAR;rr++) {
        for(int cc=0;cc<MAPAC;cc++) {
            vm.vis[rr][cc]=(visual_t){0,0,0,0};
        }
    }
    vm.ri=vm.ci=0;
    vm.rf=MAPAR-1;
    vm.cf=MAPAC-1;
    vm.obj=NULL;
    vm.jug=FALSE;
    localidad_t* le=mapget(e->r,e->c);
    if(le && le->osc) {
        for(int rr=e->r-1;rr<=e->r+1;rr++) {
            for(int cc=e->c-1;cc<=e->c+1;cc++) {
                localidad_t l=mapgeg(rr,cc);
                if(l->trs==1 && mapdis(rr,cc,e->r,e->c)==1) vm.vis[rr][cc].fre=1;
            }
        }
        vm.ri=e->r-1;
        vm.ci=e->c-1;
        vm
            



