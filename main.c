#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "avl.h" 

// Arrumar a altura da AVL, tá ineficiente por ser recursiva, o ideal é usar o campo das struct (tinha feito que nem o professor)
// OBS: O teste para mais de 100 valores DEMORA MUITO, tem que dar uma olhada na otimização ou ver se é assim mesmo
// O meu código da AVL não está otimizado, fiz com base nos slides do professor e pode ter erros

#define MAX 100
//#include "testes.h" --> e colocar todas funções lá



// ======== PARTE DE TESTES
int* gerarDados(){

    int *vetorDados = malloc(sizeof(int) * MAX);
    for(int i = 0; i < MAX; i++){
        vetorDados[i] = rand(); // não sei se precisa limitar ou só ser aleatorios total msm
    }

    return vetorDados;
}

void media(int *v, int n, int reps){
    for(int i = 0; i < n; i++){
        v[i] = v[i] / reps;
    }
}


int* vetorTestesMedioADD(){

    int *vetorTestesADD = malloc(sizeof(int) * MAX);
    for(int i = 0; i < MAX; i++) vetorTestesADD[i] = 0; // inicializando o vetor, armazena as qtds de comparacoes de cada i, de 1 até 10k
    int reps;

    for(reps = 0; reps < 10; reps++){
  
        int *valores = gerarDados();

        // Para cada K de 1 até MAX (cada quantidade de valores dos testes), ou seja, de 1 até 10 mil
        for(int k = 1; k <= MAX; k++){

            arvoreAVL *a = criaArv();

            // teste para os primeiros K valores
            for(int j = 0; j < k; j++){
                        a->raiz = adiciona(a->raiz, valores[j], a);
                }

                vetorTestesADD[k-1] += a->comparacoes;

            destroiAVL(a->raiz); // liberando espaço pra proxima arvore com k valores
            free(a);

        }

        free(valores); // liberando espaço pros novos 10 mil
    }

    media(vetorTestesADD, MAX, reps);

    return vetorTestesADD;
}

void salvarArq(const char *arq, int *vetor){
    FILE *f;
    f = fopen(arq, "w");

    if(f == NULL){
        printf("\nErro ao criar o arquivo!\n");
    }

    fprintf(f, "tamanho n com k comparacoes\n");
    for(int i = 0; i < MAX; i++){
        fprintf(f,"%d\n", vetor[i]);
    }

    fclose(f);
}
// ======== PARTE DE TESTES


int main(){

    srand(time(NULL));
    int *testesAVL = vetorTestesMedioADD();
    salvarArq("avl_add.txt", testesAVL);
    free(testesAVL);


    /*
    arvoreAVL *a = criaArv();
    a->raiz = adiciona(a->raiz,4, a);
    a->raiz = adiciona(a->raiz,2, a);
    a->raiz = adiciona(a->raiz,1, a);
    a->raiz = adiciona(a->raiz,3, a);
    a->raiz = adiciona(a->raiz,8, a);
    a->raiz = adiciona(a->raiz,9, a);
    a->raiz = adiciona(a->raiz,6, a);
    a->raiz = adiciona(a->raiz,5, a);
    a->raiz = adiciona(a->raiz,7, a);

    a->raiz = remover(a->raiz, 7, a);
  
    printf("\nArvore: ");
    //preOrder(a->raiz);

    printArvore(a->raiz, 0, a);
    printf("\nComparacoes: %li\n", getComparacoesAVL(a));
    */
    return 0;
}