#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

typedef struct {
    int u;
    int v;
    int w;
} Edge;

typedef struct {
    int to;
    int w;
} AdjNode;

typedef struct {
    int V;
    int E;
    Edge *edges;
    int edges_capacity;
    AdjNode **adj;
    int *adj_size;
    int *adj_capacity;
} Graph;

Graph *graph_create(int V, int predicted_edge_capacity);
void graph_free(Graph *g);
int graph_add_edge(Graph *g, int u, int v, int w);
void graph_print_stats(const Graph *g);

#endif
