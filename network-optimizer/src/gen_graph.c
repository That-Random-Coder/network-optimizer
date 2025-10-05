#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s V E maxw outfile\n", argv[0]);
        return 1;
    }
    int V = atoi(argv[1]);
    int E = atoi(argv[2]);
    int maxw = atoi(argv[3]);
    const char *out = argv[4];
    if (E < V-1) {
        fprintf(stderr, "E must be >= V-1\n"); return 1;
    }
    srand((unsigned)time(NULL));
    int remaining = E - (V - 1);
    FILE *f = fopen(out, "w");
    if (!f) { perror("fopen"); return 1; }
    fprintf(f, "%d %d\n", V, E);
    for (int i = 1; i < V; ++i) {
        int u = i;
        int v = rand() % i;
        int w = 1 + rand() % maxw;
        fprintf(f, "%d %d %d\n", u+1, v+1, w);
    }
    while (remaining > 0) {
        int u = rand() % V;
        int v = rand() % V;
        if (u == v) continue;
        int w = 1 + rand() % maxw;
        fprintf(f, "%d %d %d\n", u+1, v+1, w);
        remaining--;
    }
    fclose(f);
    printf("Generated %s (V=%d, E=%d)\n", out, V, E);
    return 0;
}
