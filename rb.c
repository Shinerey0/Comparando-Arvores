#include <stdio.h>
#include <stdlib.h>
#include "arvores.h"

/*
https://computerscience360.wordpress.com/wp-content/uploads/2018/02/algoritmos-teoria-e-prc3a1tica-3ed-thomas-cormen.pdf
https://www.ulysseso.com/livros/ed2/ApF.pdf

*/

// Definições para a árvore rubro-negra
#define RED 1
#define BLACK 0


long rn_custo_insercao = 0;
long rn_custo_remocao = 0;
typedef enum { RN_INSERCAO, RN_REMOCAO } RN_Operacao;
RN_Operacao rn_operacao_atual;


void rn_contaCusto(int vezes) {
    if (rn_operacao_atual == RN_INSERCAO)
        rn_custo_insercao += vezes;
    else
        rn_custo_remocao += vezes;
}

typedef struct No {
    int valor;
    int cor;
    struct No *esq, *dir, *pai;
} No;

No *NIL;

typedef struct {
    No* raiz;
} ArvoreRN;

No* criarNo(int valor) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->cor = RED;
    novoNo->esq = novoNo->dir = novoNo->pai = NIL;
    return novoNo;
}

void initNIL() {
    static int inicializado = 0;
    if(!inicializado){
        NIL = criarNo(0);
        NIL->cor = BLACK;
        inicializado = 1;
    }
}

// prefixo rn_ nas funções de rotação
void rn_esqRotate(No** raiz, No* x) {
    rn_contaCusto(1); 
    No* y = x->dir;
    x->dir = y->esq;
    if (y->esq != NIL) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL) *raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rn_dirRotate(No** raiz, No* y) {
    rn_contaCusto(1); 
    No* x = y->esq;
    y->esq = x->dir;
    if (x->dir != NIL) x->dir->pai = y;
    x->pai = y->pai;
    if (y->pai == NIL) *raiz = x;
    else if (y == y->pai->esq) y->pai->esq = x;
    else y->pai->dir = x;
    x->dir = y;
    y->pai = x;
}

// prefixo rn_ na função
void rn_corrigirInsercao(No** raiz, No* z) {
    while (z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esq) {
            No* y = z->pai->pai->dir;
            if (y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rn_esqRotate(raiz, z); 
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                rn_dirRotate(raiz, z->pai->pai); 
            }
        } else {
            No* y = z->pai->pai->esq;
            if (y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rn_dirRotate(raiz, z); 
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                rn_esqRotate(raiz, z->pai->pai); 
            }
        }
    }
    (*raiz)->cor = BLACK;
}

// prefixo rn_ na função
void rn_inserir(No** raiz, int valor) {
    rn_contaCusto(1); 
    No* z = criarNo(valor);
    No* y = NIL;
    No* x = *raiz;
    while (x != NIL) {
        rn_contaCusto(1); 
        y = x;
        if (z->valor < x->valor) x = x->esq;
        else x = x->dir;
    }
    z->pai = y;
    if (y == NIL) *raiz = z;
    else if (z->valor < y->valor) y->esq = z;
    else y->dir = z;
    if (z->pai == NIL) {
        z->cor = BLACK;
        return;
    }
    if (z->pai->pai == NIL) return;
    rn_corrigirInsercao(raiz, z); 
}

// prefixo rn_ na função
void rn_transplant(No** raiz, No* u, No* v) {
    if (u->pai == NIL) *raiz = v;
    else if (u == u->pai->esq) u->pai->esq = v;
    else u->pai->dir = v;
    v->pai = u->pai;
}

// prefixo rn_ na função
No* rn_minimo(No* x) {
    while (x->esq != NIL) {
        rn_contaCusto(1); 
        x = x->esq;
    }
    return x;
}

// prefixo rn_ na função
void rn_corrigirRemocao(No** raiz, No* x) {
    while (x != *raiz && x->cor == BLACK) {
        if (x == x->pai->esq) {
            No* w = x->pai->dir;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                rn_esqRotate(raiz, x->pai); 
                w = x->pai->dir;
            }
            if (w->esq->cor == BLACK && w->dir->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->dir->cor == BLACK) {
                    w->esq->cor = BLACK;
                    w->cor = RED;
                    rn_dirRotate(raiz, w); 
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->dir->cor = BLACK;
                rn_esqRotate(raiz, x->pai); 
                x = *raiz;
            }
        } else {
            No* w = x->pai->esq;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                rn_dirRotate(raiz, x->pai); 
                w = x->pai->esq;
            }
            if (w->dir->cor == BLACK && w->esq->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq->cor == BLACK) {
                    w->dir->cor = BLACK;
                    w->cor = RED;
                    rn_esqRotate(raiz, w); 
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->esq->cor = BLACK;
                rn_dirRotate(raiz, x->pai); 
                x = *raiz;
            }
        }
    }
    x->cor = BLACK;
}

// prefixo rn_ na função 
void rn_remover(No** raiz, int valor) {
    No* z = *raiz;
    while (z != NIL && z->valor != valor) {
        rn_contaCusto(1); 
        if (valor < z->valor) z = z->esq;
        else z = z->dir;
    }
    if (z == NIL) return;
    No* y = z;
    No* x;
    int y_original_cor = y->cor;
    if (z->esq == NIL) {
        x = z->dir;
        rn_transplant(raiz, z, z->dir); 
    } else if (z->dir == NIL) {
        x = z->esq;
        rn_transplant(raiz, z, z->esq); 
    } else {
        y = rn_minimo(z->dir); 
        y_original_cor = y->cor;
        x = y->dir;
        if (y->pai == z) x->pai = y;
        else {
            rn_transplant(raiz, y, y->dir); 
            y->dir = z->dir;
            y->dir->pai = y;
        }
        rn_transplant(raiz, z, y); 
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    free(z);
    if (y_original_cor == BLACK) rn_corrigirRemocao(raiz, x); 
}

// Funções de interfaces genéricas
ArvoreRN* rn_criar() {
    initNIL();
    ArvoreRN* arv = (ArvoreRN*)malloc(sizeof(ArvoreRN));
    arv->raiz = NIL;
    return arv;
}

void rn_destruir_no(No* no) {
    if (no == NIL) return;
    rn_destruir_no(no->esq);
    rn_destruir_no(no->dir);
    free(no);
}

void rn_destruir(void* arvore) {
    ArvoreRN* rn = (ArvoreRN*)arvore;
    rn_destruir_no(rn->raiz);
    free(rn);
}

void rn_inserir2(void* arvore, int valor) {
    rn_operacao_atual = RN_INSERCAO; 
    ArvoreRN* rn = (ArvoreRN*)arvore;
    rn_inserir(&rn->raiz, valor); 
}

void rn_remover2(void* arvore, int valor) {
    rn_operacao_atual = RN_REMOCAO; 
    ArvoreRN* rn = (ArvoreRN*)arvore;
    rn_remover(&rn->raiz, valor); 
}

Resultados rn_obter_resultados() {
    Resultados res;
    res.custo_insercao = rn_custo_insercao; 
    res.custo_remocao = rn_custo_remocao; 
    return res;
}

void rn_resetar() {
    rn_custo_insercao = 0; 
    rn_custo_remocao = 0; 
    rn_operacao_atual = RN_INSERCAO; 
}

OperacoesArvore obter_operacoes_rn() {
    OperacoesArvore ops;
    ops.criar = (void* (*)())rn_criar; 
    ops.destruir = rn_destruir;
    ops.inserir = rn_inserir2;
    ops.remover = rn_remover2;
    ops.obter_resultados = rn_obter_resultados;
    ops.resetar = rn_resetar;
    return ops;
}