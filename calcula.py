import sys
import networkx as nx

def main():
    input_data = sys.stdin.read().strip().split()
    index = 0

    # Lê o número de vértices
    n = int(input_data[index])
    index += 1

    # Cria um grafo
    G = nx.Graph()

    # Adiciona todos os vértices
    G.add_nodes_from(range(1, n + 1))

    # Lê as arestas
    while index < len(input_data) - 1:
        u = int(input_data[index])
        v = int(input_data[index + 1])
        G.add_edge(u, v)
        index += 2

    # Encontra os componentes biconexos
    biconnected_components = list(nx.biconnected_components(G))

    # Adiciona vértices isolados como componentes biconexos
    isolated_vertices = [v for v in G.nodes() if G.degree(v) == 0]
    for v in isolated_vertices:
        biconnected_components.append({v})

    # Imprime o número de blocos
    print(f"O grafo consiste de {len(biconnected_components)} blocos")

    # Imprime cada bloco com a quantidade de vértices e arestas
    for i, component in enumerate(biconnected_components):
        subgraph = G.subgraph(component)
        num_vertices = subgraph.number_of_nodes()
        num_edges = subgraph.number_of_edges()
        print(f"  Bloco {i + 1:3d}: {num_vertices:4d} vértices, {num_edges:4d} arestas")

if __name__ == "__main__":
    main()
