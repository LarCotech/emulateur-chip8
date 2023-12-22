#include <stdint.h>
#include <stddef.h>

#define SIZE = 4096

struct ram * init();
int destroy(struct ram *);
int read(struct ram *, int pos);
int write(struct ram *, int pos, uint8_t weight);

