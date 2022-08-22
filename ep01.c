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

char mem[MEM_SIZE];
const size_t cabecalho_tam = MEM_SIZE/4;
void initMemory(){
	/* Garanta que o cabeçalho não tenha lixo no início do programa */
	int i;
	for(i=0; i<cabecalho_tam; i++)
		mem[i]=0x00;
}
int my_ceil(double n){
        if ( n-((int)n)  >= 0.00005f )
                return ((int)n)+1;
	
        return (int)n;
}

/* Estrutura de dados */
struct part * enderecoNaTabela(void * pos){

	int i;
	struct part * bloco, *ultB;
	for(i=0; i<cabecalho_tam; i+=sizeof(part)){
		if(mem[i]==0x00)
			continue;

		bloco = (struct part *) &mem[i];
		ultB = (bloco->pos)+bloco->size-1;
		printf("%d - %x + %x <= %x\n", i, bloco->pos, ultB, pos);
		if(bloco->pos <= pos && ultB >= pos)
			return bloco;
	}

	return NULL;

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
