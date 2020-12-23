#include <stdio.h>
#include <stdlib.h>

#define N_GEMS 4

typedef struct {
    char type;
    int n;
    int val;
} Gem;

typedef struct {
    int* composition;
    int n[N_GEMS];      //indica il numero di gemme presenti nella collana [n_Z, n_R, n_T, n_S]
    int val;
} Necklace;

enum {
    Z, R, T, S
};

void getGems(FILE* fp, Gem gems[], int *tot_gems, int *max_rip);
void maxNecklace(Gem gems[], int tot_gems, int max_rip);
void disp_rip(int pos, Gem gems[], Necklace necklace, Necklace* max_nl, int max_rip);
int check(int pos, Gem next, int gem_idx, Necklace necklace, Necklace max_nl, int max_rip);

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
    Gem gems[N_GEMS];
    gems[Z].type = 'Z';
    gems[R].type = 'R';
    gems[T].type = 'T';
    gems[S].type = 'S';
    int tot_gems, max_rip, nl_len;
    Necklace necklace, max_nl;
    fscanf(fp, "%d", &n_test);
    int i,j;

    for(i=0; i<n_test; i++) {
        printf("TEST #%d:\n", i+1);
        
        getGems(fp, gems, &tot_gems, &max_rip);
        printf("zaffiro = %d [%d], rubino = %d [%d], topazio = %d [%d], smeraldo = %d [%d], TOT = %d {max_rip = %d}\n", gems[0].n, gems[0].val, gems[1].n, gems[1].val, gems[2].n, gems[2].val, gems[3].n, gems[3].val, tot_gems, max_rip);
        
        maxNecklace(gems, tot_gems, max_rip);
    }

    return 0;
}

/**
 * La funzione:
 * - legge da file il numero di ogni gemma di un singolo test
 * - legge da file il numero massimo di ripetizioni, passato come parametro by reference 
 * - calcola il numero totale di gemme presenti, passato come parametro by reference
 */ 
void getGems(FILE* fp, Gem gems[], int *tot_gems, int *max_rip) {
    int j;
    *tot_gems=0;
    for(j=0; j<N_GEMS; j++) {
        fscanf(fp, "%d", &gems[j].n);
        *tot_gems+=gems[j].n;
    }
    for(j=0; j<N_GEMS; j++)
        fscanf(fp, "%d", &gems[j].val);
    fscanf(fp, "%d", max_rip);
}

/**
 * Funzione wrapper che stampa a schermo la collana di valore ottimale
 */ 
void maxNecklace(Gem gems[], int tot_gems, int max_rip) {
    Necklace necklace, //Collana corrente durante la ricorsione
             max_nl; //Collana di massima lunghezza
    int j, nl_len;
    
    //Inizializzazione collane
    necklace.composition = (int*)malloc(sizeof(int)*tot_gems);
    max_nl.composition = (int*)malloc(sizeof(int)*tot_gems);
    necklace.val = max_nl.val = 0;
    for(j=0; j<N_GEMS;j++)
        necklace.n[j] = 0;
    
    disp_rip(0, gems, necklace, &max_nl, max_rip);

    nl_len = 0;
    for(j=0; j<N_GEMS; j++)
        nl_len+=max_nl.n[j];

    printf("Soluzione ottima di valore %d usando %d gemma/e\nComposizione collana: ", max_nl.val, nl_len);
    for(j=0; j<nl_len; j++)
        printf("%c ", gems[max_nl.composition[j]].type);
    printf("\n");

    free(necklace.composition);
    free(max_nl.composition);
}

/**
 * La funzione esplora tutte le possibili collane e salva in max_nl quella avente valore ottimale che rispetta il vincolo di ripetizioni massime max_rip 
 */ 
void disp_rip(int pos, Gem gems[], Necklace necklace, Necklace* max_nl, int max_rip) {
    int i;
    // Se la collana corrente è valida e ha valore maggiore di quella massima, salva in max_nl
    if(necklace.val > max_nl->val && necklace.n[Z] <= necklace.n[S]) {
        int nl_len=0;
        max_nl->val = necklace.val;
        for(i=0;i<N_GEMS;i++) {
            max_nl->n[i] = necklace.n[i];
            nl_len+=necklace.n[i];
        }
        for(i=0;i<nl_len;i++)
            max_nl->composition[i] = necklace.composition[i];
    }

    for(i=0; i<N_GEMS; i++) {
        if(gems[i].n > 0 && check(pos, gems[i], i, necklace, *max_nl, max_rip)) {
            gems[i].n--;
            necklace.n[i]++;
            necklace.val+=gems[i].val;
            necklace.composition[pos] = i;
            disp_rip(pos+1, gems, necklace, max_nl, max_rip);
            gems[i].n++;
            necklace.n[i]--;
            necklace.val-=gems[i].val;
        }
    }
}

/**
 * Funzione che controlla le condizioni di pruning
 */ 
int check(int pos, Gem next, int gem_idx, Necklace necklace, Necklace max_nl, int max_rip) {
    if(pos == 0)
        return 1;
    
    //Controllo sul numero di gemme ripetute
    int i, n_rip;
    for(i=pos-1, n_rip=0; i>=0 && necklace.composition[i] == gem_idx; i--, n_rip++)
        if(n_rip>=max_rip-1)
            return 0;

    //Se il numero di zaffiri è maggiore di quello degli smeraldi e non ci sono più smeraldi da inserire, allora non possono più essere aggiunte gemme
    if(necklace.n[Z] > necklace.n[S] && necklace.n[S] == 0)
        return 0;
    
    // Controllo sulla gemma successiva
    switch(necklace.composition[pos-1]) {
        case Z:
        case T:
            return next.type == 'Z' || next.type == 'R';
        case R:
        case S:
            return next.type == 'S' || next.type == 'T';
        default:
            return 0;
    }
}