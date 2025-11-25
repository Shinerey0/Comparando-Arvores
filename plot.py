import matplotlib.pyplot as plt
import csv
import os

# Arvores que serao comparadas
ARVORES = ["AVL", "RN", "B1", "B5", "B10"]

# Operacoes que serao mostradas
OPERACOES = ["insercao", "remocao"]

# Pasta onde estão os arquivos CSV (ou "" se estiverem no mesmo diretório)
PASTA = ""


def ler_csv(caminho):
    N, Custo = [], []
    with open(caminho, "r") as f:
        leitor = csv.reader(f)
        next(leitor)  # pula cabeçalho
        for linha in leitor:
            if not linha or len(linha) < 2:  
                continue
            N.append(int(linha[0]))
            Custo.append(float(linha[1]))
    return N, Custo



# --------------------------
#   GRÁFICO DE INSERÇÃO
# --------------------------
plt.figure(figsize=(12, 7))

for arvore in ARVORES:
    nome_arquivo = f"{arvore}_insercao.csv"
    caminho = os.path.join(PASTA, nome_arquivo)

    if not os.path.exists(caminho):
        print(f"[AVISO] Arquivo não encontrado: {nome_arquivo}")
        continue

    N, custo = ler_csv(caminho)

    # PRINT DETALHADO
    #print(f"\nLendo {nome_arquivo}:")
    #print("N =", N)
    #print("Custo =", custo)

    plt.plot(N, custo, linewidth=1, label=arvore)

plt.title("Comparação de Custo Médio — Inserção")
plt.xlabel("Tamanho do conjunto de dados (N)")
plt.ylabel("Esforço computacional (custo)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("comparacao_insercao.png", dpi=300)
plt.show(block=False)


# --------------------------
#   GRÁFICO DE REMOÇÃO
# --------------------------
plt.figure(figsize=(12, 7))

for arvore in ARVORES:
    nome_arquivo = f"{arvore}_remocao.csv"
    caminho = os.path.join(PASTA, nome_arquivo)

    if not os.path.exists(caminho):
        print(f"[AVISO] Arquivo não encontrado: {nome_arquivo}")
        continue

    N, custo = ler_csv(caminho)

    #print(f"\nLendo {nome_arquivo}:")
    #print("N =", N)
    #print("Custo =", custo)

    plt.plot(N, custo, linewidth=1, label=arvore)

plt.title("Comparação de Custo Médio — Remoção")
plt.xlabel("Tamanho do conjunto de dados (N)")
plt.ylabel("Esforço computacional (custo)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("comparacao_remocao.png", dpi=300)
plt.show()
