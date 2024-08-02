import sys
import matplotlib.pyplot as plt
import networkx as nx
from collections import defaultdict

def process_file(filename):
    blocks = defaultdict(int)
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith("  Bloco"):
                parts = line.split()
                try:
                    num_vertices = int(parts[2])
                    num_edges = int(parts[4])
                    blocks[(num_vertices, num_edges)] += 1
                except (IndexError, ValueError):
                    print(f"Formato inválido na linha: {line.strip()}")
    return blocks

def compare_blocks(file1_blocks, file2_blocks):
    for key, count in file1_blocks.items():
        if file2_blocks[key] < count:
            return False
        file2_blocks[key] -= count
    return all(count == 0 for count in file2_blocks.values())

def main():
    if len(sys.argv) != 3:
        print("Uso: python3 verifica.py resultado.out resultadoPy.out")
        sys.exit(1)
    
    file1 = sys.argv[1]
    file2 = sys.argv[2]

    file1_blocks = process_file(file1)
    file2_blocks = process_file(file2)

    if compare_blocks(file1_blocks, file2_blocks):
        print("Os blocos estão iguais.")
    else:
        print("Os blocos são diferentes.")


if __name__ == "__main__":
    main()
