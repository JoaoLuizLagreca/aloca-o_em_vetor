#include <stdio.h>

typedef struct block{
	struct block *b, *n;
	void *val;
}block;
struct block mem[500];

/* PROTÓTIPOS */
void * aloca(size_t nbytes);
void   libera(void *p);


int main(){
	printf("Olá mundo!");
}
