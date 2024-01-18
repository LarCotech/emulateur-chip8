
#ifndef EMULATEUR_CHIP8_PROC_H
#define EMULATEUR_CHIP8_PROC_H

#include <stdint.h>
#include "ram.h"
#include <stdint.h>
#include <stdlib.h>
#define NBINSTRUCTIONS 35
// A modifier
#include "include//display/display.h"
#include "include/keyboard/keyboard.h"
#include "include/speaker/speaker.h"

typedef struct cpu {
    // RAM
    ram* RAM;
    uint8_t V[16]; //les registres
    uint16_t I; //stocke une adresse mémoire ou dessinateur
    uint16_t stack[16]; //pour gérer les sauts dans « mémoire », 16 au maximum
    uint8_t nbrstack; //stocke le nombre de sauts effectués pour ne pas dépasser 16
    uint8_t timerDelay; //compteur pour la synchronisation
    uint8_t timerSound; //compteur pour le son
    uint16_t pc; //pour parcourir le tableau « mémoire »
    // Libprovided
    // Display & Sprite
    struct Display* display;
    struct Sprite* sprite;
    // Keyboard
    struct Keyboard* keyboard;
    // Speaker
    struct Speaker* speaker;
} cpu;



cpu* initrCpu();
int decompter(cpu* CPU);

/*
 * Recupere deux cases adjacentes en memoire à partir du pc (compteur programme) et renvoi une instruction chip-8 codee sur 16 octets
 */
uint16_t recupererInstruction(cpu * CPU);

typedef struct instructionsChip8{
    uint16_t masque[NBINSTRUCTIONS];
    uint16_t id[NBINSTRUCTIONS];
}instructionsChip8;


void initialiserInstructionsChip8(instructionsChip8 * instructions);

/*
 * Prend en parametre un instruction chip8 sur 16 bytes et retourne un uint8_t compris entre 0 et 35 
 * qui correspond a l'id (de la structure instructionsChip8) de l'instruction passé en parametere. 
 */
uint8_t decoderInstruction(uint16_t instruction, instructionsChip8 * instructions);

/*
 * prend une instruction chip-8 et execute l'action de l'insrtruction
 */
void effectuerActionInstruction(uint8_t instruction, instructionsChip8 * instructions, cpu* cpu);

/*
 * A partir d'une instuction renvoi l'adresse correspondante a nnn (les 3 bits de poids faible) 
 */
uint8_t recupererNNN(uint8_t instruction);

/*
 * retourne les 2 bits de poids faible de l'instruction 
 */
uint8_t recupererkk(uint8_t instruction);

/*
 * retourne le 2 eme byte de poid fort(0x0F00)
 */
uint8_t recupererVx(uint8_t instruction);

/*
 * retourne le 2 eme bit de poid faible (0x00F0)
 */
uint8_t recupererVy(uint8_t instruction);
#endif //EMULATEUR_CHIP8_PROC_H



