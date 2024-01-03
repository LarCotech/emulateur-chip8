#include <stdint.h>
#include <stdlib.h>
#include "ram.h"
#include "proc.h"


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

uint16_t recupererInstruction(cpu * CPU){
    /*Ram est un tableau avec des tailles de 8 bits, une instruction chip-8 est codee sur 16 bits.
     *on recupere en mémoire 2 cases adjacentes et on décale le contenu de la 1ere de 8 bits de poids forts et on ajoutele contenu de la 2eme.*/
    if(CPU->pc + 1 <= CPU->RAM->size){
        return ((CPU->RAM->first[CPU->pc] << 8) + CPU->RAM->first[CPU->pc +1]);
    } else {
        return 0x0000;
    }
}

void initialiserInstructionsChip8(instructionsChip8 * instruction){
  instruction->masque[0]= 0x0000; instruction->id[0]=0x0FFF;          /* 0NNN */ 
  instruction->masque[1]= 0xFFFF; instruction->id[1]=0x00E0;          /* 00E0 */ 
  instruction->masque[2]= 0xFFFF; instruction->id[2]=0x00EE;          /* 00EE */ 
  instruction->masque[3]= 0xF000; instruction->id[3]=0x1000;          /* 1NNN */ 
  instruction->masque[4]= 0xF000; instruction->id[4]=0x2000;          /* 2NNN */ 
  instruction->masque[5]= 0xF000; instruction->id[5]=0x3000;          /* 3XNN */ 
  instruction->masque[6]= 0xF000; instruction->id[6]=0x4000;          /* 4XNN */ 
  instruction->masque[7]= 0xF00F; instruction->id[7]=0x5000;          /* 5XY0 */ 
  instruction->masque[8]= 0xF000; instruction->id[8]=0x6000;          /* 6XNN */ 
  instruction->masque[9]= 0xF000; instruction->id[9]=0x7000;          /* 7XNN */ 
  instruction->masque[10]= 0xF00F; instruction->id[10]=0x8000;          /* 8XY0 */ 
  instruction->masque[11]= 0xF00F; instruction->id[11]=0x8001;          /* 8XY1 */ 
  instruction->masque[12]= 0xF00F; instruction->id[12]=0x8002;          /* 8XY2 */ 
  instruction->masque[13]= 0xF00F; instruction->id[13]=0x8003;          /* BXY3 */ 
  instruction->masque[14]= 0xF00F; instruction->id[14]=0x8004;          /* 8XY4 */ 
  instruction->masque[15]= 0xF00F; instruction->id[15]=0x8005;          /* 8XY5 */ 
  instruction->masque[16]= 0xF00F; instruction->id[16]=0x8006;          /* 8XY6 */ 
  instruction->masque[17]= 0xF00F; instruction->id[17]=0x8007;          /* 8XY7 */ 
  instruction->masque[18]= 0xF00F; instruction->id[18]=0x800E;          /* 8XYE */ 
  instruction->masque[19]= 0xF00F; instruction->id[19]=0x9000;          /* 9XY0 */ 
  instruction->masque[20]= 0xF000; instruction->id[20]=0xA000;          /* ANNN */ 
  instruction->masque[21]= 0xF000; instruction->id[21]=0xB000;          /* BNNN */ 
  instruction->masque[22]= 0xF000; instruction->id[22]=0xC000;          /* CXNN */ 
  instruction->masque[23]= 0xF000; instruction->id[23]=0xD000;          /* DXYN */ 
  instruction->masque[24]= 0xF0FF; instruction->id[24]=0xE09E;          /* EX9E */ 
  instruction->masque[25]= 0xF0FF; instruction->id[25]=0xE0A1;          /* EXA1 */ 
  instruction->masque[26]= 0xF0FF; instruction->id[26]=0xF007;          /* FX07 */ 
  instruction->masque[27]= 0xF0FF; instruction->id[27]=0xF00A;          /* FX0A */ 
  instruction->masque[28]= 0xF0FF; instruction->id[28]=0xF015;          /* FX15 */ 
  instruction->masque[29]= 0xF0FF; instruction->id[29]=0xF018;          /* FX18 */ 
  instruction->masque[30]= 0xF0FF; instruction->id[30]=0xF01E;          /* FX1E */ 
  instruction->masque[31]= 0xF0FF; instruction->id[31]=0xF029;          /* FX29 */ 
  instruction->masque[32]= 0xF0FF; instruction->id[32]=0xF033;          /* FX33 */ 
  instruction->masque[33]= 0xF0FF; instruction->id[33]=0xF055;          /* FX55 */ 
  instruction->masque[34]= 0xF0FF; instruction->id[34]=0xF065;          /* FX65 */ 

}


uint8_t decoderInstruction(uint16_t instruction, instructionsChip8 * instructions){
    uint8_t idInstruction;
    for(int i = 0; i < NBINSTRUCTIONS; i++){
        //on applique à l'instruction passe en parametre tous les masques de notre structure instructionsChip8
        //si c'est egal a l'id d'une des nos instruction dans la structure alors c'est l'action de cette instruction qui est à effectuer.
        uint16_t res = instructions->masque[i] & instruction;
        if(res == instructions->id[i]){
            idInstruction =  instructions->id[i];
            break;
        }
    }
    return idInstruction;
}

uint8_t recupererNNN(uint8_t instruction){
        uint8_t byte1, byte2, byte3;
        byte3 = (instruction & (0x0F00)) >> 8;
        byte2 = (instruction & (0x00F0)) >> 4;
        byte1 = (instruction & (0x000F));
        return byte3 + byte2 + byte1; 
}

void effectuerActionInstruction(uint8_t instruction, instructionsChip8 * instructions, cpu * cpu){
    //ici instruction est l'instruction a decoder et instructions est la structure qui contient les id et les masques de toutes les instructions
    uint8_t instructionDecodee = decoderInstruction(instruction, instructions);
    switch(instructionDecodee){
        case 0:
            //Jump to a machine code routine at nnn.
            //This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
            break;
        case 1:
            //Clear the display.

            break;
        case 2:
            //Return from a subroutine.
            //The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
            cpu->pc = cpu->nbrstack;
            if(cpu->nbrstack != 0){
                 cpu->nbrstack --;
            }
            break;
        case 3:
            //Jump to location nnn.
            //The interpreter sets the program counter to nnn.
            cpu->pc = recupererNNN(instruction);
            break;
        case 4:
            //Call subroutine at nnn.
            //The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            if(cpu->nbrstack <= 15){
                cpu->nbrstack ++;
                cpu->stack[cpu->nbrstack] = cpu->pc;
                cpu->pc = recupererNNN(instruction);
            }
            break;
        case 5:
            //Skip next instruction if Vx = kk.
            //The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
            break;
        case 6:
            //Skip next instruction if Vx != kk.
            //The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
            break;
        case 7:
            //Skip next instruction if Vx = Vy.
            //The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
            break;
        case 8:
            //Set Vx = kk.
            //The interpreter puts the value kk into register Vx.
            break;
        case 9:
            //Set Vx = Vx + kk.
            //Adds the value kk to the value of register Vx, then stores the result in Vx.

            break;
        case 10:
            // Actions pour le cas 10
            break;
        case 11:
            // Actions pour le cas 11
            break;
        case 12:
            // Actions pour le cas 12
            break;
        case 13:
            // Actions pour le cas 13
            break;
        case 14:
            // Actions pour le cas 14
            break;
        case 15:
            // Actions pour le cas 15
            break;
        case 16:
            // Actions pour le cas 16
            break;
        case 17:
            // Actions pour le cas 17
            break;
        case 18:
            // Actions pour le cas 18
            break;
        case 19:
            // Actions pour le cas 19
            break;
        case 20:
            // Actions pour le cas 20
            break;
        case 21:
            // Actions pour le cas 21
            break;
        case 22:
            // Actions pour le cas 22
            break;
        case 23:
            // Actions pour le cas 23
            break;
        case 24:
            // Actions pour le cas 24
            break;
        case 25:
            // Actions pour le cas 25
            break;
        case 26:
            // Actions pour le cas 26
            break;
        case 27:
            // Actions pour le cas 27
            break;
        case 28:
            // Actions pour le cas 28
            break;
        case 29:
            // Actions pour le cas 29
            break;
        case 30:
            // Actions pour le cas 30
            break;
        case 31:
            // Actions pour le cas 31
            break;
        case 32:
            // Actions pour le cas 32
            break;
        case 33:
            // Actions pour le cas 33
            break;
        case 34: 
            break;
        default:
            // Cas par défaut (pour des valeurs non attendues)
            // Actions pour le cas par défaut
            break;
    }
}