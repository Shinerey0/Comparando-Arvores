# Comparação de Árvores de Busca
Trabalho feito em **GRUPO** para a disciplina de Estruturas de Dados II, com os integrantes: KELWIN, EMANUEL, URIEL, LEONARDO E GUILHERME.

- Professor:  ALLAN RODRIGO LEITE
- UNIVERSIDADE ESTADUAL DE SANTA CATARINA - CENTRO DE CIÊNCIAS TECNOLÓGICAS

Este projeto implementa um experimento para avaliar o esforço computacional das operações de inserção e remoção em diferentes estruturas de árvores de busca:

- Árvore AVL  
- Árvore Rubro-Negra  
- Árvores B de ordem 1, 5 e 10  

O objetivo é medir o custo médio das operações quando aplicadas a conjuntos de dados aleatórios de tamanhos crescentes.  
Os resultados são armazenados em arquivos CSV e os gráficos são gerados através de scripts Python incluídos no projeto.

---

## Como compilar e executar

### Linux:
``gcc main.c avl.c rn.c b.c -o main``

``./main``

### Windows :
``gcc main.c avl.c rn.c b.c -o main.exe``

``main.exe``

---

## Dependências do Python

Os scripts utilizam:
- matplotlib
- numpy
- csv (biblioteca padrão)

### Instalação:
``pip install matplotlib numpy``

Para executar manualmente, use no terminal:

``python plot_brutos.py``

``python plot_regressao.py``

ou

``python3 plot_brutos.py``

``python3 plot_regressao.py``

--- 

## Parâmetros de Teste

Os parâmetros principais dos testes estão no início do arquivo main.c, definidos como constantes:

- MAX_N: valor máximo de N dados testados
- AMOSTRAS: número de repetições por valor de N
- PASSO: diferença entre os valores consecutivos de N avaliados

---

## Recomendação 

*(Comentado por Kelwin)*

Por fim, minha recomendação **pessoal**, é que dê uma olhada no código da MAIN e juntamente os comentários adicionados. É explicado como a contabilização dos custos foram feitas e outras considerações importantes para rodar essa implementação.





