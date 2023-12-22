//
// Created by Carine RAFFIN-PEYLOZ on 22/12/2023.
//

#ifndef EMULATEUR_CHIP8_RAM_H
#define EMULATEUR_CHIP8_RAM_H

#include <stdint.h>
#include <stddef.h>

struct ram {
    uint8_t * first;
    size_t size;
};

struct ram * init();
int destroy(struct ram *);
int read(struct ram *, int pos);
int write(struct ram *, int pos, uint8_t weight);


#endif //EMULATEUR_CHIP8_RAM_H
