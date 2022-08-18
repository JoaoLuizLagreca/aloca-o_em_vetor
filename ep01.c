#include <stdio.h>

#define MEM_SIZE 16384

typedef struct block{
	struct block *b, *n;
	void *val;
}block;
typedef struct part{
	void * pos;
	size_t size;
}part;

void* mem[MEM_SIZE]; //16Kb
const size_t cabecalho_tam = MEM_SIZE/4;

/* PROTÓTIPOS */
void * aloca(size_t nbytes);
void   libera(void *p);

void * aloca(size_t nbytes){
	
}

int main(){
	printf("Olá mundo!");
}
