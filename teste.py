import time
import random
import math
import subprocess
import os
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import statsmodels.api as sm 

# Assume-se que esta no mesmo sitio que isto
executavel = "proj2.cpp"
valoresnl = []
tempos = []


"""
Agarra nos dados do ficheiro test.in e permite chamar a função visual do networkx
"""
def ler_dados_arquivo(filename):
    with open(filename, 'r') as file:
        n, m, l = map(int, file.readline().strip().split())
        ligacoes = []
        for _ in range(m):
            x, y, linha = map(int, file.readline().strip().split())
            ligacoes.append((x, y, linha))
    return n, m, l, ligacoes


"""
Gera um grafo visual para o ficheiro test.in, apenas quando se quer executar um teste
"""
def gerar_grafo_visual(n, m, l, ligacoes):
    # Cria um grafo vazio
    G = nx.Graph()

    # Adiciona as estações como nós, exceto o vértice 0
    for i in range(1, n):
        G.add_node(i)

    # Adiciona as ligações entre as estações
    for x, y, linha in ligacoes:
        if x != 0 and y != 0:
            G.add_edge(x, y, linha=linha)

    # Cria um dicionário para armazenar as cores das linhas
    cores = {}
    colors = list(mcolors.CSS4_COLORS.values())
    random.shuffle(colors)
    for (u, v, linha) in G.edges(data='linha'):
        if linha not in cores:
            cores[linha] = colors.pop()  # Cor distinta
        G.edges[u, v]['color'] = cores[linha]

    # Obtém as cores para cada aresta
    edge_colors = [G.edges[u, v]['color'] for u, v in G.edges()]

    # Desenha o grafo
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels=True, edge_color=edge_colors, node_color='lightblue', node_size=900, width=4)
    plt.title("Rede de Metro")
    plt.show()


"""
Criar um ficheiro de teste com V vertices, E arestas e L ligações
Note-se que há uma pequena probabilidade de aparecer um vertice isolado
"""
def gerar_testes(V, E, L, S):
    # Compilar o gera.cpp se nao estiver compilado
    if not os.path.exists("./gera"):
        subprocess.run(["g++", "gera.cpp", "-o", "gera"])

    # Executar o programa para gerar o ficheiro e guardar em test.in
    with open("test.in", "w") as f:
        subprocess.run(["./gera", str(V), str(E), str(L), "1"], stdout=f, stderr=subprocess.DEVNULL)
    
    # Representar visualmente se quiser-se
    if S == True:
        n, m, l, ligacoes = ler_dados_arquivo("test.in")
        gerar_grafo_visual(n, m, l, ligacoes)


"""
Gerar diversos testes e correr o programa para cada um deles e guardar os tempos
para construir um gráfico
"""
def correr_teste():
    # Compilar o programa mesmo já compilado
    subprocess.run(["g++", "-std=c++11", "-O3", "-Wall", executavel, "-lm"])
    i = 1

    for n in range(0,100):
        n1 = 50000+(n+1)*5000
        m= 100000+(n+1)*10000
        l = int(100+20*math.log(n+1))

        gerar_testes(n1, m, l, False)

        with open("test.in", "r") as f:
            start = time.time()
            subprocess.run(["./" + "a.out"], stdin=f, stdout=subprocess.DEVNULL)
            end = time.time()

        if ((end-start) <= 1):
            print(f"Teste {i} com n={n1}, m={m} e l={l} demorou {end-start} segundos")
            i += 1

            valoresnl.append(m * l * math.log2(l))
            tempos.append(end - start)
        else:
            print("Explodiu!")
            continue

    # Após coletar os dados, realizar a regressão por segmentos
    realizar_regressao_polynomial(valoresnl, tempos)

"""
Realizar a regressão por segmentos dos tempos de execução
"""
def realizar_regressao_polynomial(valoresnl, tempos):
    plt.figure(figsize=(10,6))
    plt.scatter(valoresnl, tempos, label="Dados experimentais", alpha=0.6, color="blue", edgecolor="k")

    # Ajust a curve of degree 2
    degree = 2
    coef = np.polyfit(valoresnl, tempos, degree)
    poly_fn = np.poly1d(coef)

    # Plot the curve
    sorted_nml_values = sorted(valoresnl)
    plt.plot(sorted_nml_values, poly_fn(sorted_nml_values), '--', label="Tendência global", color="red")

    # Configurações do gráfico
    plt.xlabel("f(n,m,l) = m * l * log(l)", fontsize=12)
    plt.ylabel("Tempo (s)", fontsize=12)
    plt.title("Curva de Tendência para Tempo de Execução em Função de f(n,m,l)", fontsize=14)
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.show()


correr_teste()
