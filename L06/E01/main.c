#include <stdio.h>
#include <stdlib.h>

#define FILE_BRANI "/home/alberto/Dropbox (Politecnico Di Torino Studenti)/apa-laboratorio/L06/E01/brani.txt"
#define MAX_BRANO 255

typedef struct {
    char** brani;
    int n_brani;
} Amico;

int ricava_playlist(Amico* amici, int n_amici);
int princ_molt(int pos, Amico* amici, char* sol[], int n, int cnt);
void free_struct(Amico* amici, int n_amici);

int main() {

    FILE* fp = fopen(FILE_BRANI, "r"); 
    if(!fp) {
        printf("Errore in lettura file.\n");
        return -1;
    }

    int n_amici;
    fscanf(fp, "%d", &n_amici);
    Amico* amici = (Amico*)malloc(n_amici*sizeof(Amico));
    for(int i=0; i<n_amici; i++) {
        fscanf(fp, "%d", &amici[i].n_brani);
        amici[i].brani = (char**)malloc(sizeof(char*)*amici[i].n_brani);

        for(int j=0; j<amici[i].n_brani; j++) {
            amici[i].brani[j] = (char*)malloc(sizeof(char)*(MAX_BRANO+1));
            fscanf(fp, "%s", amici[i].brani[j]);
        }
    }

    int cnt = ricava_playlist(amici, n_amici);
    printf("Sono state trovate %d playlist possibili.\n", cnt);

    free_struct(amici, n_amici);
}

int ricava_playlist(Amico* amici, int n_amici) {
    char* sol[n_amici];
    return princ_molt(0, amici, sol, n_amici, 0);
}

int princ_molt(int pos, Amico* amici, char* sol[], int n, int cnt) {
    int i;
    if(pos>=n) {
        for(i=0; i<n;i++)
            printf("%d. %s\n", i+1, sol[i]);
        printf("\n");
        return cnt+1;
    }
    for(i=0; i<amici[pos].n_brani;i++) {
        sol[pos] = amici[pos].brani[i];
        cnt = princ_molt(pos+1, amici, sol, n, cnt);
    }
    return cnt;
}

void free_struct(Amico* amici, int n_amici) {
    for(int i=0; i<n_amici; i++) {
        for(int j=0; j<amici[i].n_brani; j++)
            free(amici[i].brani[j]);
        free(amici[i].brani);
    }
    free(amici);
}