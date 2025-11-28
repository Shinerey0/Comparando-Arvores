#ifndef ARVORES_H
#define ARVORES_H

/*
Interface genérica para os testes das árvores e contabilização de forma ideal
*/

typedef struct {
    long custo_insercao;
    long custo_remocao;
} Resultados;

typedef struct { // tipo void* para receber qualquer tipo de arvore 
    void* (*criar)();
    void  (*destruir)(void*);
    void  (*inserir)(void*, int);
    void  (*remover)(void*, int);
    Resultados (*obter_resultados)();
    void (*resetar)();
} OperacoesArvore;

// Funções que cada implementação deve fornecer, cada elemento da struct definido com a função ESPECÍFICA da arvore.
OperacoesArvore obter_operacoes_avl();
OperacoesArvore obter_operacoes_rn();
OperacoesArvore obter_operacoes_b1();
OperacoesArvore obter_operacoes_b5();
OperacoesArvore obter_operacoes_b10();

#endif
