#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definições para a árvore rubro-negra
#define RED 1
#define BLACK 0
#define CONJUNTOS 10

typedef struct No {
    int valor;
    int cor;
    struct No *esq, *dir, *pai;
} No;

No *NIL; // Nó sentinela

// Contadores globais para rotações (esforço computacional)
long long inseridos;
long long excluidos;

// Função para criar um novo nó
No* criarNo(int valor) {
    No* No = (No*)malloc(sizeof(No));
    No->valor = valor;
    No->cor = RED;
    No->esq = No->dir = No->pai = NIL;
    return No;
}

// Inicializar NIL
void initNIL() {
    NIL = criarNo(0);
    NIL->cor = BLACK;
}

// Rotações
void esqRotate(No** raiz, No* x) {
    inseridos++; // Conta rotação para inserção
    excluidos++; // Conta rotação para remoção (se aplicável)
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

void dirRotate(No** raiz, No* y) {
    inseridos++; // Conta rotação para inserção
    excluidos++; // Conta rotação para remoção (se aplicável)
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

// Correção após inserção
void corrigirInsercao(No** raiz, No* z) {
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
                    esqRotate(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                dirRotate(raiz, z->pai->pai);
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
                    dirRotate(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                esqRotate(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = BLACK;
}

void inserir(No** raiz, int valor) {
    No* z = criarNo(valor);
    No* y = NIL;
    No* x = *raiz;
    while (x != NIL) {
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
    corrigirInsercao(raiz, z);
}

// Substituir nó
void transplant(No** raiz, No* u, No* v) {
    if (u->pai == NIL) *raiz = v;
    else if (u == u->pai->esq) u->pai->esq = v;
    else u->pai->dir = v;
    v->pai = u->pai;
}

No* minimo(No* x) {
    while (x->esq != NIL) x = x->esq;
    return x;
}

// Correção após remoção
void corrigirRemocao(No** raiz, No* x) {
    while (x != *raiz && x->cor == BLACK) {
        if (x == x->pai->esq) {
            No* w = x->pai->dir;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                esqRotate(raiz, x->pai);
                w = x->pai->dir;
            }
            if (w->esq->cor == BLACK && w->dir->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->dir->cor == BLACK) {
                    w->esq->cor = BLACK;
                    w->cor = RED;
                    dirRotate(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->dir->cor = BLACK;
                esqRotate(raiz, x->pai);
                x = *raiz;
            }
        } else {
            No* w = x->pai->esq;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                dirRotate(raiz, x->pai);
                w = x->pai->esq;
            }
            if (w->dir->cor == BLACK && w->esq->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq->cor == BLACK) {
                    w->dir->cor = BLACK;
                    w->cor = RED;
                    esqRotate(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->esq->cor = BLACK;
                dirRotate(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    x->cor = BLACK;
}

void remover(No** raiz, int valor) {
    No* z = *raiz;
    while (z != NIL && z->valor != valor) {
        if (valor < z->valor) z = z->esq;
        else z = z->dir;
    }
    if (z == NIL) return; // Chave não encontrada
    No* y = z;
    No* x;
    int y_original_cor = y->cor;
    if (z->esq == NIL) {
        x = z->dir;
        transplant(raiz, z, z->dir);
    } else if (z->dir == NIL) {
        x = z->esq;
        transplant(raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        y_original_cor = y->cor;
        x = y->dir;
        if (y->pai == z) x->pai = y;
        else {
            transplant(raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplant(raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    free(z);
    if (y_original_cor == BLACK) corrigirRemocao(raiz, x);
}

// Função para gerar chaves únicas aleatórias
void gerarValores(int* valores, int n) {
    int* usado = (int*)calloc(100000, sizeof(int)); // Assumindo chaves de 0 a 99999
    for (int i = 0; i < n; i++) {
        int valor;
        do {
            valor = rand() % 100000;
        } while (usado[valor]);
        usado[valor] = 1;
        valores[i] = valor;
    }
    free(usado);
}

int main() {
    srand(time(NULL));
    initNIL();
    
    printf("Tamanho,Média_Inserção_Rotações,Média_Remocão_Rotações\n");
    
    for (int n = 1; n <= 10000; n += 100) { // Incremento de 100 para acelerar; ajuste para 1 se necessário
        long long totalInseridos = 0;
        long long totalRemovidos = 0;
        
        for (int s = 0; s < CONJUNTOS; s++) {
            No* raiz = NIL;
            int* valores = (int*)malloc(n * sizeof(int));
            gerarValores(valores, n);
            
            // Inserção
            inseridos = 0;
            for (int i = 0; i < n; i++) {
                inserir(&raiz, valores[i]);
            }
            totalInseridos += inseridos;
            
            // Remoção (na mesma ordem)
            excluidos = 0;
            for (int i = 0; i < n; i++) {
                remover(&raiz, valores[i]);
            }
            totalRemovidos += excluidos;
            
            free(valores);
            // Liberar árvore
        }
        
        double mediaInseridos = (double)totalInseridos / CONJUNTOS;
        double mediaRemovidos = (double)totalRemovidos / CONJUNTOS;
        printf("%d,%.2f,%.2f\n", n, mediaInseridos, mediaRemovidos);
    }
    
    return 0;
}
