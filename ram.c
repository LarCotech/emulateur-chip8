#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define SIZE = 4096

typedef struct ram {
    uint8_t * first;
    size_t size;
} ram;

ram * init();
int destroy(struct ram *);
int read(struct ram *, int pos);
int write(struct ram *, int pos, uint8_t weight);

ram * init() {
    ram* RAM = malloc(4096);
}