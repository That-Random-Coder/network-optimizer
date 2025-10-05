#ifndef UNIONFIND_H
#define UNIONFIND_H

typedef struct {
    int n;
    int *parent;
    int *rank;
} DSU;

DSU *dsu_create(int n);
void dsu_free(DSU *d);
int dsu_find(DSU *d, int x);
int dsu_union(DSU *d, int a, int b);

#endif
