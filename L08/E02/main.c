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
    int n[N_GEMS];      //indica il numero di gemme presenti nella collana [n_z, n_r, n_t, n_s]
    int val;
} Necklace;

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
    gems[0].type = 'Z';
    gems[1].type = 'R';
    gems[2].type = 'T';
    gems[3].type = 'S';
    int tot_gems, max_rip, nl_len;
    Necklace necklace, max_nl;
    fscanf(fp, "%d", &n_test);
    int i,j;

    for(i=0; i<n_test; i++) {
        printf("TEST #%d:\n", i+1);
        tot_gems=0;
        for(j=0; j<N_GEMS; j++) {
            fscanf(fp, "%d", &gems[j].n);
            tot_gems+=gems[j].n;
        }
        for(j=0; j<N_GEMS; j++)
            fscanf(fp, "%d", &gems[j].val);
        fscanf(fp, "%d", &max_rip);
        printf("zaffiro = %d [%d], rubino = %d [%d], topazio = %d [%d], smeraldo = %d [%d], TOT = %d {max_rip = %d}\n", gems[0].n, gems[0].val, gems[1].n, gems[1].val, gems[2].n, gems[2].val, gems[3].n, gems[3].val, tot_gems, max_rip);

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

    return 0;
}

void disp_rip(int pos, Gem gems[], Necklace necklace, Necklace* max_nl, int max_rip) {
    int i;
    if(necklace.val > max_nl->val && necklace.n[0] <= necklace.n[3]) {
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

int check(int pos, Gem next, int gem_idx, Necklace necklace, Necklace max_nl, int max_rip) {
    if(pos == 0)
        return 1;
    
    //Controllo sul numero di gemme ripetute
    int i, n_rip;
    for(i=pos-1, n_rip=0; i>=0 && necklace.composition[i] == gem_idx; i--, n_rip++)
        if(n_rip>=max_rip-1)
            return 0;

    //Se il numero di zaffiri è maggiore di quello degli smeraldi e non ci sono più smeraldi da inserire, allora la collana non può essere valida
    if(necklace.n[0] > necklace.n[3] && necklace.n[3] == 0)
        return 0;
    
    switch(necklace.composition[pos-1]) {
        case 0: //Z
        case 2: //R
            return next.type == 'Z' || next.type == 'R';
        case 1: //T
        case 3: //S
            return next.type == 'S' || next.type == 'T';
        default:
            return 0;
    }
}