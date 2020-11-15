#include <stdio.h>
#include <stdlib.h>

#define FILE_MATRICE "/home/alberto/Dropbox (Politecnico Di Torino Studenti)/apa-laboratorio/L06/E02/mat.txt"

int** malloc2dR(int nr, int nc);
int leggiMatrice(int*** m, int* nr, int* nc);
void separa(int** mat, int nr, int nc, int* bianche, int* nere);
void stampaVettore(int* v, int n);
void free2dR(int** m, int nr);

int main() {

    int nr,nc;
    int** m;
    if(!leggiMatrice(&m, &nr, &nc)) {
        printf("Errore in lettura del file.\n");
        return -1;
    }

    int area = nr*nc;
    int n_nere = area/2;
    int n_bianche = area-area/2;
    int* nere = (int*)malloc(sizeof(int)*n_nere);
    int* bianche = (int*)malloc(sizeof(int)*n_bianche);

    separa(m, nr, nc, bianche, nere);

    printf("Celle bianche: ");
    stampaVettore(bianche, n_bianche);

    printf("Celle nere: ");
    stampaVettore(nere, n_nere);
    
    free2dR(m,nr);
}

int** malloc2dR(int nr, int nc)  {
    int** m = (int**)malloc(sizeof(int*)*nr);
    for(int i=0; i<nr; i++)
        m[i] = (int*)malloc(sizeof(int)*nc);
    return m;
}

int leggiMatrice(int*** m, int* nr, int* nc) {
    FILE* fp = fopen(FILE_MATRICE, "r");
    if(!fp)
        return 0;
    
    fscanf(fp, "%d %d", nr, nc);
    *m = malloc2dR(*nr,*nc);

    for(int i=0; i<*nr; i++)
        for(int j=0; j<*nc; j++)
            fscanf(fp, "%d", &(*m)[i][j]);
    
    return 1;
}

void separa(int** mat, int nr, int nc, int* bianche, int* nere) {
    int n_bianche=0, n_nere=0;
    for(int i=0; i<nr; i++)
        for(int j=0; j<nc; j++)
            if((i*nc+j)%2==0)
                bianche[n_bianche++] = mat[i][j];
            else
                nere[n_nere++] = mat[i][j];
}

void stampaVettore(int* v, int n) {
    printf("[ ");
    for(int i=0; i<n; i++)
        printf("%d ", v[i]);
    printf("]\n");
}

void free2dR(int** m, int nr) {
    for(int i=0; i<nr; i++)
        free(m[i]);
    free(m);
}