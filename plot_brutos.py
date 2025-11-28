import matplotlib.pyplot as plt
import csv
import numpy as np
import os

ARVORES = ["AVL", "RN", "B1", "B5", "B10"]

def ler_csv(nome_arquivo):
    N_list, C_list = [], []
    with open(nome_arquivo, "r") as f:
        leitor = csv.reader(f)
        next(leitor)  # pula 1 linha/cabeçalho
        for linha in leitor:
            if len(linha) < 2:
                continue
            N_list.append(int(linha[0]))
            C_list.append(float(linha[1]))
    return np.array(N_list), np.array(C_list) #converte para np array para ser melhor em gráfico

def plotar_dados_brutos(operacao):
    plt.figure(figsize=(12, 7))

    for arv in ARVORES:
        nome_arquivo = f"{arv}_{operacao}.csv"

        if not os.path.exists(nome_arquivo):
            print(f"[Aviso] Arquivo não encontrado: {nome_arquivo}")
            continue

        N, custo = ler_csv(nome_arquivo)

        # linha mais suave
        plt.plot(N, custo, linewidth=1.3, label=arv)

    plt.title(f"Comparação de Custo Médio — {operacao.capitalize()}")
    plt.xlabel("Tamanho do conjunto (N)")
    plt.ylabel("Esforço computacional (custo)")
    plt.grid(True, linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()

    saida = f"comparacao_{operacao}.png"
    plt.savefig(saida, dpi=300)
    print(f"Gráfico salvo: {saida}")

    plt.show()

# -------------------------------
# Executar os gráficos
# -------------------------------
plotar_dados_brutos("insercao")
plotar_dados_brutos("remocao")
