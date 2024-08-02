import random

def main():
    n = 10**3   
    k = random.randint(1, n * (n - 1) // n)
    edges = set()
    while len(edges) < k:
        u = random.randint(1, n)
        v = random.choice([i for i in range(1, n) if i != u])
        if u != v:
            edges.add((min(u, v), max(u, v)))

    print(n)
    for edge in edges:
        print(edge[0], edge[1])

if __name__ == "__main__":
    main()
