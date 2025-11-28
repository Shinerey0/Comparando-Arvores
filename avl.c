#include "arvores.h"
#include <stdlib.h>
#include <stdio.h>

// Variáveis especiais para testes e contabilização!
typedef enum { INSERCAO, REMOCAO } Operacao;
long custo_insercao = 0;  
long custo_remocao = 0;   
Operacao operacao_atual; 

/*
Para a implementação da árvore usamos, em grande parte, a base fornecida pelo professor durante as aulas.

Novamente, consideramos para contabilização de esforço computacional:
-Operações de balanceamento (rotações, splits, merges, redistribuições)
-Movimentação entre níveis → +1 para cada descida/avanço de nó durante busca/inserção/remoção
-Verificações se é necessário balanceamento ou não.

Desta forma, na árvore AVL foram contabilizados:
    Operações de rotação simples (à esquerda e à direita)
    Avanços em níveis, como na recursão da inserção e remoção, busca de sucessores.
    Verificações de balanceamento, como na verificação de necessidade de rotação a partir dos fatores de balanceamento

*/

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

int maior(int a, int b){
    if(a>b){
        return a;
    } else{
        return b;
    }
}

void contaCusto(int vezes) { // FUNÇÃO RESPONSÁVEL PARA CONTABILIZAÇÃO! ELA SERÁ CHAMADA A CADA 'CUSTO' CONSIDERADO NA IMPLEMENTAÇÃO!
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
    novo->altura = 1;
    novo->valor = valor;
    return novo;
}

int getAltura(node* no){
    if(no == NULL){
        return 0;
    }
    return no->altura;
}

void atualizarAltura(node* no){
    if(no == NULL){
        return;
    }
    int alturaEsq = getAltura(no->esq);
    int alturaDir = getAltura(no->dir);
    no->altura = maior(alturaEsq, alturaDir) + 1;
}

int fb(node* no) {
    if (no == NULL){
        return 0;
    }
    return getAltura(no->esq) - getAltura(no->dir);
}

node* rse(node* no){
    contaCusto(1); // conta custo, operação de balanceamento (rotação)
    node* pai = no->pai;
    node* direita = no->dir;

    no->dir = direita->esq;
    no->pai = direita;

    if (no->dir != NULL) {
        no->dir->pai = no;
    }

    direita->esq = no;
    direita->pai = pai;

    atualizarAltura(no);
    atualizarAltura(direita);

    return direita;
}

node* rsd(node* no){
    contaCusto(1); // conta custo, operação de balanceamento (rotação)
    node* pai = no->pai;
    node* esquerda = no->esq;  

    no->esq = esquerda->dir;
    no->pai = esquerda;

    if (no->esq != NULL) {
        no->esq->pai = no;
    }
    
    esquerda->dir = no;
    esquerda->pai = pai;

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
    contaCusto(1);  // avanço de nível recursivo
    
    if (no == NULL){
        return criaNo(valor);
    }

    if (valor < no->valor){
        no->esq = adiciona(no->esq, valor);
        no->esq->pai = no;
    } else if(valor > no->valor){
        no->dir = adiciona(no->dir, valor);
        no->dir->pai = no; 
    } else{
        return no;
    }

    atualizarAltura(no);
    
   
    // CONTABILIZAR VERIFICAÇÕES DE BALANCEAMENTO 
    contaCusto(1); // uma verificação de balanceamento
    if (fb(no) > 1 && fb(no->esq) >= 0){ 
        return rsd(no);
    }
    contaCusto(1); // foram duas verificações
    if (fb(no) < -1 && fb(no->dir) <= 0 ){
        return rse(no);
    }
    contaCusto(1); // foram tres verificações
    if (fb(no) > 1 && fb(no->dir) < 0 ){
        return rdd(no);
    }
    contaCusto(1); // foram quatro verificações
    if (fb(no) < -1 && fb(no->dir) > 0) {
        return rde(no);
    }

    return no;
}

node* remover(node *no, int chave){
    contaCusto(1); // avanço de nível
    
    if(no == NULL){
        //printf("\nValor nao encontrado para remocao\n");
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
        if(no->esq == NULL && no->dir == NULL){
            free(no);
            return NULL;
        }

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

        // CASO 3: DOIS FILHOS
        node *temp = no->dir;
        while(temp->esq != NULL){
            contaCusto(1); // avanço de nível na busca do sucessor
            temp = temp->esq;
        }

        no->valor = temp->valor;
        no->dir = remover(no->dir, temp->valor);
        if (no->dir != NULL){
            no->dir->pai = no; 
        }
    }

    if(no == NULL){
        return NULL;
    }

    atualizarAltura(no);

    // CONTABILIZAR VERIFICAÇÕES DE BALANCEAMENTO 
   
    contaCusto(1); // uma verificação de balanceamento
    if (fb(no) > 1 && fb(no->esq) >= 0){ 
        return rsd(no);
    }
    contaCusto(1); // foram duas verificações
    if (fb(no) < -1 && fb(no->dir) <= 0 ){
        return rse(no);
    }
    contaCusto(1); // foram tres verificações
    if (fb(no) > 1 && fb(no->dir) < 0 ){
        return rdd(no);
    }
    contaCusto(1); // foram quatro verificações
    if (fb(no) < -1 && fb(no->dir) > 0) {
        return rde(no);
    }

    return no;
}

// CRIAÇÃO DAS FUNÇÕES PARA ADAPTAÇÃO NA ESTRUTURA GENÉRICA! É FEITO A CONVERSÃO DA FUNÇÃO GENÉRICA PARA A FUNÇÃO ESPECÍFICA DA ÁRVORE.

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