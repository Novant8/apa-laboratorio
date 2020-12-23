#include <stdio.h>
#include <stdlib.h>

#define N_GEMS 4

enum gems {
    Z, R, T, S
};

int max_length(int gems[]);
int fZ(int***** known, int gems[]);
int fR(int***** known, int gems[]);
int fS(int***** known, int gems[]);
int fT(int***** known, int gems[]);
int***** alloc_known(int gems[]);
void free_known(int***** known, int gems[]);

int main(int argc, char* argv[]) {

    if(argc!=2) {
        printf("USO CORRETTO: %s <file_input>\n", *argv);
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    if(!fp) {
        printf("Errore in lettura del file.\n");
        return -2;
    }
    
    int n_test;
    int gems[N_GEMS]; /* [n_Z, n_R, n_T, n_S] */
    int tot_gems;
    fscanf(fp, "%d", &n_test);
    int i,j;

    for(i=0; i<n_test; i++) {
        printf("TEST #%d:\n", i+1);
        tot_gems=0;
        for(j=0; j<N_GEMS; j++) {
            fscanf(fp, "%d", gems+j);
            tot_gems+=gems[j];
        }
        printf("zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n", gems[0], gems[1], gems[2], gems[3], tot_gems);

        printf("Collana massima di lunghezza %d.\n", max_length(gems));
    }

    return 0;
}

/**
 * Funzione wrapper che stampa la lunghezza massima della collana di un singolo test.
 */ 
int max_length(int gems[]) {

    /* 
    Matrice a cinque dimensioni: il primo indice indica la gemma iniziale, gli altri quattro sono il numero di gemme presenti nella collana attuale.
    Ogni cella della matrice contiene la lunghezza massima della collana.
     */ 
    int***** known = alloc_known(gems);
    int max[N_GEMS];

    max[Z] = fZ(known, gems);
    max[R] = fR(known, gems);
    max[T] = fT(known, gems);
    max[S] = fS(known, gems);

    free_known(known, gems);

    // Ricavo massima lunghezza tra le quattro trovate 
    int max_len = 0;
    for(int i=0; i<N_GEMS; i++)
        if(max[i]>max_len)
            max_len = max[i];

    return max_len;
}

/**
 * Funzione che ricava la lunghezza massima di una collana avente uno zaffiro come primo elemento.
 */ 
int fZ(int***** known, int gems[]) {
    if (gems[Z] <= 0)
        return 0;

    /* Se la lunghezza massima è già stata ricavata in precedenza, allora restituisci quella */
    int known_sol = known[Z][gems[Z]][gems[R]][gems[T]][gems[S]];
    if(known_sol != -1)
        return known_sol;

    /* La lunghezza massima non è ancora stata trovata: ricava ricorsivamente */
    gems[Z]--;
    int max_Z = fZ(known, gems);
    int max_R = fR(known, gems);
    gems[Z]++;

    int max = (max_Z > max_R ? max_Z : max_R)+1;
    known[Z][gems[Z]][gems[R]][gems[T]][gems[S]] = max;
    return max;
}

/**
 * Funzione che ricava la lunghezza massima di una collana avente un rubino come primo elemento.
 */ 
int fR(int***** known, int gems[]) {
    if (gems[R] <= 0)
        return 0;

    /* Se la lunghezza massima è già stata ricavata in precedenza, allora restituisci quella */
    int known_sol = known[R][gems[Z]][gems[R]][gems[T]][gems[S]];
    if(known_sol != -1)
        return known_sol;

    /* La lunghezza massima non è ancora stata trovata: ricava ricorsivamente */
    gems[R]--;
    int max_S = fS(known, gems);
    int max_T = fT(known, gems);
    gems[R]++;

    int max = (max_S > max_T ? max_S : max_T)+1;
    known[R][gems[Z]][gems[R]][gems[T]][gems[S]] = max;
    return max;
}

/**
 * Funzione che ricava la lunghezza massima di una collana avente un topazio come primo elemento.
 */ 
int fT(int***** known, int gems[]) {
    if (gems[T] <= 0)
        return 0;

    /* Se la lunghezza massima è già stata ricavata in precedenza, allora restituisci quella */
    int known_sol = known[T][gems[Z]][gems[R]][gems[T]][gems[S]];
    if(known_sol != -1)
        return known_sol;

    /* La lunghezza massima non è ancora stata trovata: ricava ricorsivamente */
    gems[T]--;
    int max_Z = fZ(known, gems);
    int max_R = fR(known, gems);
    gems[T]++;

    int max = (max_Z > max_R ? max_Z : max_R)+1;
    known[T][gems[Z]][gems[R]][gems[T]][gems[S]] = max;
    return max;
}

/**
 * Funzione che ricava la lunghezza massima di una collana avente uno smeraldo come primo elemento.
 */ 
int fS(int***** known, int gems[]) {
    if (gems[S] <= 0)
        return 0;

    /* Se la lunghezza massima è già stata ricavata in precedenza, allora restituisci quella */
    int known_sol = known[S][gems[Z]][gems[R]][gems[T]][gems[S]];
    if(known_sol != -1)
        return known_sol;

    /* La lunghezza massima non è ancora stata trovata: ricava ricorsivamente */
    gems[S]--;
    int max_S = fS(known, gems);
    int max_T = fT(known, gems);
    gems[S]++;

    int max = (max_S > max_T ? max_S : max_T)+1;
    known[S][gems[Z]][gems[R]][gems[T]][gems[S]] = max;
    return max;
}

/**
 * Allocazione della matrice a cinque dimensioni contenente i valori già noti.
 * Tutti i valori della matrice vengono inizializzati a -1.
 */ 
int***** alloc_known(int gems[]) {
    int***** known;
    known = (int*****)malloc(sizeof(int****)*N_GEMS);
    for(int i=0; i<N_GEMS;i++) {
        known[i] = (int****)malloc(sizeof(int***)*(gems[Z]+1));
        for(int j=0; j<=gems[Z]; j++) {
            known[i][j] = (int***)malloc(sizeof(int**)*(gems[R]+1));
            for(int k=0; k<=gems[R]; k++) {
                known[i][j][k] = (int**)malloc(sizeof(int*)*(gems[T]+1));
                for(int l=0; l<=gems[T]; l++) {
                    known[i][j][k][l] = (int*)malloc(sizeof(int)*(gems[S]+1));
                    for(int m=0; m<=gems[S]; m++)
                        known[i][j][k][l][m] = -1;
                }
            }
        }
    }
    return known;
}

/**
 * Free della matrice a cinque dimensioni contenente i valori noti.
 */ 
void free_known(int***** known, int gems[]) {
    for(int i=0; i<N_GEMS; i++) {
        for(int j=0; j<gems[Z]; j++) {
            for(int k=0; k<gems[R]; k++) {
                for(int l=0; l<gems[T]; l++) {
                    free(known[i][j][k][l]);
                }
                free(known[i][j][k]);
            }
            free(known[i][j]);
        }
        free(known[i]);
    }
    free(known);
}