#ifndef MST_H
#define MST_H

#include "graph.h"

Edge *kruskal_mst(const Graph *g, int *out_count, long long *total_cost);

#endif
