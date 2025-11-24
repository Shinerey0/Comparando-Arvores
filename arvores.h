#ifndef ARVORES_H
#define ARVORES_H

typedef struct {
    long custo_insercao;
    long custo_remocao;
} Resultados;

typedef struct {
    void* (*criar)();
    void  (*destruir)(void*);
    void  (*inserir)(void*, int);
    void  (*remover)(void*, int);
    Resultados (*obter_resultados)();
    void (*resetar)();
} OperacoesArvore;

// Funções que cada implementação deve fornecer
OperacoesArvore obter_operacoes_avl();
OperacoesArvore obter_operacoes_rn();
OperacoesArvore obter_operacoes_b1();
OperacoesArvore obter_operacoes_b5();
OperacoesArvore obter_operacoes_b10();

#endif
