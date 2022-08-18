#include <stdio.h>

#define MEM_SIZE 16384 /* 16Kb */

typedef struct block{
	struct block *b, *n;
	void *val;
}block;
typedef struct part{
	void * pos;
	size_t size;
}part;

void* mem[MEM_SIZE];
const size_t cabecalho_tam = MEM_SIZE/4;
void initMemory(){
	/* Garanta que o cabeçalho não tenha lixo no início do programa */
	int i;
	for(i=0; i<cabecalho_tam; i++)
		mem[i]=0x00;
}

/* PROTÓTIPOS */
void * aloca(size_t nbytes);
void   libera(void *p);

void * aloca(size_t nbytes){
	
}

int main(){
	initMemory();
	printf("Olá mundo!");
}
