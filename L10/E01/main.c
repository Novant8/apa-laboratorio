#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int i; //inizio
    int f; //fine
} att;

att* leggiAttivita(char* fileName, int* n_att);
void attSel(int N, att* v);
void attSel_R(int pos, att* v, int* sol, int* sol_max, int N, int d, int* d_max);
int check(int pos, att* v, int* sol, int N);

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("USO CORRETTO: %s <file>\n", *argv);
        return -1;
    }

    char* fileName = argv[1];
    int n_att;
    att* v = leggiAttivita(fileName, &n_att);
    if(v == NULL) {
        printf("Errore in lettura file\n");
        return -2;
    }

    attSel(n_att, v);

    free(v);

    return 0;
}

/**
 * La funzione legge le attività da un file il cui nome è passato per parametro.
 * Return:
 * - Vettore di strutture att contenenti le attività
 * - NULL se si è verificato un errore di lettura dal file
 */ 
att* leggiAttivita(char* fileName, int* n_att) {
    FILE* fp = fopen(fileName, "r");
    if(!fp) 
        return NULL;

    fscanf(fp, "%d", n_att);
    
    att* v = (att*)malloc(*n_att*sizeof(att));

    for(int i=0; i<*n_att; i++)
        fscanf(fp, "%d %d", &v[i].i, &v[i].f);

    return v;
}

void attSel(int N, att* v) {
    int sol[N], sol_max[N], d_max;
    memset(sol, 0, sizeof(int)*N);
    attSel_R(0, v, sol, sol_max, N, 0, &d_max);

    //Stampa soluzione massima
    printf("Soluzione di distanza massima (d_max = %d): ", d_max);
    for(int i=0; i<=N; i++)
        if(sol_max[i]==1)
            printf("(%d, %d) ", v[i].i, v[i].f);
    printf("\n");
}

void attSel_R(int pos, att* v, int* sol, int* sol_max, int N, int d, int* d_max) {
    /* Condizione di terminazione */
    if(pos >= N) {
        //Se la durata appena calcolata è massima, si salvano i dati nel vettore
        if(d > *d_max) {
            *d_max = d;
            for(int j=0; j<N; j++)
                sol_max[j] = sol[j];
        }
        return;
    }
    if(check(pos, v, sol, N)) {
        sol[pos] = 1;
        d+=v[pos].f-v[pos].i;
        attSel_R(pos+1, v, sol, sol_max, N, d, d_max);
        d-=v[pos].f-v[pos].i;
    }
    sol[pos] = 0;
    attSel_R(pos+1, v, sol, sol_max, N, d, d_max);
}

int check(int pos, att* v, int* sol, int N) {
    int last;
    //Cicla il vettore di soluzioni per prendere l'ultimo dato preso (impostato a 1)
    for(last=N-1; last>=0 && sol[last] == 0; last--);
    
    //Se nessun dato è stato preso, oppure se l'ultimo dato preso è compatibile con quello corrente, allora quest'ultimo è valido
    return last < 0 || v[pos].i >= v[last].f;
}