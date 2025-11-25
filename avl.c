
#include "arvores.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum { INSERCAO, REMOCAO } Operacao;
long custo_insercao = 0;   // DEFINIÇÃO
long custo_remocao = 0;    // DEFINIÇÃO
Operacao operacao_atual;   // DEFINIÇÃO


typedef struct node{
    struct node* pai;
    struct node* esq;
    struct node* dir;
    int altura;
    int valor;
} node;

typedef struct arvoreAVL{
    struct node* raiz;
} arvoreAVL;

/* CONTABILIZAÇÕES:
Comparações
Operações de rotação (um custo só)

Não estou contabilizando:
Acesso a nós 
*/

int maior(int a, int b){

    if(a>b){
        return a;
    } else{
        return b;
    }
}

void contaCusto(int vezes) {
    if (operacao_atual == INSERCAO)
        custo_insercao += vezes;
    else
        custo_remocao += vezes;
}

arvoreAVL* criaArv(){
    arvoreAVL *a;
    a = malloc(sizeof(arvoreAVL));
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
    
    novo->altura = 1; // Vou usar 1 como altura base
    novo->valor = valor;
    
    return novo;
}




// Função para pegar a altura de um nó pela struct
int getAltura(node* no){
    if(no == NULL){
        return 0;
    }
    return no->altura;
}

// Atualizar altura de um nó
void atualizarAltura(node* no){

    if(no == NULL){
        return;
    }
    int alturaEsq = getAltura(no->esq);
    int alturaDir = getAltura(no->dir);
    no->altura = maior(alturaEsq, alturaDir) + 1;

}

// Cálculo do fator de balanceamento
int fb(node* no) {
    if (no == NULL){
        return 0;
    }
    return getAltura(no->esq) - getAltura(no->dir);
}
node* rse(node* no){
    contaCusto(1); // Rotações contam custo (1 validação estrutural)

    node* pai = no->pai; // salvando o pai do nó que ira ser rotacionado
    node* direita = no->dir; // salvando a direita do no, que irá virar o NO atual

    no->dir = direita->esq; // o no da direita vai virar o da esquerda do no atual
    no->pai = direita; // o no pai deste atual vai virar o da direita

    // CORREÇÃO AQUI: Atualizar o pai dessa subárvore que foi movida
    if (no->dir != NULL) {
        no->dir->pai = no;
    }

    // substituição do nó da direita no lugar desse atual
    direita->esq = no;
    direita->pai = pai;

    // atualizar altura
    atualizarAltura(no);
    atualizarAltura(direita);

    return direita;

}

node* rsd(node* no){ // mesma coisa agora com a esquerda
    contaCusto(1); // Rotações contam custo (1 validação estrutural)
    node* pai = no->pai;
    node* esquerda = no->esq;  

    no->esq = esquerda->dir;
    no->pai = esquerda; 

    // CORREÇÃO AQUI: Atualizar o pai dessa subárvore que foi movida
    if (no->esq != NULL) {
        no->esq->pai = no;
    }
    
    esquerda->dir = no;
    esquerda->pai = pai;

    // atualizar altura
    atualizarAltura(no);
    atualizarAltura(esquerda);

    return esquerda;

}

node* rde(node* no){
    no->dir = rsd(no->dir);
    return rse(no);
}

node* rdd(node *no){
    no->esq = rse(no->esq);

    return rsd(no);
}

node* adiciona(node* no, int valor) {
    contaCusto(1);  // conta esse nivel atual
    if (no == NULL){
        return criaNo(valor);
    }

    if (valor < no->valor){
        
        no->esq = adiciona(no->esq, valor);
        no->esq->pai = no;
    
    }
    else {
        
        if(valor > no->valor){
            
            no->dir = adiciona(no->dir, valor);
            no->dir->pai = no; 
        } else{
            return no; // se for igual
        }
    }

    contaCusto(1); // VERIFICAR NECESSIDADE DE BALANCEAMENTO
    atualizarAltura(no);
    //contaCusto(1); // altura
  
    if (fb(no) > 1 && fb(no->esq) >= 0){ 
    
        return rsd(no);
    }

    if (fb(no) < -1 && fb(no->dir) <= 0 ){
       
        return rse(no);
    }

    if (fb(no) > 1 && fb(no->dir) < 0 ){ // menor igual?
    
        return rdd(no);
    }

    if (fb(no) < -1 && fb(no->dir) > 0) { // maior igual?
    
        return rde(no);
    }

    return no;
}

node* remover(node *no, int chave){
    contaCusto(1); // avança nivel cada recursao
    if(no == NULL){
        printf("\nValor nao encontrado para remocao\n");
        return NULL;
    }

    if(chave < no->valor){
        
        no->esq = remover(no->esq, chave);
        if (no->esq != NULL) no->esq->pai = no; 
        
   
    } else if(chave > no->valor){
        
        no->dir = remover(no->dir, chave);
        if (no->dir != NULL) no->dir->pai = no;
    
    } else{
        // Achou o nó

        // CASO 1 DA REMOÇÃO, NO REMOVIDO NAO TENHA FILHOS
        if(no->esq == NULL && no->dir == NULL){
            free(no);
            return NULL;
        }

        // CASO 2, NO REMOVIDO TEM UM FILHO
        if(no->esq == NULL){
     
            no->dir->pai = no->pai;
            free(no);
            return no->dir;
        } 
        if(no->dir == NULL){
           
            no->esq->pai = no->pai;
            free(no);
            return no->esq;
        }

        // CASO 3, NO REMOVIDO TEM DOIS FILHOS
        // Encontrar o sucessor, nesse caso menor sub arvore da direita, para nao quebrar a propridade de arvore binaria
        // Caso eu pegue só o nó da esquerda, vai quebrar os valores
        node *temp = no->dir;
        while(temp->esq != NULL){
            contaCusto(1); // avança nível
            temp = temp->esq;
        }
    

        // colocando o sucessor no lugar desejado, copio o valor dele
        no->valor = temp->valor;
        // removo o sucessor (ira cair no caso 1 ou 2, pq será o mais a esquerda)
        no->dir = remover(no->dir, temp->valor);
        if (no->dir != NULL){
            no->dir->pai = no; 
        }
        
    }

        // se só tinha um nó
        if(no == NULL){
            return NULL;
        }

        // atualizar altura do nó atual
        atualizarAltura(no);
        //contaCusto(1); // altura é necessário e especifico da AVL, validação

        contaCusto(1); // VALIDAÇÃO DE BALANCEAMENTO
        // balancear
        if (fb(no) > 1 && fb(no->esq) >= 0){ 
            
            return rsd(no);
        }

        if (fb(no) < -1 && fb(no->dir) <= 0 ){
            
            return rse(no);
        }

        if (fb(no) > 1 && fb(no->dir) < 0 ){
          
            return rdd(no);
        }

        if (fb(no) < -1 && fb(no->dir) > 0) {
         
            return rde(no);
        }


    return no;

}




// FUNÇÕES GENÉRICAS =================|
Resultados avl_obter_resultados() {
    Resultados res;
    res.custo_insercao = custo_insercao;
    res.custo_remocao = custo_remocao;
    return res;
}

void avl_resetar_resultados() {
    custo_insercao = 0;
    custo_remocao = 0;
    operacao_atual = INSERCAO;  
}

void avl_inserir(void* arvore, int valor) {
    operacao_atual = INSERCAO;
    arvoreAVL* avl = (arvoreAVL*)arvore;
    avl->raiz = adiciona(avl->raiz, valor);
}

void avl_remover(void* arvore, int valor) {
    operacao_atual = REMOCAO;
    arvoreAVL* avl = (arvoreAVL*)arvore;
    avl->raiz = remover(avl->raiz, valor);
}

void* avl_criar() {
    return (void*)criaArv();
}

void avl_destruir(void* arvore) {
    arvoreAVL* avl = (arvoreAVL*)arvore;
    destroiAVL(avl->raiz);
    free(avl);
}

OperacoesArvore obter_operacoes_avl() {
    OperacoesArvore ops;
    ops.criar = avl_criar;
    ops.destruir = avl_destruir;
    ops.inserir = avl_inserir;
    ops.remover = avl_remover;
    ops.obter_resultados = avl_obter_resultados;
    ops.resetar = avl_resetar_resultados;
    return ops;
}