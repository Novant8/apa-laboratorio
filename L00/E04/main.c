#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE 20

typedef struct {
    int** m;
    int dim;
    int somma;
}Sottomatrice;

Sottomatrice stampaSottomatrice(int* m[], int rp, int cp, int dim);
void liberaMatrice(int** m, int dim);

int main() {

    //INPUT FILE
    char file[MAX_FILE];
    printf("Inserire il nome del file: ");
    scanf("%s", file);

    FILE* fp = fopen(file, "r");
    if(fp == NULL) {
        printf("File non trovato.\n");
        return -1;
    }

    //INPUT NUMERO DI RIGHE E COLONNE
    int nr, nc;
    fscanf(fp, "%d %d", &nr, &nc);

    //INPUT MATRICE
    int** m = (int**)malloc(sizeof(int*)*nr);
    for(int i=0; i<nr; i++) {
        m[i] = (int*)malloc(sizeof(int)*nc);
        for(int j=0; j<nc; j++)
            fscanf(fp, "%d", *(m+i)+j);
    }
    
    fclose(fp);

    //INPUT DIMENSIONE SOTTOMATRICI QUADRATE
    int dim;
    int limit = nr < nc ? nr : nc;
    Sottomatrice max_m;
    max_m.somma = 0;
    while(1) {
        printf("Inserire dimensione desiderata delle sottomatrici quadrate (inserire un valore  >1 o <%d per terminare): ", limit);
        scanf("%d", &dim);

        //Termina il ciclo se la dimensione non è coerente con le dimensioni della matrice
        if(dim < 1 || dim > limit) {
            printf("Termine ciclo...\n");
            break;
        }

        //Individuazione sottomatrici quadrate
        printf("Sottomatrici di grandezza %d:\n", dim);
        for(int i=0; i<nr-dim+1; i++)
            for(int j=0; j<nc-dim+1; j++) {
                Sottomatrice tmp = stampaSottomatrice(m, i, j, dim);
                if(tmp.somma > max_m.somma)
                    max_m = tmp;
                else
                    liberaMatrice(tmp.m, dim);
                printf("\n");
            }
    }

    printf("Sottomatrice con somma massima:\n");
    for(int i=0; i<max_m.dim; i++) {
        for(int j=0; j<max_m.dim; j++)
            printf("%d ", max_m.m[i][j]);
        printf("\n");
    }

    free(m);

    return 0;
}

/*
La seguente funzione:
- Stampa una sottomatrice quadrata di m[][] date riga e colonna di partenza (rispettivamente rp e cp).
- Restituisce un elemento Sottomatrice contenente la sottomatrice in sé e la somma di tutti i valori
*/
Sottomatrice stampaSottomatrice(int* m[], int rp, int cp, int dim) {
    Sottomatrice ret;
    ret.m = (int**)malloc(sizeof(int*)*dim);
    ret.dim = dim;
    ret.somma = 0;
    for(int i=rp; i<rp+dim; i++) {
        ret.m[i-rp] = (int*)malloc(sizeof(int)*dim);
        for(int j=cp; j<cp+dim; j++)  {
            ret.m[i-rp][j-cp] = m[i][j];
            printf("%d ", m[i][j]);
            ret.somma += m[i][j];
        }
        printf("\n");
    }
    return ret;
}

void liberaMatrice(int** m, int dim) {
    for(int i=0; i<dim; i++)
        free(m[i]);
    free(m);
}