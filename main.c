#include <stdio.h>
#include <stdint.h>
#include "ram.h"

int main () {
    
    int continuer = 1;
    
    ram* RAM = init();
    printf("%zu ", RAM->size);
    *(RAM->first) = 1;
    printf("%u", *(RAM->first));
    
    //do{
        
    //}while(continuer == 1);
    destroy(RAM);
    return 0;
}