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
			(pos<bloco->pos && ultP>=bloco->pos)||
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
	char * lastAddress=&mem[cabecalho_tam];
	struct part *ult = NULL;
	
	BUSCA_BLOCO:{

		if(
			lastAddress>=&mem[MEM_SIZE-1] /*Se chegar até o último bloco da memória, significa que não foi possível encontrar um espaço livre, retorne NULL*/
			|| lastAddress<&mem[cabecalho_tam] /*Ou, por segurança, interrompa se o último endereço for menor que o endereço reservado de dados inicial*/
		)
			return NULL;

		ult = enderecoNaTabela(lastAddress, nbytes);
		if(ult == NULL) /* Vá para o parágrafo de alocação caso encontre um bloco livre */
			goto ALOCACAO;

		/* Procure no próximo bloco após o final indicado pelo cabeçalho */
		lastAddress = ult->pos + ult->size;
		goto BUSCA_BLOCO;
		
	}

	ALOCACAO:{

		/* Encontrar espaço livre no cabeçalho */
		int i;
		for (i=0; i<cabecalho_tam; i+=sizeof(part)){
			if(mem[i]==0x00){
				/* Criar e armazenar informação de particionamento */
				ult = (struct part *)&mem[i];
				ult->pos = lastAddress;
				ult->size = nbytes;

				return lastAddress; //Retorna o endereço da memória
			}
		}
	}

	return NULL;
}

void libera(void *p){
	struct part * b  = enderecoNaTabela(p, 1);
	if (b!=NULL)
		b->pos=0x00; /* Define bloco do cabeçalho como livre caso encontrado */
}

int main(){
	initMemory();
	printf("Olá mundo!");
}
