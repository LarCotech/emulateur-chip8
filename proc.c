#include <stdint.h>
#include <stdlib.h>
#include "proc.h"

/*
 * Commande build / compile
 * clang -std=c18 -Wall -pedantic test_sdl2.c -lSDL2
 * clang -std=c18 -Wall -pedantic test_sdl2.c `pkg-config --libs sdl2`
 * */

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
    // À Supp car dans structure machine (émulateur)
    // Init SLD elt
    // Display & Sprite
    struct Display* display;

    Display_init(CPU->display, 8);
    struct Sprite* sprite;
    // Keyboard
    struct Keyboard* keyboard;
    // Speaker
    struct Speaker* speaker;

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
        return (byte3<<8) + (byte2<<4) + byte1; 
}

uint8_t recupererkk(uint8_t instruction){
    uint8_t byte1, byte2;
    byte2 = (instruction & (0x00F0)) >> 4;
    byte1 = (instruction & (0x000F));
    return (byte2<<4) + byte1;
}

uint8_t recupererVx(uint8_t instruction){
    uint8_t byte1;
    byte1 = (instruction & (0x0F00)) >> 8;
    return byte1;
}

uint8_t recupererVy(uint8_t instruction){
    uint8_t byte1;
    byte1 = (instruction & (0x0F0)) >> 4;
    return byte1;
}

void effectuerActionInstruction(uint8_t instruction, instructionsChip8 * instructions, cpu * cpu){
    //ici instruction est l'instruction a decoder et instructions est la structure qui contient les id et les masques de toutes les instructions
    uint16_t result;
    uint8_t x, y, kk, key;
    
    uint8_t instructionDecodee = decoderInstruction(instruction, instructions);
    switch(instructionDecodee){
        case 0:
            //0nnn - SYS addr
            //Jump to a machine code routine at nnn.
            //This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
            break;
        case 1:
            //00E0 - CLS
            //Clear the display.

            break;
        case 2:
            //00EE - RET
            //Return from a subroutine.
            //The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
            cpu->pc = cpu->nbrstack;
            if(cpu->nbrstack != 0){
                 cpu->nbrstack --;
            }
            break;
        case 3:
            //1nnn - JP addr
            //Jump to location nnn.
            //The interpreter sets the program counter to nnn.
            cpu->pc = recupererNNN(instruction);
            break;
        case 4:
            //2nnn - CALL addr
            //Call subroutine at nnn.
            //The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            if(cpu->nbrstack <= 15){
                cpu->nbrstack ++;
                cpu->stack[cpu->nbrstack] = cpu->pc;
                cpu->pc = recupererNNN(instruction);
            }
            break;
        case 5:
            //3xkk - SE Vx, byte            
            //Skip next instruction if Vx = kk.
            //The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
            x = recupererVx(instruction);
            if(cpu->V[x]== recupererkk(instruction)){
                cpu->pc +=2;
            }
            break;
        case 6:
            //4xkk - SNE Vx, byte            
            //Skip next instruction if Vx != kk.
            //The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
            x = recupererVx(instruction);
            if(cpu->V[x] != recupererkk(instruction)){
                cpu->pc +=2;
            }
            break;
        case 7:
            //5xy0 - SE Vx, Vy
            //Skip next instruction if Vx = Vy.
            //The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            if(cpu->V[x] == cpu->V[y]){
                cpu->pc += 2;
            }
            break;
        case 8:
            //Set Vx = kk.
            //The interpreter puts the value kk into register Vx.
            x = recupererVx(instruction);
            cpu->V[x] = recupererkk(instruction);
            break;
        case 9:
            //6xkk - LD Vx, byte
            //Set Vx = Vx + kk.
            //Adds the value kk to the value of register Vx, then stores the result in Vx.
            x = recupererVx(instruction);
            cpu->V[x] += recupererkk(instruction);

            break;
        case 10:
            //7xkk - ADD Vx, byte
            //Set Vx = Vx + kk.
            //Adds the value kk to the value of register Vx, then stores the result in Vx.
            x = recupererVx(instruction);
            cpu->V[x] += recupererkk(instruction);
            break;
        case 11:
            //8xy0 - LD Vx, Vy
            //Set Vx = Vy.
            //Stores the value of register Vy in register Vx.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            cpu->V[x] = y;
            break;
        case 12:
            //8xy1 - OR Vx, Vy
            //Set Vx = Vx OR Vy.
            //Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits 
            //from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            cpu->V[x] = x | y; 
            break;
        case 13:
            //8xy2 - AND Vx, Vy
            //Set Vx = Vx AND Vy.
            //Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            cpu->V[x] = x & y; 
            break;
        case 14:
            //8xy3 - XOR Vx, Vy
            //Set Vx = Vx XOR Vy.
            //Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            cpu->V[x] = x ^ y; 
            break;
        case 15:
            //8xy4 - ADD Vx, Vy
            //Set Vx = Vx + Vy, set VF = carry.
            //The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1,
            //otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            //garde les 8 bits de oids faibles
            result = cpu->V[x] + cpu->V[y];
            if(result > 0xFF){
                cpu->V[15] = 1;
            } else cpu->V[15] = 0;
            break;
        case 16:
            //8xy5 - SUB Vx, Vy
            //Set Vx = Vx - Vy, set VF = NOT borrow.
            //If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            if(cpu->V[x] > cpu->V[y]){
                cpu->V[15] = 1;
            }else cpu->V[15] = 0;
             cpu->V[x] -=  cpu->V[y];
            break;
        case 17:
            //8xy6 - SHR Vx {, Vy}
            //Set Vx = Vx SHR 1.
            //If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
            x = recupererVx(instruction);
            if((cpu->V[x] & (0x000F)) == 1){
                 cpu->V[15] = 1;
            }else  cpu->V[15] = 0;
            cpu->V[x] /= 2;
            break;
        case 18:
            //8xy7 - SUBN Vx, Vy
            //Set Vx = Vy - Vx, set VF = NOT borrow.
            //If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            if(cpu->V[y] > cpu->V[x]){
                cpu->V[15] = 1;
            }else cpu->V[15] = 0;
            cpu->V[x] = cpu->V[y] - cpu->V[x];
            break;
        case 19:
            //8xyE - SHL Vx {, Vy}
            //Set Vx = Vx SHL 1.
            //If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
            x = recupererVx(instruction);
            if(x & (0XF000) == 1){
                cpu->V[15] = 1;
            } else cpu->V[15] = 0;
            cpu->V[x] *= 2;
            break;
        case 20:
            //9xy0 - SNE Vx, Vy
            //Skip next instruction if Vx != Vy.
            //The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            if(cpu->V[x] != cpu->V[y]){
                cpu->pc += 2;
            }
            break;
        case 21:
            //Annn - LD I, addr
            //Set I = nnn.
            //The value of register I is set to nnn
            cpu->I = recupererNNN(instruction);
            break;
        case 22:
            // Bnnn - JP V0, addr
            //Jump to location nnn + V0.
            //The program counter is set to nnn plus the value of V0.
            cpu->pc = recupererNNN(instruction) + cpu->V[0];
            break;
        case 23:
            //Cxkk - RND Vx, byte
            //Set Vx = random byte AND kk.
            //The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. 
            //The results are stored in Vx. See instruction 8xy2 for more information on AND.
            x = recupererVx(instruction);
            kk = recupererkk(instruction);
            uint8_t random = rand() % 255;
            cpu->V[x] = random & kk;
            break;
        case 24:
            // Dxyn - DRW Vx, Vy, nibble
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            // The interpreter reads n bytes from memory, starting at the address stored in I.
            // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
            // Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
            // VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of
            // it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
            // See instruction 8xy3 for more information on XOR, and section 2.4, Display,
            // for more information on the Chip-8 screen and sprites.
            x = recupererVx(instruction);
            y = recupererVy(instruction);
            /*
            for (int pos = cpu->I; pos < cpu->RAM->size; pos++) {

            }*/
            Display_DRW(cpu->display, cpu->sprite, cpu->V[x], cpu->V[y], &cpu->V[15]);

            break;
        case 25:
            // Ex9E - SKP Vx
            // Skip next instruction if key with the value of Vx is pressed.
            // Checks the keyboard, and if the key corresponding to the value of Vx is currently
            // in the down position, PC is increased by 2.
            // Check keyboard
            key = 0;
            x = recupererVx(instruction);
            if (Keyboard_get(cpu->keyboard, cpu->V[x]) == KEY_DOWN) cpu->pc += 2;
            break;
        case 26:
            // ExA1 - SKNP Vx
            // Skip next instruction if key with the value of Vx is not pressed.
            // Checks the keyboard, and if the key corresponding to the value of Vx is
            // currently in the up position, PC is increased by 2.
            key = 0;
            x = recupererVx(instruction);
            if (Keyboard_get(cpu->keyboard, cpu->V[x]) == KEY_UP) cpu->pc += 2;
            break;
        case 27:
            // Fx07 - LD Vx, DT
            // Set Vx = delay timer value.
            // The value of DT is placed into Vx.
            x = recupererVx(instruction);
            cpu->V[x] = cpu->timerDelay;
            break;
        case 28:
            // Fx0A - LD Vx, K
            // Wait for a key press, store the value of the key in Vx.
            // All execution stops until a key is pressed, then the value of that key is stored in Vx.
            x = recupererVx(instruction);
            Keyboard_wait(cpu->keyboard, &cpu->V[x]);
            break;
        case 29:
            // Fx15 - LD DT, Vx
            // Set delay timer = Vx.
            // DT is set equal to the value of Vx.
            x = recupererVx(instruction);
            cpu->timerDelay = cpu->V[x];
            break;
        case 30:
            // Fx1E - ADD I, Vx
            // Set I = I + Vx.
            // The values of I and Vx are added, and the results are stored in I.
            x = recupererVx(instruction);
            cpu->I += cpu->V[x];
            break;
        case 31:
            // Fx29 - LD F, Vx
            // Set I = location of sprite for digit Vx.
            // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
            // See section 2.4, Display, for more information on the Chip-8 hexadecimal font.

            break;
        case 32:
            //Fx33
            x = recupererVx(instruction);
            result = cpu->V[x];
            write(cpu->RAM, cpu->I + 2, result%10);
            write(cpu->RAM, cpu->I + 1, (result/10)%10);
            write(cpu->RAM, cpu->I, result/100);
            break;
        case 33:
            // Fx55 - LD [I], Vx
            // Store registers V0 through Vx in memory starting at location I.
            // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
            x = recupererVx(instruction);
            for (int i = 0; i <= x; i++) {
                write(cpu->RAM, cpu->I+i, cpu->V[i]);
            }
            break;
        case 34:
            // Fx65 - LD Vx, [I]
            // Read registers V0 through Vx from memory starting at location I.
            // The interpreter reads values from memory starting at location I into registers V0 through Vx.
            x = recupererVx(instruction);
            for (int i = 0; i <= x; i++) {
                cpu->V[i] = read(cpu->RAM, cpu->I+1);
            }
            break;
        default:
            // Cas par défaut (pour des valeurs non attendues)
            // Actions pour le cas par défaut
            break;
    }
}