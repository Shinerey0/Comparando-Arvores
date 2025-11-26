#include "arvores.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_N 4000
#define AMOSTRAS 50

typedef enum { INSERCAO, REMOCAO } Operacao;
long b_custo_insercao = 0;   // DEFINIÇÃO
long b_custo_remocao = 0;    // DEFINIÇÃO
Operacao operacao_atual;   // DEFINIÇÃO

void testes(OperacoesArvore ops, const char *output);


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

// Contador de operações
long long contadorOperacoes = 0;

/////////////   ///// //PROTÓTIPOS /////////////////////////////
No* criaNo(ArvoreB* arvore);
void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave);
///////////     ///// //FUNÇÕES ///////////////////////////

void contaCusto(int vezes) {
    if (operacao_atual == INSERCAO)
        b_custo_insercao += vezes;
    else
        b_custo_remocao += vezes;
}




// Criação da árvore e nós
ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);
    return a;
}
//criação do nó
No* criaNo(ArvoreB* arvore) {
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
        contaCusto(1);
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
No* localizaNo(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;
    while (no != NULL) {
        contaCusto(1);
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
    
    contaCusto(no->total - i);
    
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    no->total++;
}

// Verifica transbordo
int transbordo(ArvoreB *arvore, No *no) {
    return no->total > arvore->ordem * 2;
}

// Divide o nó
No* divideNo(ArvoreB* arvore, No* no) {
    contaCusto(1);
    int meio = no->total / 2;
    No* novo = criaNo(arvore);
    novo->pai = no->pai;
    for (int i = meio + 1; i < no->total; i++) {
    
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL)
            novo->filhos[novo->total]->pai = novo;
        novo->total++;
    }
    contaCusto(no->total - meio);
    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL)
        novo->filhos[novo->total]->pai = novo;
    no->total = meio;
    return novo;
}
//

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo_filho_direita, int chave) {

    contaCusto(1);;
    adicionaChaveNo(no, novo_filho_direita, chave); 
    
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        
        
        novo_filho_direita = divideNo(arvore, no); 
        
        if (no->pai == NULL) {
            No* raiz = criaNo(arvore);
            raiz->filhos[0] = no;
            adicionaChaveNo(raiz, novo_filho_direita, promovido);
            no->pai = raiz;
            novo_filho_direita->pai = raiz;
            arvore->raiz = raiz;
        } else {
            
            adicionaChaveRecursivo(arvore, no->pai, novo_filho_direita, promovido);
        }
    }
}
// Inserção principal (depende da recursiva)
void adicionaChave(ArvoreB* arvore, int chave) {
    contaCusto(1);
    No* no = localizaNo(arvore, chave);
    adicionaChaveRecursivo(arvore, no, NULL, chave);
}

// Remoção apenas de folhas
void removeChave(ArvoreB* arvore, int chave) {
    contaCusto(1);
    No *no = arvore->raiz;
    
    // Busca a chave
    while (no != NULL) {
        
        int i = pesquisaBinaria(no, chave);
        
        // Ao encontrar a chave em uma folha, remove-se
        if (i < no->total && no->chaves[i] == chave && no->filhos[0] == NULL) {
            for (int j = i; j < no->total - 1; j++) {
                
                no->chaves[j] = no->chaves[j + 1];
            }
            contaCusto(no->total - i - 1);
            no->total--;
            return;
        }
        
        // Continua a busca
        if (no->filhos[i] != NULL)
            no = no->filhos[i];
        else
            return; // Não encontrou a chave buscada
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

// Gera chaves aleatórias
void geraChavesAleatorias(int* chaves, int n) {
    for (int i = 0; i < n; i++) {
        chaves[i] = rand() % (n * 10);
    }
}



///////////////////FUNÇõES GENÉRICAS//////////////

Resultados b1_obter_resultados() {
    Resultados res;
    res.custo_insercao = custo_insercao;
    res.custo_remocao = custo_remocao;
    return res;
}

void b1_resetar_resultados() {
    custo_insercao = 0;
    custo_remocao = 0;
    operacao_atual = INSERCAO;  
}

void b1_inserir(void* arvore, int valor) {
    operacao_atual = INSERCAO;
    ArvoreB* b = (ArvoreB*) arvore;
    b->ordem = 1;
    adicionaChave(b, valor);
}

void b1_remover(void* arvore, int valor) {
    operacao_atual = REMOCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    b->ordem = 1;
    removeChave(b, valor);
}



ArvoreB* b1_criar() {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = 1;
  
    a->raiz = criaNo(a);
    printf("////");
    return a;
}

void b1_destruir(void *arv) {
    ArvoreB *b = (ArvoreB*) arv;
    liberaArvore(b);
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

////////////////// 5 ////////////////////////

Resultados b5_obter_resultados() {
    Resultados res;
    res.custo_insercao = custo_insercao;
    res.custo_remocao = custo_remocao;
    return res;
}

void b5_resetar_resultados() {
    custo_insercao = 0;
    custo_remocao = 0;
    operacao_atual = INSERCAO;  
}

void b5_inserir(void* arvore, int valor) {
    operacao_atual = INSERCAO;
    ArvoreB* b = (ArvoreB*) arvore;
    b->ordem = 5;
    adicionaChave(b, valor);
}

void b5_remover(void* arvore, int valor) {
    operacao_atual = REMOCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    b->ordem = 5;
    removeChave(b, valor);
}



ArvoreB* b5_criar() {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = 5;
  
    a->raiz = criaNo(a);
    printf("////");
    return a;
}

void b5_destruir(void *arv) {
    ArvoreB *b = (ArvoreB*) arv;
    liberaArvore(b);
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
////////// 10 ///////////////////////

Resultados b10_obter_resultados() {
    Resultados res;
    res.custo_insercao = custo_insercao;
    res.custo_remocao = custo_remocao;
    return res;
}

void b10_resetar_resultados() {
    custo_insercao = 0;
    custo_remocao = 0;
    operacao_atual = INSERCAO;  
}

void b10_inserir(void* arvore, int valor) {
    operacao_atual = INSERCAO;
    ArvoreB* b = (ArvoreB*) arvore;
    b->ordem = 10;
    adicionaChave(b, valor);
}

void b10_remover(void* arvore, int valor) {
    operacao_atual = REMOCAO;
    ArvoreB* b = (ArvoreB*)arvore;
    b->ordem = 10;
    removeChave(b, valor);
}



ArvoreB* b10_criar() {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = 10;
  
    a->raiz = criaNo(a);
    printf("////");
    return a;
}

void b10_destruir(void *arv) {
    ArvoreB *b = (ArvoreB*) arv;
    liberaArvore(b);
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


int main() {

    srand(time(NULL));

    // AVL
    // testes(obter_operacoes_avl(), "AVL");

    // //  Rubro Negra
    // testes(obter_operacoes_rn(),"RN");
    
    testes(obter_operacoes_b1(),"B1");
    
    testes(obter_operacoes_b5(),"B5");
     
    testes(obter_operacoes_b10(),"B10");

    #ifdef _WIN32
        system("python plot.py");
    #else
        system("python3 plot.py");
    #endif
    
    return 0;
}

void testes(OperacoesArvore ops, const char *arquivo_saida_base) {

    char arq_ins[256];
    char arq_rem[256];

    sprintf(arq_ins, "%s_insercao.csv", arquivo_saida_base); // salva uma string formatada em arq_ins, tava tentando usar strcat e dava erro
    sprintf(arq_rem, "%s_remocao.csv",  arquivo_saida_base);

    FILE *f_ins = fopen(arq_ins, "w");
    FILE *f_rem = fopen(arq_rem, "w");

    fprintf(f_ins, "N,CustoInsercao\n");
    fprintf(f_rem, "N,CustoRemocao\n");

    int dados[MAX_N];

    printf("\nTestando: %s\n", arquivo_saida_base);

    for (int n = 1; n <= MAX_N; n++) {

        double soma_ins = 0;
        double soma_rem = 0;

        for (int am = 0; am < AMOSTRAS; am++) {

            // gera MAX_N dados aleatórios
            for (int i = 0; i < n; i++)
                dados[i] = rand();
                
            void *arv = ops.criar();

            // INSERIR n-1 elementos
            for (int i = 0; i < n - 1; i++)
                ops.inserir(arv, dados[i]);

          
            // INSERÇÃO DO ULTIMO
        
            ops.resetar(); 
           
            ops.inserir(arv, dados[n - 1]);
          

            Resultados r_ins = ops.obter_resultados();
            soma_ins += r_ins.custo_insercao;

          
            // REMOÇÃO DO ULTIMO
         
            ops.resetar();
            ops.remover(arv, dados[n - 1]);

            Resultados r_rem = ops.obter_resultados();
            soma_rem += r_rem.custo_remocao;

            ops.destruir(arv);
        }

        double media_ins = soma_ins / AMOSTRAS;
        double media_rem = soma_rem / AMOSTRAS;

        fprintf(f_ins, "%d,%.2f\n", n, media_ins);
        fprintf(f_rem, "%d,%.2f\n", n, media_rem);

        if (n % 500 == 0)
            printf("N=%d concluído\n", n);
    }

    fclose(f_ins);
    fclose(f_rem);

    printf("\nArquivos gerados:\n%s\n%s\n", arq_ins, arq_rem);
}
