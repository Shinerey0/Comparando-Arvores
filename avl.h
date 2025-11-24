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
} arvoreAVL;

// Operações de criação e visualização da arvore AVL
arvoreAVL *criaArv();
void destroiAVL(node *a);
int vaziaArv(arvoreAVL *a);
node *criaNo(int valor);
int getAltura(node* no);
void atualizarAltura(node* no);
int maior(int a, int b);

int fb(node *no);
void preOrder(node *no);
void printArvore(node *no, int nivel, arvoreAVL *a);
//

// Operações de rotação 
node* rse(node* no);
node* rsd(node* no);
node* rde(node* no);
node* rdd(node* no);
//

// Operações de adição e remoção
node* adiciona(node *no, int valor);
node* remover(node *no, int chave);
//

// Funções de Custo
// DECLARAR VARIÁVEIS GLOBAIS PARA CUSTO 
extern long custo_insercao;
extern long custo_remocao;

// Declara o enum e a variável global 
typedef enum { INSERCAO, REMOCAO } Operacao;
extern Operacao operacao_atual;

// Declara a função que incrementa o custo */
void contaCusto(int vezes);

void printArvore(node *no, int nivel, arvoreAVL *a);

#endif