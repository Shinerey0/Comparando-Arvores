#include <stdio.h>
#include <stdlib.h>
#include "arvores.h"


/*

Para a implementação da árvore usamos, em grande parte, a base fornecida pelo professor durante as aulas.
E algumas informações adicionais no trecho do livro encontrado publicamente:https://computerscience360.wordpress.com/wp-content/uploads/2018/02/algoritmos-teoria-e-prc3a1tica-3ed-thomas-cormen.pdf

Novamente, consideramos para contabilização de esforço computacional:
-Operações de balanceamento (rotações, splits, merges, redistribuições)
-Movimentação entre níveis → +1 para cada descida/avanço de nó durante busca/inserção/remoção
-Verificações se é necessário balanceamento ou não.

Desta forma, na árvore B foram contabilizados:
    Operações de balanceamento, como splits (divisões), merges e redistribuições.
    Avanço em níveis, ao subir ou descer níveis na inserção e remoção recursiva, localização de nós, encontrar mínimo.
    Verificações de balanceamento, nesse caso overflow e underflow, balanceamento em raízes e verificações de necessidade de split ou merge.

    + Exceção: A pesquisa binária de chaves um nó foi contabilizada para tornar a comparação mais justa. 
    + Como contabilizamos como métrica o avanço em níveis, isso beneficia a árvore B, dessa forma foi tomado a decisão de contabilizar a pesquisa.
    + Essa contabilização não existe nas árvores binárias por não haver multi-chave no nó
    
*/


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

// Variáveis globais para contagem
typedef enum { INSERCAO, REMOCAO } Operacao;
long b_custo_insercao = 0;
long b_custo_remocao = 0;
Operacao b_operacao_atual;

// Declarações antecipadas
No* b_criaNo(ArvoreB* arvore);
void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave);

// Função de contagem - OTIMIZADA
void b_contaCusto(int vezes) {
    if (b_operacao_atual == INSERCAO)
        b_custo_insercao += vezes;
    else
        b_custo_remocao += vezes;
}

// Criação da árvore e nós
ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = b_criaNo(a);
    return a;
}

No* b_criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    No* no = malloc(sizeof(No));
    no->pai = NULL;
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No*) * (max + 2));
    no->total = 0;
    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    return no;
}

// Pesquisa binária 
int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
  
    while (inicio <= fim) {
       b_contaCusto(1); // contando cada percurso no nó em busca da chave, isso penaliza a arvore B (consequentemente quem tem mais chave piora)
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


No* localizaNo(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;
    while (no != NULL) {
        b_contaCusto(1); // MOVIMENTO ENTRE NÍVEIS
        int i = pesquisaBinaria(no, chave);
        if (no->filhos[i] == NULL)
            return no;
        else
            no = no->filhos[i];
    }
    return NULL;
}

// Adiciona chave no nó
void adicionaChaveNo(No* no, No* direita, int chave) {
   
    int i = pesquisaBinaria(no, chave);
    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    no->total++;
}

// Retorna se há um transbordo ou não, como NÃO É UMA VERIFICAÇÃO, não é contabilizado. Somente é uma função que retorna 0 ou 1.
int transbordo(ArvoreB *arvore, No *no) {
    return no->total > arvore->ordem * 2;
}

// Divide o nó 
No* divideNo(ArvoreB* arvore, No* no) {
     b_contaCusto(1); //operação de split
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

// Inserção recursiva 
void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {

    adicionaChaveNo(no, novo, chave);

    b_contaCusto(1); // verificação de transbordo
    if (transbordo(arvore, no)) {
        
        int promovido = no->chaves[arvore->ordem];
        No* novo = divideNo(arvore, no);
        
        b_contaCusto(1); //  verificação de Balanceamento na raiz
        if (no->pai == NULL) {
            No* raiz = b_criaNo(arvore);
            raiz->filhos[0] = no;
            adicionaChaveNo(raiz, novo, promovido);
            no->pai = raiz;
            novo->pai = raiz;
            arvore->raiz = raiz;
        } else {
            // subiu um nivel (nó promovido no split)
            b_contaCusto(1);
            adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
        }
    }
}

// Inserção principal
void adicionaChave(ArvoreB* arvore, int chave) {
    No* no = localizaNo(arvore, chave);
    adicionaChaveRecursivo(arvore, no, NULL, chave);
}


// FUNÇÕES DE REMOÇÃO 
// ==========================================================

// Encontra o predecessor (maior chave da subárvore à esquerda)
int getPredecessor(ArvoreB* arvore, No* no, int idx) {
    No* cur = no->filhos[idx];
    while (cur->filhos[cur->total] != NULL) {
        b_contaCusto(1); // Custo de descer um nível
        cur = cur->filhos[cur->total];
    }
    return cur->chaves[cur->total - 1];
}

// Encontra o sucessor (menor chave da subárvore à direita)
int getSucessor(ArvoreB* arvore, No* no, int idx) {
    No* cur = no->filhos[idx + 1];
    while (cur->filhos[0] != NULL) {
        b_contaCusto(1); // Custo de descer um nível
        cur = cur->filhos[0];
    }
    return cur->chaves[0];
}

// Remove chave de um nó folha
void removeDeFolha(No* no, int idx) {
    for (int i = idx + 1; i < no->total; ++i) {
        // Desloca para trás
        no->chaves[i - 1] = no->chaves[i];
    }
    no->total--;
}

// Pega emprestado do irmão anterior (Esquerda)
void emprestaDoAnterior(No* no, int idx) {
    No* filho = no->filhos[idx];
    No* irmao = no->filhos[idx - 1];

    // Abre espaço no início do filho
    for (int i = filho->total - 1; i >= 0; --i)
        filho->chaves[i + 1] = filho->chaves[i];

    if (filho->filhos[0] != NULL) {
        for (int i = filho->total; i >= 0; --i)
            filho->filhos[i + 1] = filho->filhos[i];
    }

    // A chave do pai desce para o filho
    filho->chaves[0] = no->chaves[idx - 1];
    
    // O último filho do irmão passa a ser o primeiro do filho atual
    if (no->filhos[0] != NULL) {
        filho->filhos[0] = irmao->filhos[irmao->total];
        if(filho->filhos[0]) filho->filhos[0]->pai = filho;
    }

    // A maior chave do irmão sobe para o pai
    no->chaves[idx - 1] = irmao->chaves[irmao->total - 1];

    filho->total++;
    irmao->total--;
    b_contaCusto(1); 
}

// Pega emprestado do irmão seguinte (Direita)
void emprestaDoProximo(No* no, int idx) {
    No* filho = no->filhos[idx];
    No* irmao = no->filhos[idx + 1];

    // A chave do pai desce para o final do filho
    filho->chaves[filho->total] = no->chaves[idx];

    // O primeiro filho do irmão passa a ser o último do filho atual
    if (irmao->filhos[0] != NULL) {
        filho->filhos[filho->total + 1] = irmao->filhos[0];
        if(filho->filhos[filho->total + 1]) 
            filho->filhos[filho->total + 1]->pai = filho;
    }

    // A menor chave do irmão sobe para o pai
    no->chaves[idx] = irmao->chaves[0];

    // Remove a chave e o ponteiro movidos do irmão
    for (int i = 1; i < irmao->total; ++i)
        irmao->chaves[i - 1] = irmao->chaves[i];

    if (irmao->filhos[0] != NULL) {
        for (int i = 1; i <= irmao->total; ++i)
            irmao->filhos[i - 1] = irmao->filhos[i];
    }

    filho->total++;
    irmao->total--;
   b_contaCusto(1); 
}

// Faz a fusão (Merge) de dois filhos e a chave do pai
void mergeNos(ArvoreB* arvore, No* no, int idx) {
    b_contaCusto(1); 
    No* filho = no->filhos[idx];
    No* irmao = no->filhos[idx + 1];

    // Desce a chave do pai para o meio
    filho->chaves[filho->total] = no->chaves[idx];
    filho->total++;

    // Copia chaves do irmão para o filho
    for (int i = 0; i < irmao->total; ++i) {
        filho->chaves[filho->total + i] = irmao->chaves[i];
    }

    // Copia ponteiros de filhos do irmão (se não for folha)
    if (filho->filhos[0] != NULL) {
        for (int i = 0; i <= irmao->total; ++i) {
            filho->filhos[filho->total + i] = irmao->filhos[i];
            if (filho->filhos[filho->total + i]) 
                filho->filhos[filho->total + i]->pai = filho;
        }
    }
    
    filho->total += irmao->total;

    // Remove a chave usada do pai e arrasta ponteiros no pai
    for (int i = idx + 1; i < no->total; ++i)
        no->chaves[i - 1] = no->chaves[i];

    for (int i = idx + 2; i <= no->total; ++i)
        no->filhos[i - 1] = no->filhos[i];

    no->total--;

    // Libera memória do irmão
    free(irmao->chaves);
    free(irmao->filhos);
    free(irmao);
}

// Garante que o filho tenha chaves suficientes (> ordem) antes de descer
void garantirFilho(ArvoreB* arvore, No* no, int idx) {
    // Se tem exatamente 'ordem' chaves, não podemos tirar nenhuma
    // Precisamos que tenha mais que a ordem
    b_contaCusto(1); // verifica de balanceamento, se o filho já tem chaves suficientes
    if (no->filhos[idx]->total > arvore->ordem) {
        return;
    }

    // Tenta irmão esquerdo
    b_contaCusto(1); // verificação 2 de balanceamento
    if (idx != 0 && no->filhos[idx - 1]->total > arvore->ordem) {
        emprestaDoAnterior(no, idx);
    }
    // Tenta irmão direito
    else{ 
        b_contaCusto(1); // verificação 3 de balanceamento
        if (idx != no->total && no->filhos[idx + 1]->total > arvore->ordem) {
        emprestaDoProximo(no, idx);
    }
    // Se não der, merge
    else {
        if (idx != no->total){
            mergeNos(arvore, no, idx);
        } else{
            mergeNos(arvore, no, idx - 1);
        }
            
    }

    }
}


void removeChaveRecursivo(ArvoreB* arvore, No* no, int chave) {

    int idx = pesquisaBinaria(no, chave);
    int encontrou = (idx < no->total && no->chaves[idx] == chave);

    if (encontrou) {
        if (no->filhos[0] == NULL) {
            // Caso 1: Folha
            removeDeFolha(no, idx);
        } else {
            // Caso 2: Nó interno
            No* esq = no->filhos[idx];
            No* dir = no->filhos[idx + 1];

            // Precisamos que o filho tenha > ordem para ceder uma chave
            b_contaCusto(1); // Verifica necessidade de balanceamento, pegar da esquerda, direita ou merge
            if (esq->total > arvore->ordem) {
                int pred = getPredecessor(arvore, no, idx);
                no->chaves[idx] = pred;
                b_contaCusto(1); // avançou nivel recursivamente
                removeChaveRecursivo(arvore, esq, pred);
            } else{ if (dir->total > arvore->ordem) {
                    int suc = getSucessor(arvore, no, idx);
                    no->chaves[idx] = suc;
                    b_contaCusto(1); // avançou nivel recursivamente
                    removeChaveRecursivo(arvore, dir, suc);
                } else {
                    // Se ambos estão no limite, merge
                    mergeNos(arvore, no, idx);
                    // A chave desceu para o filho resultante do merge
                    b_contaCusto(1); // avançou nivel recursivamente
                    removeChaveRecursivo(arvore, no->filhos[idx], chave);
                }
            }
        }
    } else {
        // Chave não está aqui
        if (no->filhos[0] == NULL) return; // Não existe na árvore

        // Caso 3: Preparar filho para descida (Underflow preventivo)
        garantirFilho(arvore, no, idx);

        // Se houve merge nos últimos dois filhos, o índice pode ter mudado
        if (idx > no->total){
            b_contaCusto(1); // avançou nivel recursivamente
            removeChaveRecursivo(arvore, no->filhos[idx - 1], chave);
        }else{
            b_contaCusto(1); // avançou nivel recursivamente
            removeChaveRecursivo(arvore, no->filhos[idx], chave);
        }
    }
}

void removeChave(ArvoreB* arvore, int chave) {
    if (arvore->raiz == NULL) return;

    removeChaveRecursivo(arvore, arvore->raiz, chave);

    // Tratamento da Raiz após remoção
    if (arvore->raiz->total == 0) {
        if (arvore->raiz->filhos[0] != NULL) {
            // A raiz ficou vazia mas tem filhos (fusão ocorreu na raiz)
            // O primeiro filho vira a nova raiz (reduz altura)
            No* tmp = arvore->raiz;
            arvore->raiz = arvore->raiz->filhos[0];
            arvore->raiz->pai = NULL;
            
            free(tmp->chaves);
            free(tmp->filhos);
            free(tmp);
        } else {
            // A árvore ficou totalmente vazia (raiz era folha e perdeu última chave)
            arvore->raiz->total = 0;
        }
    }
}


// Libera memória
void liberaNo(No* no) {
    if (no != NULL) {
        for (int i = 0; i <= no->total; i++) {
            if (no->filhos[i] != NULL)
                liberaNo(no->filhos[i]);
        }
        free(no->chaves);
        free(no->filhos);
        free(no);
    }
}

void liberaArvore(ArvoreB* arvore) {
    liberaNo(arvore->raiz);
    free(arvore);
}

// CRIAÇÃO DAS FUNÇÕES PARA ADAPTAÇÃO NA ESTRUTURA GENÉRICA! É FEITO A CONVERSÃO DA FUNÇÃO GENÉRICA PARA A FUNÇÃO ESPECÍFICA DA ÁRVORE.

// ============== FUNÇÕES PARA ORDEM 1 ==============

void* b1_criar() {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = 1;
    a->raiz = b_criaNo(a);
    return a;
}

void b1_inserir(void* arvore, int valor) {
    b_operacao_atual = INSERCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    adicionaChave(b, valor);
}

void b1_remover(void* arvore, int valor) {
    b_operacao_atual = REMOCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    removeChave(b, valor);
}

void b1_destruir(void* arv) {
    ArvoreB* b = (ArvoreB*)arv;
    liberaArvore(b);
}

void b1_resetar_resultados() {
    b_custo_insercao = 0;
    b_custo_remocao = 0;
    b_operacao_atual = INSERCAO;
}

Resultados b1_obter_resultados() {
    Resultados res;
    res.custo_insercao = b_custo_insercao;
    res.custo_remocao = b_custo_remocao;
    return res;
}

OperacoesArvore obter_operacoes_b1() {
    OperacoesArvore ops;
    ops.criar = (void* (*)())b1_criar;
    ops.destruir = b1_destruir;
    ops.inserir = b1_inserir;
    ops.remover = b1_remover;
    ops.obter_resultados = b1_obter_resultados;
    ops.resetar = b1_resetar_resultados;
    return ops;
}

// ============== FUNÇÕES PARA ORDEM 5 ==============

void* b5_criar() {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = 5;
    a->raiz = b_criaNo(a);
    return a;
}

void b5_inserir(void* arvore, int valor) {
    b_operacao_atual = INSERCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    adicionaChave(b, valor);
}

void b5_remover(void* arvore, int valor) {
    b_operacao_atual = REMOCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    removeChave(b, valor);
}

void b5_destruir(void* arv) {
    ArvoreB* b = (ArvoreB*)arv;
    liberaArvore(b);
}

void b5_resetar_resultados() {
    b_custo_insercao = 0;
    b_custo_remocao = 0;
    b_operacao_atual = INSERCAO;
}

Resultados b5_obter_resultados() {
    Resultados res;
    res.custo_insercao = b_custo_insercao;
    res.custo_remocao = b_custo_remocao;
    return res;
}

OperacoesArvore obter_operacoes_b5() {
    OperacoesArvore ops;
    ops.criar = (void* (*)())b5_criar;
    ops.destruir = b5_destruir;
    ops.inserir = b5_inserir;
    ops.remover = b5_remover;
    ops.obter_resultados = b5_obter_resultados;
    ops.resetar = b5_resetar_resultados;
    return ops;
}

// ============== FUNÇÕES PARA ORDEM 10 ==============

void* b10_criar() {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = 10;
    a->raiz = b_criaNo(a);
    return a;
}

void b10_inserir(void* arvore, int valor) {
    b_operacao_atual = INSERCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    adicionaChave(b, valor);
}

void b10_remover(void* arvore, int valor) {
    b_operacao_atual = REMOCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    removeChave(b, valor);
}

void b10_destruir(void* arv) {
    ArvoreB* b = (ArvoreB*)arv;
    liberaArvore(b);
}

void b10_resetar_resultados() {
    b_custo_insercao = 0;
    b_custo_remocao = 0;
    b_operacao_atual = INSERCAO;
}

Resultados b10_obter_resultados() {
    Resultados res;
    res.custo_insercao = b_custo_insercao;
    res.custo_remocao = b_custo_remocao;
    return res;
}

OperacoesArvore obter_operacoes_b10() {
    OperacoesArvore ops;
    ops.criar = (void* (*)())b10_criar;
    ops.destruir = b10_destruir;
    ops.inserir = b10_inserir;
    ops.remover = b10_remover;
    ops.obter_resultados = b10_obter_resultados;
    ops.resetar = b10_resetar_resultados;
    return ops;
}
