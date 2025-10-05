#define _POSIX_C_SOURCE 200809L
#include "graphviz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* helper: check if edge e connects vertices a and b (0-based) */
static int edge_matches(const Edge *e, int a, int b) {
    return (e->u == a && e->v == b) || (e->u == b && e->v == a);
}

void writeDOTFile(const char *dot_path,
                  const Graph *g,
                  const Edge *mst, int mst_count,
                  const int *parent,
                  int source,      // new
                  int target)      // new
{
    if (!g) return;
    FILE *fp = fopen(dot_path, "w");
    if (!fp) { perror("fopen"); return; }

    fprintf(fp,
            "graph Network {\n"
            "  rankdir=LR;\n"
            "  splines=true;\n"
            "  overlap=false;\n"
            "  node [shape=circle, style=filled, fontname=\"Arial\", fontsize=12];\n\n");

    /* nodes (print 1-based labels, highlight source/target) */
    for (int i = 0; i < g->V; ++i) {
    const char *fill = "white"; // default color
    char label[32];             // text for the node

    if (i == source) {
        fill = "green";
        snprintf(label, sizeof(label), "%d (S)", i+1);  // source label
    } else if (i == target) {
        fill = "orange";
        snprintf(label, sizeof(label), "%d (T)", i+1);  // target label
    } else {
        snprintf(label, sizeof(label), "%d", i+1);      // normal label
    }

    fprintf(fp, "  %d [label=\"%s\", style=filled, fillcolor=%s];\n", i+1, label, fill);
}

    fprintf(fp, "\n");

    /* mark MST edges: create boolean flags per graph edge */
    int *in_mst = calloc((size_t)g->E, sizeof(int));
    if (!in_mst) { fclose(fp); return; }
    for (int ei = 0; ei < g->E; ++ei) {
        for (int mi = 0; mi < mst_count; ++mi) {
            if (edge_matches(&g->edges[ei], mst[mi].u, mst[mi].v)) {
                in_mst[ei] = 1;
                break;
            }
        }
    }

    /* mark path edges (if target >= 0) */
    int *in_path = calloc((size_t)g->E, sizeof(int));
    if (!in_path) { free(in_mst); fclose(fp); return; }
    if (target >= 0 && parent != NULL) {
        int cur = target;
        while (cur != -1 && parent[cur] != -1) {
            int p = parent[cur];
            /* find the corresponding graph edge index (first match) */
            for (int ei = 0; ei < g->E; ++ei) {
                if (edge_matches(&g->edges[ei], p, cur)) {
                    in_path[ei] = 1;
                    break;
                }
            }
            cur = p;
        }
    }

    /* print edges with styles */
    for (int ei = 0; ei < g->E; ++ei) {
        int u = g->edges[ei].u + 1;
        int v = g->edges[ei].v + 1;
        int w = g->edges[ei].w;
        if (in_mst[ei]) {
            fprintf(fp, "  %d -- %d [label=\"%d\", penwidth=3, color=\"red\"];\n", u, v, w);
        } else if (in_path[ei]) {
            fprintf(fp, "  %d -- %d [label=\"%d\", penwidth=3, color=\"blue\"];\n", u, v, w);
        } else {
            fprintf(fp, "  %d -- %d [label=\"%d\", penwidth=1, color=\"gray70\"];\n", u, v, w);
        }
    }

    fprintf(fp, "}\n");
    fclose(fp);
    free(in_mst);
    free(in_path);

    printf("Wrote DOT file: %s\n", dot_path);
}
