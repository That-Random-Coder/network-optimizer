#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "graph.h"   /* provides Graph and Edge types */

#ifdef __cplusplus
extern "C" {
#endif

/* Write DOT file highlighting:
 * - MST edges in red
 * - shortest-path-to-target edges in blue
 * - source node in green
 * - target node in orange
 * - path: if target < 0, only MST is highlighted.
 * - The function reads edges from Graph->edges and compares endpoints.
 */
void writeDOTFile(const char *dot_path,
                  const Graph *g,
                  const Edge *mst, int mst_count,
                  const int *parent,
                  int source,   /* 0-based index of source vertex */
                  int target);  /* 0-based index of target vertex (-1 if skipped) */

#ifdef __cplusplus
}
#endif

#endif /* GRAPHVIZ_H */
