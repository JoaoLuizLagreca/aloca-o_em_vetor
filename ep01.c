#include <stdio.h>

typedef struct block{
	struct block *b, *n;
	void *val;
}block;
struct block mem[500];

int main(){
	printf("Olá mundo!");
}
