#include <stdio.h>

typedef struct block{
	struct block *b, *n;
	void *val;
}block;
void* mem[16384]; //16Kb

/* PROTÓTIPOS */
void * aloca(size_t nbytes);
void   libera(void *p);


int main(){
	printf("Olá mundo!");
}
