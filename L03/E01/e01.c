#include <stdio.h>

#define MAXR 50
#define MAX_FILE 20

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate estremo;
    int base;
    int altezza;
} Regione;

FILE* getFile();
int leggiMatrice(int m[MAXR][MAXR], int* nr, int* nc);
int riconosciRegione(int m[MAXR][MAXR], int nr, int nc, int r, int c, int* b, int* h);
void stampaDatiRegione(Regione r);
Regione getRegione(int x, int y, int b, int h);

int main() {

    int nr, nc, m[MAXR][MAXR];
    if(!leggiMatrice(m, &nr, &nc)) {
        printf("Si è verificato un errore in lettura del file.\n");
        return -1;
    }

    Regione reg = getRegione(0,0,0,0);
    Regione r_maxb = reg, r_maxh = reg, r_maxa = reg;

    int n_regioni = 0, b, h, r, c=0;
    for(int r=0; r<nr; r++)
        for(c=0; c<nc; c++)
            if(riconosciRegione(m, nr, nc, r, c, &b, &h)) {
                reg = getRegione(r, c, b, h);

                printf("Individuata regione: ");
                stampaDatiRegione(reg);
                
                //Controllo se la regione contiene parametri (b,h,A) massimi
                if(b > r_maxb.base)
                    r_maxb = reg;
                if(h > r_maxh.altezza)
                    r_maxh = reg;
                if(b*h > r_maxa.base * r_maxa.altezza)
                    r_maxa = reg;

                //Le prossime b celle in orizzontale appartengono a questa regione, quindi vengono saltate
                c+=b;
            }

    //Output regioni con parametri massimi
    printf("\nMax Base: ");
    stampaDatiRegione(r_maxb);
    printf("Max Area: ");
    stampaDatiRegione(r_maxa);
    printf("Max Altezza: ");
    stampaDatiRegione(r_maxh);
    
    return 0;
}

FILE* getFile() {
    char fileName[MAX_FILE];
    printf("Inserire nome del file contenente la matrice: ");
    scanf("%s", fileName);
    return fopen(fileName, "r");
}

int leggiMatrice(int m[MAXR][MAXR], int* nr, int* nc) {
    FILE* fp = getFile();
    if(!fp)
        return 0;
    
    fscanf(fp, "%d %d", nr, nc);

    for(int i=0; i<*nr; i++)
        for(int j=0; j<*nc; j++)
            fscanf(fp, "%d", &m[i][j]);
}

int riconosciRegione(int m[MAXR][MAXR], int nr, int nc, int r, int c, int* b, int* h) {
    /*
    - Se la cella corrente è pari a 0 non si è dentro una regione
    - Se la cella superiore è pari a 1 si è dentro una regione ma non è l'estremo superiore sinistro
    Non è necessario in questo caso controllare se la cella a sinistra è pari a 1 perchè nel main vengono già saltate le prossime celle in orizzontale quando una regione viene individuata
    */
    if(m[r][c] == 0 || (r>0 && m[r-1][c] == 1))
        return 0;
    
    //La cella è l'estremo superiore sinistro di una regione, si misurano base e altezza
    int k;
    *b=0;
    *h=0;
    for(k=r; k<nr && m[k][c] != 0; k++)
        (*h)++;
    for(k=c; k<nc && m[r][k] != 0; k++)
        (*b)++;

    return 1;
}

void stampaDatiRegione(Regione r) {
    printf("estr. sup. SX=<%d,%d>, b=%d, h=%d, Area=%d\n", r.estremo.x, r.estremo.y, r.base, r.altezza, r.base*r.altezza);
}

Regione getRegione(int x, int y, int b, int h) {
    Regione r;
    r.estremo.x = x;
    r.estremo.y = y;
    r.base = b;
    r.altezza = h;
    return r;
}