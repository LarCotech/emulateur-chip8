#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define SIZE 4096

typedef struct ram {
    uint8_t * first;
    size_t size;
} ram;

ram * init() {

    //Allocation Memoire comme tableau d'élément uint8_t
    uint8_t* memoire = calloc(SIZE, sizeof(uint8_t));
    if (memoire == NULL)
        return (ram *) EXIT_FAILURE;

    // Allocation de la structre ram
    ram * RAM = malloc(sizeof(ram *));
    //Init valeur champs
    RAM->first = memoire;
    RAM->size = SIZE;

    return RAM;
}

int destroy(struct ram * RAM) {
    // Libère la structure RAM
    free(RAM->first);
    free(RAM);
    return 0;
}

uint8_t read(struct ram * RAM, int pos) {
    return *(RAM->first + pos);
}

int write(struct ram * RAM, unsigned int pos, uint8_t weight) {
    if ((0 < pos) && (pos < RAM->size)) {
        *(RAM->first + pos) = weight;
        return 0;
    }
    return 1;
}