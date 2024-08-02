#include "blocos.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct vertex { 		// a vertex in the graph
	struct vertex *parent;   	// parent vertex in the spanning tree
	struct list *neighbors;  	// list of neighbors

	unsigned short state;      	// vertex state
	unsigned int level;        	// level of the vertex
	unsigned int lowpoint;     	// lowpoint of the vertex
	unsigned int return_edges;	// number of return edges of the block
} vertex_t;

typedef struct list {	// a list of neighbors
	vertex_t *self;     // pointer to the vertex
	struct list *next;  // pointer to the next vertex
} neighbor_t;

typedef struct stack { 		// a stack of vertices
	struct vertex **stack;	// the stack
	unsigned int top; 		// the top of the stack
} stack_t;

// Creates a list of vertices
vertex_t *create_vertex_list(unsigned int n);
// Adds an edge to the adjacency list
void add_edge(vertex_t *vertices, unsigned int a, unsigned int b);
// Prints the adjacency list
void print_adjacency_list(vertex_t *vertices, unsigned int n);
// Frees the memory allocated for the vertex list
void free_vertex_list(vertex_t *vertices, unsigned int n);
// The same lowpoint(G, r) shown in class
void tarjan(vertex_t *vertices, vertex_t *r, info_t *blocks, unsigned int *num_blocos, stack_t *stack);
// The same lowpoint(G) shown in class
void calc_all_lowpoints(vertex_t *vertices, unsigned int n, info_t *blocks, unsigned int *num_blocos);
// Print each level and lowpoint of each vertex
void print_levels(vertex_t *vertices, unsigned int n);
// Creates a list of info_t, with n blocks, which is the worst case scenario
info_t *create_info(unsigned int n);

// Create a stack of vertices
stack_t *create_stack(unsigned int n);
// Free the memory allocated for the stack
void free_stack(stack_t *stack);
// Push a item to a stack of vertices
void push(stack_t *stack, vertex_t *v);
// Get the top item from a stack of vertices
vertex_t *top(stack_t *stack);
// Pop a item from a stack of vertices
vertex_t *pop(stack_t *stack);
// Verify if the stack is empty
unsigned int is_empty(stack_t *stack);

// Takes the stack and pop the top element until the top element is the child of the parent vertex, which is the cut vertex (add the cut vertex to the block but doesnt pop it)
void make_block(vertex_t *parent, stack_t *stack, info_t *blocks, unsigned int *num_blocos);

stack_t *create_stack(unsigned int n) {
	stack_t *stack = (stack_t *)calloc(1, sizeof(stack_t));
	stack->stack = (vertex_t **)calloc(2 * (n - 1), sizeof(vertex_t *));
	stack->top = 0;
	return stack;
}

void free_stack(stack_t *stack) {
	free(stack->stack);
	free(stack);
}

void push(stack_t *stack, vertex_t *v) {
	stack->stack[stack->top] = v;
	stack->top++;
}

vertex_t *pop(stack_t *stack) {
	stack->top--;
	return stack->stack[stack->top];
}

vertex_t *top(stack_t *stack) {
	return stack->stack[stack->top - 1];
}

unsigned int is_empty(stack_t *stack) {
	return stack->top == 0;
}

info_t *create_info(unsigned int n) {
	info_t *blocks;
	if((blocks = (info_t *)calloc(n, sizeof(info_t))) == NULL) {
		perror("ERROR: malloc");
		return NULL;
	}
	for(unsigned int i = 0; i < n; i++) {
		blocks[i].vertices = 0;
		blocks[i].arestas = 0;
	}
	return blocks;
}

vertex_t *create_vertex_list(unsigned int n) {
	vertex_t *v = (vertex_t *)calloc(n, sizeof(vertex_t));
	for(unsigned int i = 0; i < n; i++) {
		v[i].state = 0;
		v[i].parent = NULL;
		v[i].level = 0;
		v[i].neighbors = NULL;
		v[i].lowpoint = 0;
		v[i].return_edges = 0;
	}
	return v;
}

void add_edge(vertex_t *vertices, unsigned int a, unsigned int b) {
	// Add b to the list of neighbors of a
	neighbor_t *new_neighbor = (neighbor_t *)calloc(1, sizeof(neighbor_t));
	new_neighbor->self = &vertices[b - 1];
	new_neighbor->next = vertices[a - 1].neighbors;
	vertices[a - 1].neighbors = new_neighbor;

	// Add a to the list of neighbors of b
	new_neighbor = (neighbor_t *)calloc(1, sizeof(neighbor_t));
	new_neighbor->self = &vertices[a - 1];
	new_neighbor->next = vertices[b - 1].neighbors;
	vertices[b - 1].neighbors = new_neighbor;
}

void print_adjacency_list(vertex_t *vertices, unsigned int n) {
	for(unsigned int i = 0; i < n; i++) {
		printf("Vertex %u:", i + 1);
		neighbor_t *current = vertices[i].neighbors;
		while(current != NULL) {
			printf(" %lu", (current->self - vertices) + 1);
			current = current->next;
		}
		printf("\n");
	}
}

void free_vertex_list(vertex_t *vertices, unsigned int n) {
	for(unsigned int i = 0; i < n; i++) {
		neighbor_t *current = vertices[i].neighbors;
		while(current != NULL) {
			neighbor_t *to_free = current;
			current = current->next;
			free(to_free);
		}
	}
	free(vertices);
}

void make_block(vertex_t *parent, stack_t *stack, info_t *blocks, unsigned int *num_blocos) {
	vertex_t *v = pop(stack);

	// the parent is a cut vertex, we repeat until the top of the stack is the child of the parent
	while(v->parent != parent) {
		blocks[*num_blocos].vertices++;
		blocks[*num_blocos].arestas += v->return_edges;
		blocks[*num_blocos].arestas++;
		v = pop(stack);
	}

	// the last vertex before the parent
	blocks[*num_blocos].vertices++;
	blocks[*num_blocos].arestas += v->return_edges;
	blocks[*num_blocos].arestas++;

	// if isnt a root, the stack is not empty, so we add another vertex to the block to represent the last vertex
	if(!is_empty(stack)) 
		blocks[*num_blocos].vertices++;

	(*num_blocos)++;
}

void tarjan(vertex_t *vertices, vertex_t *r, info_t *blocks, unsigned int *num_blocos, stack_t *stack) {
	neighbor_t *w = r->neighbors;
	r->state = 1;

	push(stack, r);
	while(w != NULL) {
		if(w->self->state == 1 && w->self != r->parent) {
			if(w->self->level < r->lowpoint) {
				r->lowpoint = w->self->level;
			}
			r->return_edges++;  // a return edge
		} else if(w->self->state == 0) {
			w->self->parent = r;
			w->self->level = r->level + 1;
			w->self->lowpoint = w->self->level;
			
			// recursive call
			tarjan(vertices, w->self, blocks, num_blocos, stack);

			if(w->self->lowpoint < r->lowpoint) {
				r->lowpoint = w->self->lowpoint;
			}
			// can be a cut vertex
			else {
				// if the vertex is the root of the spanning tree or the lowpoint of the child is greater(below) than the level of the vertex, it is a cut vertex
				if(r->parent == NULL || r->level <= w->self->lowpoint) {
					make_block(r, stack, blocks, num_blocos);
				}
			}
		}
		w = w->next;
	}

	r->state = 2;
}

void calc_all_lowpoints(vertex_t *vertices, unsigned int n, info_t *blocks, unsigned int *num_blocos) {
	stack_t *stack = create_stack(n);
	for(unsigned int i = 0; i < n; i++) {
		vertex_t *v = &vertices[i];
		if(v->state == 0) {
			tarjan(vertices, v, blocks, num_blocos, stack);
			// if the root of the spanning tree is an isolated vertex, it is a block
			if(v->neighbors == NULL) {
				blocks[*num_blocos].vertices++;
				(*num_blocos)++;
			}
		}
	}
	free_stack(stack);
}

void print_levels(vertex_t *vertices, unsigned int n) {
	for(unsigned int i = 0; i < n; i++) {
		// print the parent
		if(vertices[i].parent != NULL) {
			printf("Vertex %u: parent = %lu\n", i + 1, (vertices[i].parent - vertices) + 1);
		} else {
			printf("Vertex %u: parent = NULL\n", i + 1);
		}
		printf("Vertex %u: level = %u, lowpoint = %u\n\n", i + 1, vertices[i].level, vertices[i].lowpoint);
	}
}

info_t *info_blocos(unsigned int *num_blocos) {
	unsigned int n;
	scanf("%u", &n);

	info_t *blocks = create_info(n);
	vertex_t *vertices = create_vertex_list(n);
	*num_blocos = 0;

	unsigned int a, b;
	while(scanf("%u %u", &a, &b) == 2) 
		add_edge(vertices, a, b);

	calc_all_lowpoints(vertices, n, blocks, num_blocos);
	free_vertex_list(vertices, n);
	return blocks;
}