#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE 20

typedef struct {
    int a;
    int b;
} Branch;

int vertex_cover(int pos, int* sol, Branch* branches, int n_nodes, int n_branches, int count);
int is_valid(int* sol, Branch* branches, int n_branches);
Branch* get_branches(FILE* fp, int* n_branches, int* n_nodes);
FILE* getFile();

int main() {
    
    FILE* fp = getFile();
    if(!fp) {
        printf("Errore in lettura file.\n");
        return -1;
    }
    
    int n_branches, n_nodes;
    Branch* branches = get_branches(fp, &n_branches, &n_nodes);

    int* sol = (int*)malloc(sizeof(int)*n_nodes);
    
    printf("Vertex cover trovati: \n");
    int count = vertex_cover(0, sol, branches, n_nodes, n_branches, 0);
    printf("\n(%d trovati)\n", count);

    return 0;
}

/**
 * Implementazione dell'algoritmo per trovare l'insieme delle parti.
 * Non serve il parametro int* val, in quanto si sa già che i nodi sono identificati da interi che vanno da 0 a N.
 * Tutti gli insiemi che non soddisfano le condizioni di vertex cover non vengono stampati.
 */ 
int vertex_cover(int pos, int* sol, Branch* branches, int n_nodes, int n_branches, int count) {
    if(pos >= n_nodes) {
        if(is_valid(sol, branches, n_branches)) { /* Controllo se l'insieme trovato è un vertex cover */
            printf("{ ");
            for(int i=0; i<n_nodes; i++)
                if(sol[i]==1)
                    printf("%d ", i);
            printf("} ");
            count++;
        }
        return count;
    }
    sol[pos] = 0;
    count = vertex_cover(pos+1, sol, branches, n_nodes, n_branches, count);
    sol[pos] = 1;
    count = vertex_cover(pos+1, sol, branches, n_nodes, n_branches, count);
    return count;
}

/**
 * La funzione controlla se l'insieme dato (identificato dagli indici di sol dove sol[i]==1) è un vertex cover.
 */
int is_valid(int* sol, Branch* branches, int n_branches) {
    for(int i=0; i<n_branches; i++)
        if(sol[branches[i].a]!=1 && sol[branches[i].b]!=1) /* Se nessuno dei nodi del ramo appartengono all'insieme, allora questo non è vertex cover. */
            return 0;
    return 1;
}

/**
 * La funzione legge da file i dati inerenti il grafo da analizzare.
 */ 
Branch* get_branches(FILE* fp, int* n_branches, int* n_nodes) {

    fscanf(fp, "%d %d", n_nodes, n_branches);

    Branch* branches = (Branch*)malloc(*n_branches*sizeof(Branch));
    
    for(int i=0; i<*n_branches; i++)
        fscanf(fp, "%d %d", &branches[i].a, &branches[i].b);

    return branches;
}

FILE* getFile() {
    char fileName[MAX_FILE+1];

    printf("Inserire nome file: ");
    scanf("%s", fileName);

    return fopen(fileName, "r");
}