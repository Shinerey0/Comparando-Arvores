import matplotlib.pyplot as plt
import csv

# Arquivo gerado pelo código em C
ARQUIVO = "AVL_remocao.csv"

# Listas para armazenar os dados
N = []
CUSTO = []

# Ler o arquivo CSV
with open(ARQUIVO, "r") as f:
    leitor = csv.reader(f)
    next(leitor)  # pular cabeçalho

    for linha in leitor:
        n = int(linha[0])
        custo = float(linha[1])

        N.append(n)
        CUSTO.append(custo)

# Criar gráfico
plt.figure(figsize=(10, 6))
plt.plot(N, CUSTO, linewidth=1)

plt.title("Custo Médio de Inserção - Árvore AVL")
plt.xlabel("Tamanho do conjunto de dados (N)")
plt.ylabel("Esforço computacional (custo_insercao)")
plt.grid(True)

# Salvar figura
plt.savefig("grafico_avl_insercao.png", dpi=300)

# Exibir figura
plt.show()
