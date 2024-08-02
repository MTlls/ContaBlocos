O trabalho captura a entrada em stdin e retorna uma struct info_t com o número de arestas e blocos de cada bloco. Além de alterar o valor de num_blocos para ser o número de blocos do grafo como especificado no enunciado.

Foi implementado o algoritmo de Tarjan com algumas modificações para resolver o problema de contar os blocos e uma pilha durante a recursão para armazenar os vértices visitados.

É usado 3 structs: vertex_t, neighbor_t e stack_t.

A struct vertex_t é usada para representar um vértice do grafo, contém os seguintes campos:
typedef struct vertex { 		// a vertex in the graph
	struct vertex *parent;   	// parent vertex in the spanning tree
	struct list *neighbors;  	// list of neighbors

	unsigned short state;      	// vertex state
	unsigned int level;        	// level of the vertex
	unsigned int lowpoint;     	// lowpoint of the vertex
	unsigned int return_edges;	// number of return edges of the block
} vertex_t;

Há campos comumente usados em listas de adjacencia, como uma lista ligada de vizinhos, mas é interessante salientar que foi aproveitado a struct vertex_t: além de ser um vértice do grafo em si, é um vértice de uma árvore geradora (usado no algoritmo de Tarjan), possuindo um ponteiro para o vértice pai na árvore, um estado, um nível, um lowpoint e um número de arestas de retorno.

Usa-se um vetor de vertex_t para representar o grafo em si, já que contém a lista de vizinhos, a próxima struct.

A struct neighbor_t é usada para representar um vizinho de um vértice, contém os seguintes campos:

typedef struct list {	// a list of neighbors
	vertex_t *self;     // pointer to the vertex
	struct list *next;  // pointer to the next vertex
} neighbor_t;

Devido ao aproveitamento da struct vertex_t, não foi usado a mesma strutura para representar os vizinhos. Apesar disso aproveita-se do ponteiro para o vértice vizinho, podendo acessar os campos da struct vertex_t sem muitos problemas. Como é uma lista ligada, também é usado um ponteiro para o próximo vizinho.

Por fim, a struct stack_t é usada para representar uma pilha de vértices, contém os seguintes campos:

typedef struct stack { 		// a stack of vertices
	struct vertex **stack;	// the stack
	unsigned int top; 		// the top of the stack
} stack_t;

A pilha é usada para armazenar os vértices visitados durante a recursão do algoritmo de Tarjan. A pilha é implementada como um vetor de ponteiros para vértices e um inteiro para representar o índice do topo da pilha.

Como é implementado o algoritmo de tarjan, ele acha todos os vértices de corte. Com as modificações feitas, o mesmo conta o número de blocos, seus vértices e arestas. Para isso, é usado a pilha para armazenar os vértices visitados (a cada descida): cada vez que se acha uma articulação, desempilha-se a pilha até encontrar o filho da articulação na árvore geradora (é desempilhado o filho também) e incrementa-se o número de blocos.
É importante salientar que mesmo o último vértice do bloco sendo a articulação, o mesmo não é retirado da pilha, pois o mesmo pode ser contabilizado em outro bloco. 

Vale comentar que o trabalho foi em ingles (não sei se isso é relevante para a avaliação, mas achei importante mencionar)