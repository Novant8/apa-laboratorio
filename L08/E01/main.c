#include <stdio.h>
#include <stdlib.h>

void disp_rip(int pos, int* n_gems, char* necklace, char* gem_types, char* max_nl,  int* max_len, int tot_gems);
int check(int pos, char* necklace, char next);
int promising(int max_len, int* n_gems, int tot_gems);

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
    char gem_types[4] = "ZRTS";
    int gems[4]; /* [z, r, t, s] */
    int tot_gems, max_len;
    char *necklace, *max_nl;
    fscanf(fp, "%d", &n_test);
    int i,j;

    for(i=0; i<n_test; i++) {
        printf("TEST #%d:\n", i+1);
        tot_gems=0;
        for(j=0; j<4; j++) {
            fscanf(fp, "%d", gems+j);
            tot_gems += gems[j];
        }
        printf("zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n", gems[0], gems[1], gems[2], gems[3], tot_gems);

        max_len = 0;
        necklace = (char*)malloc(sizeof(char)*tot_gems);
        max_nl = (char*)malloc(sizeof(char)*tot_gems);
        disp_rip(0, gems, necklace, gem_types, max_nl, &max_len, tot_gems);

        printf("Collana massima di lunghezza %d:\n", max_len);
        for(j=0; j<max_len; j++)
            printf("%c ", necklace[j]);
        printf("\n");

        free(necklace);
        free(max_nl);
    }

    return 0;
}

void disp_rip(int pos, int* n_gems, char* necklace, char* gem_types, char* max_nl,  int* max_len, int tot_gems) {
    int i;
    if(pos > *max_len) {
        for(i=0; i<pos; i++)
            max_nl[i] = necklace[i];
        (*max_len)++;
    }

    //if(promising(*max_len, n_gems, tot_gems))
        for(i=0; i<4; i++) {
            if(n_gems[i] > 0) {
                if(check(pos, necklace, gem_types[i])) {
                    n_gems[i]--;
                    necklace[pos] = gem_types[i];
                    disp_rip(pos+1, n_gems, necklace, gem_types, max_nl, max_len, tot_gems);
                    n_gems[i]++;
                }
            }
        }
}

int check(int pos, char* necklace, char next) {
    if(pos == 0)
        return 1;
    switch(necklace[pos-1]) {
        case 'Z':
        case 'T':
            return next == 'Z' || next == 'R';
        case 'S':
        case 'R':
            return next == 'S' || next == 'T';
        default:
            return 0;
    }
}

/*int promising(int max_len, int* n_gems, int tot_gems) {
    return max_len < tot_gems &&       
        (n_gems[0]>0 || n_gems[1]>0) &&
        (n_gems[2]>0 || n_gems[3]>0);
}*/