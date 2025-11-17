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

void destroiAVL(node *a){

    if(a == NULL){
        return;
    }

    destroiAVL(a->esq);
    destroiAVL(a->dir);

    free(a);
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
    a->comparacoes++; // comparacao 
    return altura(no->esq, a) - altura(no->dir, a);
}


int altura(node* no, arvoreAVL *a){
    int esquerda = 0, direita = 0;

    if(no == NULL){ // caso de erro
        return 0;
    }

    if(no->esq != NULL){
        //a->comparacoes++; tirei pq está ineficiente
        esquerda = altura(no->esq, a) + 1;
    } 

    if(no->dir != NULL){
        //a->comparacoes++; tirei pq está ineficiente
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

    // atualizar altura
    no->altura = altura(no, a);
    direita->altura = altura(direita, a);

    // a comparacao ficaria na altura, que vai ser atualizada
    return direita;

}

node* rsd(node* no, arvoreAVL *a){ // mesma coisa agora com a esquerda
    node* pai = no->pai;
    node* esquerda = no->esq;  

    no->esq = esquerda->dir;
    no->pai = esquerda; 

    
    esquerda->dir = no;
    esquerda->pai = pai;

    // atualizar altura
    no->altura = altura(no, a);
    esquerda->altura = altura(esquerda, a);

    // a comparacao ficaria na altura, que vai ser atualizada
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
        a->comparacoes++; // comparacao com null
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

    // balanceamento, cada if faz 2 comparação dentro do fb
    if (fb(no, a) > 1 && fb(no->esq, a) >= 0){ 
        
        return rsd(no, a);
    }

    if (fb(no, a) < -1 && fb(no->dir, a) <= 0 ){
        
        return rse(no, a);
    }

    if (fb(no, a) > 1 && fb(no->dir, a) < 0 ){
        
        return rdd(no, a);
    }

    if (fb(no, a) < -1 && fb(no->dir, a) > 0) {
        
        return rde(no, a);
    }

    return no;
}

node* remover(node *no, int chave, arvoreAVL *a){

    if(no == NULL){
        printf("\nValor nao encontrado para remocao\n");
        a->comparacoes++; // comparacao com null
        return NULL;
    }

    a->comparacoes++; // comparacao com o valor
    if(chave < no->valor){
        no->esq = remover(no->esq, chave, a);
        if (no->esq != NULL) no->esq->pai = no;
        a->comparacoes++;
    } else if(chave > no->valor){
        no->dir = remover(no->dir, chave, a);
        if (no->dir != NULL) no->dir->pai = no;
        a->comparacoes++;
    } else{
        // Achou o nó

        // CASO 1 DA REMOÇÃO, NO REMOVIDO NAO TENHA FILHOS
        if(no->esq == NULL && no->dir == NULL){
            free(no);
            a->comparacoes = a->comparacoes + 2; // comparação dos dois nós com nulo
            return NULL;
        }

        // CASO 2, NO REMOVIDO TEM UM FILHO
        if(no->esq == NULL){
            no->dir->pai = no->pai;
            free(no);
            a->comparacoes++; // comparação com nulo
            return no->dir;
        } 
        if(no->dir == NULL){
            no->esq->pai = no->pai;
            free(no);
            a->comparacoes++; // comparação com nulo
            return no->esq;
        }

        // CASO 3, NO REMOVIDO TEM DOIS FILHOS
        // Encontrar o sucessor, nesse caso menor sub arvore da direita, para nao quebrar a propridade de arvore binaria
        // Caso eu pegue só o nó da esquerda, vai quebrar os valores
        node *temp = no->dir;
        while(temp->esq != NULL){
            a->comparacoes++; // comparação com nulo a CADA ITERAÇÃO
            temp = temp->esq;
        }
    

        // colocando o sucessor no lugar desejado, copio o valor dele
        no->valor = temp->valor;
        // removo o sucessor (ira cair no caso 1 ou 2, pq será o mais a esquerda)
        no->dir = remover(no->dir, temp->valor, a);
        if (no->dir != NULL){
            no->dir->pai = no;
            a->comparacoes++;
        }
        
    }


        // se só tinha um nó
        ////fi(no == NULL){
         ///   a->comparacoes++; // conto comparação?
         //   return NULL;
        //}

        // atualizar altura de cada 
        no->altura = altura(no, a);
        a->comparacoes++;

        // balanceamento, igual o da adicao, conto 2 comparações
        if (fb(no, a) > 1 && fb(no->esq, a) >= 0){ 
            
            return rsd(no, a);
        }

        if (fb(no, a) < -1 && fb(no->dir, a) <= 0 ){
            
            return rse(no, a);
        }

        if (fb(no, a) > 1 && fb(no->dir, a) < 0 ){
            
            return rdd(no, a);
        }

        if (fb(no, a) < -1 && fb(no->dir, a) > 0) {
            
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

void printArvore(node *no, int nivel, arvoreAVL *a){ // so pra visualizar, nao fiz essa funcao
    if(no == NULL){
        return;
    }

    printArvore(no->dir, nivel + 1, a);

    for(int i = 0; i < nivel; i++){
        printf("     ");
    }

    printf("%d(%d)\n", no->valor, fb(no, a));

    printArvore(no->esq, nivel + 1, a);
}
