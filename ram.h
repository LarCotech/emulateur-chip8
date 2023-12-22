#ifndef EMULATEUR_CHIP8_RAM_H
#define EMULATEUR_CHIP8_RAM_H

#include <stdint.h>
#include <stddef.h>

typedef struct ram {
    uint8_t * first;
    size_t size;
} ram;

ram * init();
int destroy(struct ram * RAM);
uint8_t read(struct ram * RAM, int pos);
int write(struct ram * RAM, unsigned int pos, uint8_t weight);


#endif //EMULATEUR_CHIP8_RAM_H
