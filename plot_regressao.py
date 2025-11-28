import matplotlib.pyplot as plt
import numpy as np
import csv
import os

# Configurações
ARVORES = ["AVL", "RN", "B1", "B5", "B10"]

def ler_csv(nome_arquivo):
    N_list, Custo_list = [], []
    
    with open(nome_arquivo, "r") as f:
        leitor = csv.reader(f)
        next(leitor)  # pula cabeçalho
        
        for linha in leitor:
            if not linha or len(linha) < 2:
                continue
            N_list.append(int(linha[0]))
            Custo_list.append(float(linha[1]))
    
    return np.array(N_list), np.array(Custo_list)

def plotar_regressao(tipo_operacao):
    plt.figure(figsize=(12, 7))
    tem_dados = False # flag para saber se leu arquivo

    for arvore in ARVORES:
        nome_arquivo = f"{arvore}_{tipo_operacao}.csv"

        if not os.path.exists(nome_arquivo):
            print(f"[AVISO] Arquivo não encontrado: {nome_arquivo}")
            continue

        tem_dados = True
        N, custo = ler_csv(nome_arquivo)

        if len(N) > 0: # se arquivo não vazio
            # Regressão logaritmica: y = a * ln(x) + b
            a, b = np.polyfit(np.log(N), custo, 1)
            custo_fit = a * np.log(N) + b

            # Linha suave ajustada
            plt.plot(N, custo_fit, linewidth=2, label=f"{arvore}")


    if not tem_dados:
        return

    plt.title(f"Regressão Logarítmica do Custo Médio — {tipo_operacao.capitalize()}")
    plt.xlabel("Tamanho do conjunto (N)")
    plt.ylabel("Esforço Computacional (aprox. log N)")
    plt.grid(True, linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()

    saida = f"regressao_{tipo_operacao}.png"
    plt.savefig(saida, dpi=300)
    print(f"Gráfico salvo: {saida}")

# Gerar gráficos de inserção e remoção
plotar_regressao("insercao")
plotar_regressao("remocao")
