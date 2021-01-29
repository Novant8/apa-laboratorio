#ifndef GRAPH_H
#define GRAPH_H

#define MAX_STANZA 20

typedef struct graph_s *Graph;

typedef struct path_s {
    int* stanze;
    int n_stanze;
    int ricchezza;
} Path;

/* Inizializzazione e free */
Graph GRAPHinit(int n_stanze);
void GRAPHfree(Graph g);

/* Lettura del grafo da file */
Graph GRAPHload(FILE* fp);

/* Funzioni di percorso (PATH) */
Path PATHinit(int n_stanze);
Path GRAPHpathLoad(Graph g, FILE* fp);
int GRAPHpathCheck(Graph g, Path* p, int M, int PF);
Path GRAPHpathBest(Graph g, int M, int PF);

/* Ricavo nome stanza */
void GRAPHgetStanza(Graph g, int id, char* key);

#endif