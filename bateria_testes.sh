#!/bin/bash

# Número de iterações
n=$1

# Verifica se o número de iterações foi fornecido
if [ -z "$n" ]; then
    echo "Uso: $0 <n>"
    exit 1
fi

# Verifica se o arquivo 'verifica.py' existe
if [ ! -f verifica.py ]; then
    echo "O arquivo 'verifica.py' não foi encontrado."
    exit 1
fi

# Loop para executar o processo n vezes
for i in $(seq 1 $n); do
    echo "Executando iteração $i..."

    # Executa o script gera.py para gerar um arquivo de entrada
    python3 gera.py > in/input$i.txt

    # Executa o programa em C para cada
    ./src/teste < in/input$i.txt > out/c_$i.out

    # Executa o programa em Python
    python3 calcula.py < in/input$i.txt > out/py_$i.out

    # Compara os resultados
    python3 verifica.py out/c_$i.out out/py_$i.out 
done
