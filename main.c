#include <stdio.h>
#include "ram.h"

int main () {
    ram* RAM = init();
    printf("%zu ", RAM->size);
    *(RAM->first) = 1;
    printf("%u", *(RAM->first));
    //destroy(RAM);
    return 0;
    //TEST
}