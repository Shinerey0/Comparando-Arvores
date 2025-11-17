#ifndef AVL_H
#define AVL_H

typedef struct node{
    struct node* pai;
    struct node* esq;
    struct node* dir;
    int altura;
    int valor;
} node;


typedef struct arvoreAVL{
    struct node* raiz;
    int tamanho;
    long comparacoes;
} arvoreAVL;

// Operações de criação e visualização da arvore AVL
arvoreAVL *criaArv();
void destroiAVL(node *a);
int vaziaArv(arvoreAVL *a);
node *criaNo(int valor);
int altura(node* no, arvoreAVL *a);
int maior(int a, int b);
int fb(node *no, arvoreAVL *a);
void preOrder(node *no);
long getComparacoesAVL(arvoreAVL *a);
//

// Operações de rotação 
node* rse(node* no, arvoreAVL *a);
node* rsd(node* no, arvoreAVL *a);
node* rde(node* no, arvoreAVL *a);
node* rdd(node* no, arvoreAVL *a);
//

// Operações de adição e remoção
node* adiciona(node *no, int valor, arvoreAVL *a);
node* remover(node *no, int chave, arvoreAVL *a);
//

void printArvore(node *no, int nivel, arvoreAVL *a);

#endif