#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <windows.h>
#include "graph.h"
#include "mst.h"
#include "dijkstra.h"
#include "graphviz.h"


static long long times_diff_ms(clock_t a, clock_t b) {
    return (long long)(b - a) * 1000LL / (long long)CLOCKS_PER_SEC;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
        return 1;
    }
    const char *infile = argv[1];
    FILE *f = fopen(infile, "r");
    if (!f) { perror("fopen"); return 1; }
    int V, E;
    if (fscanf(f, "%d %d", &V, &E) != 2) { fprintf(stderr, "Failed to read V E\n"); fclose(f); return 1; }
    Graph *g = graph_create(V, E);
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        if (fscanf(f, "%d %d %d", &u, &v, &w) != 3) {
            break;
        }
        graph_add_edge(g, u-1, v-1, w);
    }
    fclose(f);
    graph_print_stats(g);
    clock_t t0 = clock();
    int mst_count = 0;
    long long mst_cost = 0;
    Edge *mst = kruskal_mst(g, &mst_count, &mst_cost);
    clock_t t1 = clock();
    long long ms_kruskal = times_diff_ms(t0, t1);
    printf("\n--- Minimum Spanning Tree (Kruskal) ---\n");
    if (!mst) {
        printf("No MST found (graph may be disconnected)\n");
    } else {
        for (int i = 0; i < mst_count; ++i) {
            printf("%d - %d : %d\n", mst[i].u + 1, mst[i].v + 1, mst[i].w);
        }
        printf("Total cost = %lld\n", mst_cost);
    }
    printf("Kruskal time: %lld ms\n", ms_kruskal);
    int source = -1;
    printf("\nEnter source vertex for Dijkstra (1..%d): ", V);
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid source\n");
        graph_free(g);
        free(mst);
        return 1;
    }
    source = source - 1;
    long long *dist = malloc(sizeof(long long) * V);
    int *parent = malloc(sizeof(int) * V);
    t0 = clock();
    dijkstra(g, source, dist, parent);
    t1 = clock();
    long long ms_dij = times_diff_ms(t0, t1);
    printf("\n--- Dijkstra distances from %d ---\n", source + 1);
    for (int i = 0; i < V; ++i) {
        if (dist[i] >= LLONG_MAX / 4) printf("To %d: INF\n", i+1);
        else printf("To %d: %lld\n", i+1, dist[i]);
    }
    printf("Dijkstra time: %lld ms\n", ms_dij);
    printf("\nPrint path to target? Enter vertex number (or 0 to skip): ");
    int target = 0;
    if (scanf("%d", &target) == 1 && target >= 1 && target <= V) {
        int t = target - 1;
        if (dist[t] >= LLONG_MAX / 4) {
            printf("No path to %d\n", target);
        } else {
            int *stack = malloc(sizeof(int) * V);
            int top = 0;
            int cur = t;
            while (cur != -1) {
                stack[top++] = cur;
                cur = parent[cur];
            }
            printf("Path %d -> %d: ", source+1, target);
            for (int i = top - 1; i >= 0; --i) {
                printf("%d", stack[i] + 1);
                if (i) printf(" -> ");
            }
            printf("\n");
            free(stack);
        }
    }
        /* --- write DOT file (before freeing g/parent/mst) --- */
    int target_index = -1;
    if (target >= 1 && target <= V) target_index = target - 1;  // convert to 0-based; if user skipped target, it'll be -1  // convert to 0-based; if user skipped target, it'll be -1
// Call graphviz writer: produces graph.dot in project folder
char dot_path[MAX_PATH];
GetFullPathName("graph.dot", MAX_PATH, dot_path, NULL);
int source_index = source; // already 0-based
writeDOTFile(dot_path, g, mst, mst_count, parent, source_index, target_index);


char py_cmd[MAX_PATH + 50];
sprintf(py_cmd, "python generate_pdf.py \"%s\"", dot_path);
system(py_cmd);

// Delete old PDF if exists (optional)
// remove("graph.pdf");

    free(dist); free(parent);
    graph_free(g);
    free(mst);
    return 0;
}
