#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "arvores.h"

#define MAX_N 4000
#define AMOSTRAS 10

void testes(OperacoesArvore ops, const char *output);


int main() {

    srand(time(NULL));

    // AVL
    testes(obter_operacoes_avl(), "AVL");

    //  Rubro Negra
    testes(obter_operacoes_rn(),"RN");

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


