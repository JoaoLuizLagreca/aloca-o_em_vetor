#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#define MEM_SIZE 16384 /* 16Kb */


/* PROTÓTIPOS */
void * aloca(size_t nbytes);
void   libera(void *p);


typedef struct part{
	void * pos;
	size_t size;
}part;
typedef struct no{
	void * ant;
	int    id;
	char   frase[50];
	void * prox;
}no;
typedef struct fila{
	struct no *comeco;
	struct no *fim;
}fila;

char mem[MEM_SIZE];
const size_t cabecalho_tam = MEM_SIZE/4;
void initMemory(){
	/* Garanta que o cabeçalho não tenha lixo no início do programa */
	int i;
	for(i=0; i<cabecalho_tam; i++)
		mem[i]=0x00;
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

int fila_incluir(fila *f, const char frase[]){
	int id=0;

	struct no *n = (struct no*)aloca(sizeof(struct no));
	if (n==NULL)
		return -1; /* Retorne erro caso não seja possível alocar */

	n->ant = NULL;
	n->prox = NULL;

	if(f->comeco==NULL){
		/*
		 fila vazia
		 O novo nó será o começo e o fim da fila
		*/
		f->comeco = n;
		f->fim = n;
	}else{
		/* 
		 Fila já tem algo 
		 Vamos mudar que o novo nó é o último da fila e o próximo nó do último anterior é o novo nó
		*/
		id=(f->fim)->id+1;
		(f->fim)->prox=n;
		n->ant=f->fim;
		f->fim=n;
	}
	n->id = id;

	strncpy(n->frase, frase, sizeof(n->frase)); /* Passar o texto para o novo nó */

	return id;
}
void fila_alterar(fila *f, const int id, const char frase[]){
	if(f->fim == NULL || (f->fim)->id<id)
		return; /* Cancele caso o ID seja inválido ou a lista esteja vazia */

	struct no *n = f->comeco;
	while(n!=NULL){
		if (n->id==id){
			strncpy(n->frase, frase, sizeof(n->frase));
			return;
		}
		n = n->prox;
	}
}
void fila_excluir(fila *f, const int id){
	if(f->fim == NULL || (f->fim)->id<id)
		return; /* Cancele caso o ID seja inválido ou a lista esteja vazia */

	struct no *n = f->comeco;
	while(n->id!=id){
		n=n->prox;
		if(n==NULL)
			return;
	}

	/* Altera o laço anterior e próximo dos nós vizinhos */
	struct no *nAnt = (struct no *)n->ant;
	struct no *nProx = (struct no *)n->prox;
	if(nAnt!=NULL)
		nAnt->prox = n->prox;
	else
		f->comeco = n->prox;
	if(nProx!=NULL)
		nProx->ant = n->ant;
	else
		f->fim = n->ant;

	libera(n); /* Libera o nó excluído da nossa memória */

}
void fila_listar(const fila *f){
	if(f->fim==NULL){
		printf("A fila está vazia!\n");
		return;
	}

	struct no *n = f->comeco;
	while(n!=NULL){
		printf("Nó #%d: %s\n", n->id, n->frase);
		n=n->prox;
	}
}


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

	struct fila *f = (struct fila *)aloca(sizeof(struct fila));
	
	int l, t;
	t=0;
	for(l=0; l<10; l++){
		char * temp = aloca(50);
		asprintf(&temp, "Teste %d", t);
		fila_incluir(f, temp);
		libera(temp);
		t++;
	}
	for(l=0;l<10;l++)
		if(l%3==0)
			fila_excluir(f, l);
	
	for(l=0; l<20; l++){
		char * temp = aloca(50);
		asprintf(&temp, "Teste %d", t);
		fila_incluir(f, temp);
		libera(temp);
		t++;
	}
	
	for(l=0;l<30;l++)
		if(l%6==0)
			fila_excluir(f, l);

	fila_incluir(f, "Se não mudar esse texto você pode me dar 0 TwT"); /* Porfavor, não. */
	fila_alterar(f, 29, "Me dá 10 OwO");

	fila_listar(f);

}
