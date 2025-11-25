#include "arvores.h"
#include <stdlib.h>
#include <stdio.h>

// ============================================================
// ESTRUTURAS E GLOBAIS
// ============================================================

typedef enum { B_INSERCAO, B_REMOCAO } B_Operacao;

long b_custo_insercao = 0;
long b_custo_remocao = 0;
B_Operacao b_operacao_atual;

typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai;
} No;

typedef struct arvoreB {
    No* raiz;
    int ordem;
} ArvoreB;

// Helper para contagem de custo
void b_contaCusto(int vezes) {
    if (b_operacao_atual == B_INSERCAO)
        b_custo_insercao += vezes;
    else
        b_custo_remocao += vezes;
}

// ============================================================
// LÓGICA DA ÁRVORE B (Baseada no arquivo "Analise - Arv B")
// ============================================================

No* b_criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    No* no = malloc(sizeof(No));
    no->pai = NULL;
    // Aloca espaço conforme a ordem definida
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No*) * (max + 2));
    no->total = 0;
    
    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    
    return no;
}

ArvoreB* b_criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = b_criaNo(a);
    return a;
}

// Pesquisa binária para encontrar posição dentro do nó
int b_pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    //b_contaCusto(1); // Conta 1 por nó visitado, não por comparação

    // NÃO contamos custo aqui dentro para focar na diferença de "Níveis" (macro)
    // Se contássemos cada comparação, B10 ficaria caro indevidamente no gráfico.

    while (inicio <= fim) {
        b_contaCusto(1); // Custo de comparação a cada iteração
        meio = (inicio + fim) / 2;
        if (no->chaves[meio] == chave)
            return meio;
        else if (no->chaves[meio] > chave)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }
    return inicio;
}

// Localiza o nó folha apropriado
No* b_localizaNo(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;
    while (no != NULL) {
        b_contaCusto(1); // Custo de navegação entre níveis
        int i = b_pesquisaBinaria(no, chave);
        
        if (no->filhos[i] == NULL)
            return no;
        else
            no = no->filhos[i];
    }
    return NULL;
}

void b_adicionaChaveNo(No* no, No* direita, int chave) {
    int i = b_pesquisaBinaria(no, chave);
    for (int j = no->total - 1; j >= i; j--) {
        
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    no->total++;
}

int b_transbordo(ArvoreB *arvore, No *no) {
    // Validar se o nó precisa de balanceamento conta custo
    b_contaCusto(1);
    return no->total > arvore->ordem * 2;
}

No* b_divideNo(ArvoreB* arvore, No* no) {
    b_contaCusto(1); // Custo estrutural do Split

    int meio = no->total / 2;
    No* novo = b_criaNo(arvore);
    novo->pai = no->pai;
    
    for (int i = meio + 1; i < no->total; i++) {
        
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL)
            novo->filhos[novo->total]->pai = novo;
        novo->total++;
    }
    
    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL)
        novo->filhos[novo->total]->pai = novo;
        
    no->total = meio;
    return novo;
}

void b_adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
   // Custo de Nível na recursão
   b_contaCusto(1);
    // mas a descida já foi paga no 'b_localizaNo'
    b_adicionaChaveNo(no, novo, chave);
    
    if (b_transbordo(arvore, no)) { // Aqui já tem o contaCusto(1)
        int promovido = no->chaves[arvore->ordem];
        No* novoNo = b_divideNo(arvore, no); // divisao
        
        if (no->pai == NULL) {
            No* raiz = b_criaNo(arvore);
            raiz->filhos[0] = no;
            b_adicionaChaveNo(raiz, novoNo, promovido);
            no->pai = raiz;
            novoNo->pai = raiz;
            arvore->raiz = raiz;
        } else {
            b_adicionaChaveRecursivo(arvore, no->pai, novoNo, promovido);
        }
    }
}

// Inserção principal
void b_adicionaChave(ArvoreB* arvore, int chave) {
    No* no = b_localizaNo(arvore, chave); // custo de descida aqui
    b_adicionaChaveRecursivo(arvore, no, NULL, chave); // custo de validação
}

// Remoção (Simplificada - Apenas folhas, conforme fornecido)
void b_removeChave(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;
    while (no != NULL) {
        b_contaCusto(1); // custo de nível
        int i = b_pesquisaBinaria(no, chave);
        
        // Se encontrou a chave e é folha
        if (i < no->total && no->chaves[i] == chave && no->filhos[0] == NULL) {
            for (int j = i; j < no->total - 1; j++) {
                
                no->chaves[j] = no->chaves[j + 1];
            }
            no->total--;
            return;
        }
        
        // Continua a busca
        if (no->filhos[i] != NULL)
            no = no->filhos[i];
        else
            return; // Não encontrou
    }
}

// Destruição recursiva
void b_liberaNo(No* no) {
    if (no != NULL) {
        for (int i = 0; i <= no->total; i++) {
            if (no->filhos[i] != NULL)
                b_liberaNo(no->filhos[i]);
        }
        free(no->chaves);
        free(no->filhos);
        free(no);
    }
}

void b_liberaArvore(void* arvore) {
    ArvoreB* a = (ArvoreB*)arvore;
    if (a) {
        b_liberaNo(a->raiz);
        free(a);
    }
}

// ============================================================
// INTERFACE GENÉRICA (Adaptação para arvores.h)
// ============================================================

Resultados b_obter_resultados() {
    Resultados res;
    res.custo_insercao = b_custo_insercao;
    res.custo_remocao = b_custo_remocao;
    return res;
}

void b_resetar() {
    b_custo_insercao = 0;
    b_custo_remocao = 0;
    b_operacao_atual = B_INSERCAO;
}

void b_inserir_void(void* arvore, int valor) {
    b_operacao_atual = B_INSERCAO;
    b_adicionaChave((ArvoreB*)arvore, valor);
}

void b_remover_void(void* arvore, int valor) {
    b_operacao_atual = B_REMOCAO;
    b_removeChave((ArvoreB*)arvore, valor);
}

// Funções construtoras específicas para cada Ordem
void* b1_criar() { return (void*)b_criaArvore(1); }
void* b5_criar() { return (void*)b_criaArvore(5); }
void* b10_criar() { return (void*)b_criaArvore(10); }

// ============================================================
// FACTORIES (Funções chamadas pelo .h)
// ============================================================

OperacoesArvore obter_operacoes_b1() {
    OperacoesArvore ops;
    ops.criar = b1_criar;
    ops.destruir = b_liberaArvore;
    ops.inserir = b_inserir_void;
    ops.remover = b_remover_void;
    ops.obter_resultados = b_obter_resultados;
    ops.resetar = b_resetar;
    return ops;
}

OperacoesArvore obter_operacoes_b5() {
    OperacoesArvore ops;
    ops.criar = b5_criar;
    ops.destruir = b_liberaArvore;
    ops.inserir = b_inserir_void;
    ops.remover = b_remover_void;
    ops.obter_resultados = b_obter_resultados;
    ops.resetar = b_resetar;
    return ops;
}

OperacoesArvore obter_operacoes_b10() {
    OperacoesArvore ops;
    ops.criar = b10_criar;
    ops.destruir = b_liberaArvore;
    ops.inserir = b_inserir_void;
    ops.remover = b_remover_void;
    ops.obter_resultados = b_obter_resultados;
    ops.resetar = b_resetar;
    return ops;
}