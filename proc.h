
#ifndef EMULATEUR_CHIP8_PROC_H
#define EMULATEUR_CHIP8_PROC_H

#include <stdint.h>
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

#endif //EMULATEUR_CHIP8_PROC_H



