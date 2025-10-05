#include "dijkstra.h"
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int v;
    long long dist;
} HeapNode;

typedef struct {
    HeapNode *arr;
    int size;
    int cap;
} MinHeap;

static MinHeap* heap_create(int cap) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->arr = malloc(sizeof(HeapNode) * cap);
    h->size = 0;
    h->cap = cap;
    return h;
}
static void heap_free(MinHeap *h) {
    if (!h) return;
    free(h->arr);
    free(h);
}
static void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a; *a = *b; *b = tmp;
}
static void heap_push(MinHeap *h, int v, long long dist) {
    if (h->size >= h->cap) {
        h->cap *= 2;
        h->arr = realloc(h->arr, sizeof(HeapNode) * h->cap);
    }
    int i = h->size++;
    h->arr[i].v = v; h->arr[i].dist = dist;
    while (i > 0) {
        int p = (i - 1) >> 1;
        if (h->arr[p].dist <= h->arr[i].dist) break;
        heap_swap(&h->arr[p], &h->arr[i]);
        i = p;
    }
}
static int heap_empty(MinHeap *h) { return h->size == 0; }
static HeapNode heap_pop(MinHeap *h) {
    HeapNode res = h->arr[0];
    h->arr[0] = h->arr[--h->size];
    int i = 0;
    while (1) {
        int l = i*2 + 1;
        int r = l + 1;
        int smallest = i;
        if (l < h->size && h->arr[l].dist < h->arr[smallest].dist) smallest = l;
        if (r < h->size && h->arr[r].dist < h->arr[smallest].dist) smallest = r;
        if (smallest == i) break;
        heap_swap(&h->arr[i], &h->arr[smallest]);
        i = smallest;
    }
    return res;
}

int dijkstra(const Graph *g, int source, long long *dist, int *parent) {
    if (!g || source < 0 || source >= g->V) return -1;
    const long long INF = LLONG_MAX / 4;
    for (int i = 0; i < g->V; ++i) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[source] = 0;
    MinHeap *h = heap_create(64);
    heap_push(h, source, 0);
    while (!heap_empty(h)) {
        HeapNode hn = heap_pop(h);
        int u = hn.v;
        long long d = hn.dist;
        if (d != dist[u]) continue;
        for (int i = 0; i < g->adj_size[u]; ++i) {
            AdjNode an = g->adj[u][i];
            int v = an.to;
            long long nd = d + (long long)an.w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                heap_push(h, v, nd);
            }
        }
    }
    heap_free(h);
    return 0;
}
