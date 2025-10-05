#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void ensure_edge_capacity(Graph *g) {
    if (g->edges_capacity == 0) {
        g->edges_capacity = 16;
        g->edges = malloc(sizeof(Edge) * g->edges_capacity);
        return;
    }
    if (g->E >= g->edges_capacity) {
        g->edges_capacity *= 2;
        g->edges = realloc(g->edges, sizeof(Edge) * g->edges_capacity);
    }
}

static void ensure_adj_capacity(Graph *g, int u) {
    if (g->adj_capacity[u] == 0) {
        g->adj_capacity[u] = 4;
        g->adj[u] = malloc(sizeof(AdjNode) * g->adj_capacity[u]);
        g->adj_size[u] = 0;
        return;
    }
    if (g->adj_size[u] >= g->adj_capacity[u]) {
        g->adj_capacity[u] *= 2;
        g->adj[u] = realloc(g->adj[u], sizeof(AdjNode) * g->adj_capacity[u]);
    }
}

Graph *graph_create(int V, int predicted_edge_capacity) {
    Graph *g = malloc(sizeof(Graph));
    if (!g) return NULL;
    g->V = V;
    g->E = 0;
    g->edges = NULL;
    g->edges_capacity = 0;
    if (predicted_edge_capacity > 0) {
        g->edges_capacity = predicted_edge_capacity;
        g->edges = malloc(sizeof(Edge) * g->edges_capacity);
    }
    g->adj = malloc(sizeof(AdjNode*) * V);
    g->adj_size = malloc(sizeof(int) * V);
    g->adj_capacity = malloc(sizeof(int) * V);
    for (int i = 0; i < V; ++i) {
        g->adj[i] = NULL;
        g->adj_size[i] = 0;
        g->adj_capacity[i] = 0;
    }
    return g;
}

void graph_free(Graph *g) {
    if (!g) return;
    if (g->edges) free(g->edges);
    for (int i = 0; i < g->V; ++i) {
        if (g->adj[i]) free(g->adj[i]);
    }
    free(g->adj);
    free(g->adj_size);
    free(g->adj_capacity);
    free(g);
}

int graph_add_edge(Graph *g, int u, int v, int w) {
    if (!g) return -1;
    if (u < 0 || u >= g->V || v < 0 || v >= g->V) return -1;
    ensure_edge_capacity(g);
    g->edges[g->E].u = u;
    g->edges[g->E].v = v;
    g->edges[g->E].w = w;
    g->E++;
    ensure_adj_capacity(g, u);
    g->adj[u][g->adj_size[u]].to = v;
    g->adj[u][g->adj_size[u]].w = w;
    g->adj_size[u]++;
    ensure_adj_capacity(g, v);
    g->adj[v][g->adj_size[v]].to = u;
    g->adj[v][g->adj_size[v]].w = w;
    g->adj_size[v]++;
    return 0;
}

void graph_print_stats(const Graph *g) {
    if (!g) return;
    printf("Graph: V=%d, E=%d (undirected)\n", g->V, g->E);
}
