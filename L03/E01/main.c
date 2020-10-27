#include <stdio.h>

#define MAXR 50
#define FILE_MAPPA "/home/alberto/OneDrive/APA/L03/E01/mappa.txt"

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate estremo;
    int base;
    int altezza;
} Regione;

int leggiMatrice(int m[MAXR][MAXR], int* nr, int* nc);
int riconosciRegione(int m[MAXR][MAXR], int nr, int nc, int r, int c, int* b, int* h);
void stampaDatiRegione(Regione r);
Regione inizializzaRegione(int x, int y, int b, int h);

int main() {

    int nr, nc, m[MAXR][MAXR];
    if(!leggiMatrice(m, &nr, &nc)) {
        printf("Si è verificato un errore in lettura del file.\n");
        return -1;
    }

    Regione reg = inizializzaRegione(0,0,0,0); /* Regione corrente nel ciclo (inizializzata con valori minimi per le prossime regioni) */
    Regione r_maxb = reg, /* Regione con base massima */
            r_maxh = reg, /* Regione con altezza massima */
            r_maxa = reg; /* Regione con area massima */

    int n_regioni = 0, b, h, r, c;
    for(int r=0; r<nr; r++) {
        c=0;
        while(c<nc) {
            if(riconosciRegione(m, nr, nc, r, c, &b, &h)) {
                reg = inizializzaRegione(r, c, b, h);
                
                /* Controllo se la regione contiene parametri (b,h,A) massimi */
                if(b > r_maxb.base)
                    r_maxb = reg;
                if(h > r_maxh.altezza)
                    r_maxh = reg;
                if(b*h > r_maxa.base * r_maxa.altezza)
                    r_maxa = reg;

                /* Le prossime b celle in orizzontale appartengono a questa regione, quindi vengono saltate */
                c+=b;
            } else {
                c++;
            }
        }
    }

    /* Output regioni con parametri massimi */
    printf("Max Base: ");
    stampaDatiRegione(r_maxb);
    printf("Max Area: ");
    stampaDatiRegione(r_maxa);
    printf("Max Altezza: ");
    stampaDatiRegione(r_maxh);
    
    return 0;
}

/**
 * La funzione individua tutte le regioni presenti nella matrice m e le inserisce in un array di stutture Regione passato come parametro.
 * Parametri:
 * - regioni[]: Array su cui andranno salvate le regioni
 * - m[][]: matrice letta da file
 * - nr: numero di righe
 * - nc: numero di colonne
 * - max_b: indice di regione[] contenente la regione con la base massima. Passato per referenza.
 * - max_h: indice di regione[] contenente la regione con l'altezza massima. Passato per referenza.
 * - max_a: indice di regione[] contenente la regione con l'area massima. Passato per referenza.
 * Return: void
 */ 
int leggiMatrice(int m[MAXR][MAXR], int* nr, int* nc) {
    FILE* fp = fopen(FILE_MAPPA, "r");
    if(!fp)
        return 0;
    
    fscanf(fp, "%d %d", nr, nc);

    for(int i=0; i<*nr; i++)
        for(int j=0; j<*nc; j++)
            fscanf(fp, "%d", &m[i][j]);
}

/** 
 * La funzione restituisce 1 se le coordinate (r,c) sono l'estremo superiore sinistro di una regione, altrimenti restituisce 0
 */ 
int riconosciRegione(int m[MAXR][MAXR], int nr, int nc, int r, int c, int* b, int* h) {
    /*
    - Se la cella corrente è pari a 0 non si è dentro una regione (condizione 1)
    - Se la cella superiore è pari a 1 si è dentro una regione ma le coordinate passate non sono l'estremo superiore sinistro (condizioni 2 e 3)
    Non è necessario in questo caso controllare se la cella a sinistra è pari a 1 perchè nel main vengono già saltate le prossime celle in orizzontale quando una regione viene individuata
    */
    if(m[r][c] == 0 || (r>0 && m[r-1][c] == 1))
        return 0;
    
    /* La cella è l'estremo superiore sinistro di una regione, si misurano base e altezza */
    int k;
    *b=0;
    *h=0;
    for(k=r; k<nr && m[k][c] != 0; k++)
        (*h)++;
    for(k=c; k<nc && m[r][k] != 0; k++)
        (*b)++;

    return 1;
}

/**
 * La funzione stampa in output i dati relativi a una regione r
 */ 
void stampaDatiRegione(Regione r) {
    printf("estr. sup. SX=<%d,%d>, b=%d, h=%d, Area=%d\n", r.estremo.x, r.estremo.y, r.base, r.altezza, r.base*r.altezza);
}

Regione inizializzaRegione(int x, int y, int b, int h) {
    Regione r;
    r.estremo.x = x;
    r.estremo.y = y;
    r.base = b;
    r.altezza = h;
    return r;
}