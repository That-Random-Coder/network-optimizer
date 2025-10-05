#include "unionfind.h"
#include <stdlib.h>

DSU *dsu_create(int n) {
    DSU *d = malloc(sizeof(DSU));
    if (!d) return NULL;
    d->n = n;
    d->parent = malloc(sizeof(int) * n);
    d->rank = malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        d->parent[i] = i;
        d->rank[i] = 0;
    }
    return d;
}

void dsu_free(DSU *d) {
    if (!d) return;
    free(d->parent);
    free(d->rank);
    free(d);
}

int dsu_find(DSU *d, int x) {
    if (d->parent[x] != x) d->parent[x] = dsu_find(d, d->parent[x]);
    return d->parent[x];
}

int dsu_union(DSU *d, int a, int b) {
    int pa = dsu_find(d, a);
    int pb = dsu_find(d, b);
    if (pa == pb) return 0;
    if (d->rank[pa] < d->rank[pb]) d->parent[pa] = pb;
    else if (d->rank[pb] < d->rank[pa]) d->parent[pb] = pa;
    else {
        d->parent[pb] = pa;
        d->rank[pa]++;
    }
    return 1;
}
