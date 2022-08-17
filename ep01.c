#include <stdio.h>

typedef struct block{
	void *b, *n;
	void *val;
}block;
struct block mem[500];

int main(){
	printf("Olá mundo!");
}
