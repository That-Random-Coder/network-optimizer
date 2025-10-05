#include "mst.h"
#include "unionfind.h"
#include <stdlib.h>
#include <string.h>

static int edge_cmp(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    if (ea->w < eb->w) return -1;
    if (ea->w > eb->w) return 1;
    return 0;
}

Edge *kruskal_mst(const Graph *g, int *out_count, long long *total_cost) {
    *out_count = 0;
    *total_cost = 0;
    if (!g || g->V <= 0) return NULL;
    if (g->E == 0) return NULL;
    Edge *edges_copy = malloc(sizeof(Edge) * g->E);
    if (!edges_copy) return NULL;
    memcpy(edges_copy, g->edges, sizeof(Edge) * g->E);
    qsort(edges_copy, g->E, sizeof(Edge), edge_cmp);
    DSU *d = dsu_create(g->V);
    Edge *mst = malloc(sizeof(Edge) * (g->V - 1));
    int cnt = 0;
    long long cost = 0;
    for (int i = 0; i < g->E && cnt < g->V - 1; ++i) {
        Edge e = edges_copy[i];
        if (dsu_find(d, e.u) != dsu_find(d, e.v)) {
            dsu_union(d, e.u, e.v);
            mst[cnt++] = e;
            cost += e.w;
        }
    }
    free(edges_copy);
    dsu_free(d);
    if (cnt == 0) {
        free(mst);
        return NULL;
    }
    *out_count = cnt;
    *total_cost = cost;
    return mst;
}
