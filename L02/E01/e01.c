#include <stdio.h>

#define MAX_FILE 20
#define MAX_MATRICE 50

typedef struct {
    int x;
    int y;
    int base;
    int altezza;
} Regione;

FILE* getFile();
void riempiMatrice(int m[MAX_MATRICE][MAX_MATRICE], FILE* fp, int nr, int nc);
void stampaDatiRegione(Regione r);
void individuaRegioni(Regione regioni[], int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc, int* max_b, int* max_h, int* max_a);

int main() {

    FILE* fp = getFile();
    if(!fp) {
        printf("Errore in lettura del file.\n");
        return -1;
    }

    int nr, nc;
    fscanf(fp, "%d %d", nr, nc);

    int m[nr][nc];
    riempiMatrice(m, fp, nr, nc);

    fclose(fp);

    int max_h, max_b, max_a;
    Regione regioni[nr*nc/2];
    individuaRegioni(regioni, m, nr, nc, &max_b, &max_h, &max_a);

    //Output
    printf("Max Base: ");
    stampaDatiRegione(regioni[max_b]);
    printf("Max Area: ");
    stampaDatiRegione(regioni[max_a]);
    printf("Max Altezza: ");
    stampaDatiRegione(regioni[max_h]);
    
    return 0;
}

FILE* getFile() {
    char fileName[MAX_FILE];
    printf("Inserire nome del file contenente la matrice: ");
    scanf("%s", fileName);
    return fopen(fileName, "r");
}

void riempiMatrice(int m[MAX_MATRICE][MAX_MATRICE], FILE* fp, int nr, int nc) {
    for(int i=0; i<nr; i++)
        for(int j=0; j<nc; j++)
            fscanf(fp, "%d", &m[i][j]);
}

void stampaDatiRegione(Regione r) {
    printf("estr. sup. SX=<%d,%d>, b=%d, h=%d, Area=%d\n", r.x, r.y, r.base, r.altezza, r.base*r.altezza);
}

void individuaRegioni(Regione regioni[], int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc, int* max_b, int* max_h, int* max_a) {
    int n_regioni = 0;
    *max_b = 0;
    *max_h = 0;
    *max_a = 0;
    for(int i=0; i<nr; i++)
        for(int j=0; j<nc; j++) {
            /*
            Aggiungi una nuova regione se:
                - La cella corrente è nera (condizione 1)
                - La cella non è già parte di un'altra regione (condizioni 2 e 3): per questo basta controllare se la cella al di sopra di quella corrente non sia nera (non è necessario controllare la cella di fianco, dato che viene già incrementata j all'interno dell'if) 
            */
            if (m[i][j] == 1 && (i==0 || m[i-1][j] == 0)) {
                regioni[n_regioni].x = i;
                regioni[n_regioni].y = j;
                regioni[n_regioni].base = 0;
                regioni[n_regioni].altezza = 0;

                int k;
                for(k=i; k<nr && m[k][j] != 0; k++)
                    regioni[n_regioni].altezza++;
                for(k=j; k<nc && m[i][k] != 0; k++) {
                    regioni[n_regioni].base++;
                    j++; //Tutte le celle nere successive a quella corrente (in orizzontale) vanno saltate, dato che vengono già aggiunte alla regione corrente.
                }
                

                int area = regioni[n_regioni].base * regioni[n_regioni].altezza;
                if(regioni[n_regioni].base > regioni[*max_b].base)
                    *max_b = n_regioni;
                if(regioni[n_regioni].altezza > regioni[*max_h].altezza)
                    *max_h = n_regioni;
                if(area > regioni[*max_a].base * regioni[*max_a].altezza)
                    *max_a = n_regioni;


                (n_regioni)++;
            }
        }
}