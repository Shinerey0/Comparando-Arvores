#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "arvores.h"

#define MAX_N 100000 // Defina a QUANTIDADE máxima de ELEMENTOS aqui.
#define AMOSTRAS 100 // Defina a QUANTIDADE de ITERAÇÕES que serão feitas em cada passo realizado aqui (calculando a média consquentemente AM vezes);
#define PASSO 1000 //  // X pontos de N (1, 101, 201, ... 10000)
/* 
ARQUIVO MAIN PARA A ANÁLISE DE COMPLEXIDADE ALGORÍTMICA DAS OPERAÇÕES DE ADIÇÃO E REMOÇÃO

Este é nosso arquivo principal de testes, que contém as funções necessárias para o teste de custo de inserção e remoção dos elementos nas árvores determinadas
O teste foi feito a partir de uma interface genérica "arvore.h" por questões de facilitação e organização. 

Para contabilização de esforço computacional, consideramos, de acordo a proposta do professor Allan Rodrigo Leite, as seguintes características:
-Operações de balanceamento (rotações, splits, merges, redistribuições)
-Movimentação entre níveis → +1 para cada descida/avanço de nó durante busca/inserção/remoção
-Verificações se é necessário balanceamento ou não.
+ Há exceções na árvore B para tornar a comparação justa de acordo com estes critérios, onde contabilizamos a pesquisa no conjunto de chave dos nós.

A maioria dos testes realizados foi em minha máquina (comentado por Kelwin) com as seguintes configurações:
    Windows 10 Pro – Versão 22H2
    Build: 19045.6466
    Arquitetura: 64 bits

É importante comentar que também foram realizados testes em máquinas Linux Mint e Fedora (neste caso, por Emanuel), disponíveis tanto na universidade quanto em computador pessoal.
Dessa forma, é possível afirmar que a implementação não apresenta problemas de portabilidade entre diferentes sistemas operacionais.

O arquivo em Python para geração dos gráficos é complementar. 
A versão utilizada foi:
    Python 3.13.7
*/


// protótipos
void testes(OperacoesArvore ops, const char *output); 
void gerarGraficosBrutos();
void gerarGraficosRegressao();



int main() {

    srand(time(NULL)); 

    // Chamada de testes para árvore AVL
    testes(obter_operacoes_avl(), "AVL");

    // Chamada de testes para árvore Rubro Negra
    testes(obter_operacoes_rn(),"RN");

    // Chamada de testes para B-Tree de Ordem 1
    testes(obter_operacoes_b1(), "B1");

    // Chamada de testes para B-Tree Ordem 5
    testes(obter_operacoes_b5(), "B5");

    // Chamada de testes para B-Tree Ordem 10
    testes(obter_operacoes_b10(), "B10");


    // ESCOLHA QUAL TIPO DE GRÁFICO DESEJA PLOTAR!
    gerarGraficosBrutos();
    //gerarGraficosRegressao();

    
    return 0;
}

void testes(OperacoesArvore ops, const char *arquivo_saida_base) { // Função responsável de testes, feita de forma genérica.

    char arq_ins[256];
    char arq_rem[256];

    sprintf(arq_ins, "%s_insercao.csv", arquivo_saida_base);
    sprintf(arq_rem, "%s_remocao.csv",  arquivo_saida_base);

    FILE *f_ins = fopen(arq_ins, "w");
    FILE *f_rem = fopen(arq_rem, "w");

    fprintf(f_ins, "N,CustoInsercao\n");
    fprintf(f_rem, "N,CustoRemocao\n");

    int dados[MAX_N];

    printf("\nTestando: %s\n", arquivo_saida_base);

    for (int n = 1; n <= MAX_N; n += PASSO ) { // "n++" Defina o tamanho do passo aqui, para tornar o processo mais rápido opte por passos maiores como 100 ou 500.

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

       if ( (10* (n - 1)) % PASSO == 0){ // multiplo de passo - 1, pois n começa em 1,
            printf("N=%d concluído\n", n-1);
       }

    }

    fclose(f_ins);
    fclose(f_rem);

    printf("\nArquivos gerados:\n%s\n%s\n", arq_ins, arq_rem);
}

/*
    Chamada automática dos scripts Python responsáveis pela geração
    dos gráficos do experimento. O bloco utiliza diretivas de 
    pré-processamento (#ifdef / #else / #endif) para detectar o 
    sistema operacional durante a compilação.

    Os scripts Python usados para gerar os gráficos utilizam as 
    seguintes bibliotecas:
        matplotlib.pyplot: renderização dos gráficos
        csv: leitura dos arquivos CSV gerados pelo programa em C
        numpy: operações matemáticas e cálculo de regressão logarítmica

*/

void gerarGraficosBrutos() {
#ifdef _WIN32
    system("python plot_brutos.py");
#else
    system("python3 plot_brutos.py");
#endif
}

void gerarGraficosRegressao() {
#ifdef _WIN32
    system("python plot_regressao.py");
#else
    system("python3 plot_regressao.py");
#endif
}
