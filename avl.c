#include "avl.h"
#include <stdlib.h>
#include <stdio.h>

long getComparacoesAVL(arvoreAVL *a){
    return a->comparacoes;
}

arvoreAVL* criaArv(){
    arvoreAVL *a;
    a = malloc(sizeof(arvoreAVL));
    a->tamanho = 0;
    a->comparacoes = 0;
    a->raiz = NULL;
    return a;
}

int vaziaArv(arvoreAVL *a){
return (a->raiz == NULL);
}

node *criaNo(int valor){
    node *novo;
    novo = malloc(sizeof(node));
    novo->pai = NULL;
    novo->esq = NULL;
    novo->dir = NULL;
    
    novo->altura = 0;
    novo->valor = valor;
    
    return novo;
}



int fb(node* no, arvoreAVL *a) {
    if (no == NULL) return 0;
    return altura(no->esq, a) - altura(no->dir, a);
}


int altura(node* no, arvoreAVL *a){
    int esquerda = 0, direita = 0;

    if(no == NULL){ // caso de erro
        a->comparacoes++;
        return 0;
    }

    if(no->esq != NULL){
        a->comparacoes++;
        esquerda = altura(no->esq, a) + 1;
    } 

    if(no->dir != NULL){
        a->comparacoes++;
        direita = altura(no->dir, a) + 1;
    }

    return maior(direita, esquerda);

}

node* rse(node* no, arvoreAVL *a){
    node* pai = no->pai; // salvando o pai do nó que ira ser rotacionado
    node* direita = no->dir; // salvando a direita do no, que irá virar o NO atual

    no->dir = direita->esq; // o no da direita vai virar o da esquerda do no atual
    no->pai = direita; // o no pai deste atual vai virar o da direita

    // substituição do nó da direita no lugar desse atual
    direita->esq = no;
    direita->pai = pai;

    a->comparacoes++;
    return direita;

}

node* rsd(node* no, arvoreAVL *a){ // mesma coisa agora com a esquerda
    node* pai = no->pai;
    node* esquerda = no->esq;  

    no->esq = esquerda->dir;
    no->pai = esquerda; 

    
    esquerda->dir = no;
    esquerda->pai = pai;

    a->comparacoes++;
    return esquerda;

}

node* rde(node* no, arvoreAVL *a){
    no->dir = rsd(no->dir, a);
    return rse(no, a);
}

node* rdd(node *no, arvoreAVL *a){
    no->esq = rse(no->esq, a);

    return rsd(no, a);
}

node* adiciona(node* no, int valor, arvoreAVL *a) {
    
    if (no == NULL){
        a->comparacoes++; // +1 cria o nó
        return criaNo(valor);
    }

    a->comparacoes++; // +1 comparado do valor
    if (valor < no->valor){
        no->esq = adiciona(no->esq, valor, a);
        no->esq->pai = no;
    }
    else if(valor > no->valor){
        no->dir = adiciona(no->dir, valor, a);
        no->dir->pai = no; 
    } else{
        return no; // se for igual
    }

    // atualiza a altura de cada nó // ou uso esse campo ou uso dinamico
    no->altura = altura(no, a); 
    a->comparacoes++; // +1 atualizando altura

    // balanceamento
    if (fb(no, a) > 1 && fb(no->esq, a) >= 0){ 
        a->comparacoes++;
        return rsd(no, a);
    }

    if (fb(no, a) < -1 && fb(no->dir, a) <= 0 ){
        a->comparacoes++;
        return rse(no, a);
    }

    if (fb(no, a) > 1 && fb(no->dir, a) < 0 ){
        a->comparacoes++;
        return rdd(no, a);
    }

    if (fb(no, a) < -1 && fb(no->dir, a) > 0) {
        a->comparacoes++;
        return rde(no, a);
    }

    return no;
}


int maior(int a, int b){
    if(a>b){
        return a;
    } else{
        return b;
    }
}

void preOrder(node *no){
    if(no == NULL){
        return;
    }
    
    printf("%d ", no->valor);
    preOrder(no->esq);
    preOrder(no->dir);
}