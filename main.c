#include <stdlib.h>
#include <stdio.h>
#include "avl.h"
//#include "testes.h" ?

int main(){

    arvoreAVL *a = criaArv();
    a->raiz = adiciona(a->raiz,4, a);
    a->raiz = adiciona(a->raiz,2, a);
    a->raiz = adiciona(a->raiz,1, a);
    a->raiz = adiciona(a->raiz,3, a);
    a->raiz = adiciona(a->raiz,8, a);
    a->raiz = adiciona(a->raiz,9, a);
    a->raiz = adiciona(a->raiz,6, a);
    a->raiz = adiciona(a->raiz,5, a);
    a->raiz = adiciona(a->raiz,7, a);
  
    printf("\nArvore: ");
    preOrder(a->raiz);
    printf("\nComparacoes: %li\n", getComparacoesAVL(a));
    
    return 0;
}