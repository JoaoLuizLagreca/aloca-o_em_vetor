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
struct part * enderecoNaTabela(void * pos, size_t size){

	int i;
	struct part * bloco;
	void *ultB;
	void *ultP = pos+size-1;
	for(i=0; i<cabecalho_tam; i+=sizeof(part)){
		if(mem[i]==0x00)
			continue;

		bloco = (struct part *) &mem[i];
		ultB = (bloco->pos)+bloco->size-1;
		if(
			(pos<bloco->pos && ultP>=bloco->pos)|| /* Não sei porque o compilador falar que o ultP ou o ultP são inteiros, sendo que estão funcionando como ponteiros, mas enfim...*/
			(pos>=bloco->pos && pos<=ultB)
		)
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
