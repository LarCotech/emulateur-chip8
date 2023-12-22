#include <stdint.h>
#include <stdlib.h>
#include "ram.h"

typedef struct cpu {
    // RAM
    ram* RAM;
    uint8_t V[16]; //le registre
    uint16_t I; //stocke une adresse mémoire ou dessinateur
    uint16_t stack[16]; //pour gérer les sauts dans « mémoire », 16 au maximum
    uint8_t nbrstack; //stocke le nombre de sauts effectués pour ne pas dépasser 16
    uint8_t timerDelay; //compteur pour la synchronisation
    uint8_t timerSound; //compteur pour le son
    uint16_t pc; //pour parcourir le tableau « mémoire »
} cpu;

cpu* initrCpu();
int decompter(cpu* CPU);


cpu* initrCpu() {

    // Allocation structure cpu
    cpu* CPU = (cpu*) malloc(sizeof(cpu));
    // Init RAM
    CPU->RAM = init();

    // Registre d'adresse
    CPU->I = 0;

    // Registre V0 à VF et Stack (pile)
    for(unsigned int i = 0; i < 16; i++) {
        CPU->V[i] = 0;
        CPU->stack[i] = 0;
    }

    CPU->pc = 0x200;
    CPU->nbrstack = 0;
    CPU->timerDelay = 0;
    CPU->timerSound = 0;

    return CPU;
}

int decompter(cpu* CPU) {
    //Timers
    if(CPU->timerDelay>0) CPU->timerDelay--;
    if(CPU->timerSound>0) CPU->timerSound--;

    return 0;
}