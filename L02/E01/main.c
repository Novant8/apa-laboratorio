#include <stdio.h>

#define MAX_MATRICE 50
#define FILE_MAPPA "mappa.txt"

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate estremo;
    int base;
    int altezza;
} Regione;

int riempiMatrice(int m[MAX_MATRICE][MAX_MATRICE], int* nr, int* nc);
void stampaDatiRegione(Regione r);
void individuaRegioni(Regione regioni[], int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc, int* max_b, int* max_h, int* max_a);
Regione inizializzaRegione(int x, int y, int b, int h);

int main() {

    int nr,nc,m[MAX_MATRICE][MAX_MATRICE];
    if(!riempiMatrice(m, &nr, &nc)) {
        printf("Errore in lettura del file.\n");
        return -1;
    }

    int max_h, max_b, max_a;
    /*
        Il numero massimo possibile di regioni individuabili è dato nel caso la matrice contenga 0 e 1 disposti "a scacchiera".
        Esempio:
        1 0 1 0 1 0
        0 1 0 1 0 1
        1 0 1 0 1 0
        0 1 0 1 0 1
        Il numero di regioni di questo tipo di matrice è pari alla metà della sua "area": (righe*colonne)/2.
    */
    Regione regioni[(nr*nc)/2];

    individuaRegioni(regioni, m, nr, nc, &max_b, &max_h, &max_a);

    //Output dati regioni
    printf("Max Base: ");
    stampaDatiRegione(regioni[max_b]);
    printf("Max Area: ");
    stampaDatiRegione(regioni[max_a]);
    printf("Max Altezza: ");
    stampaDatiRegione(regioni[max_h]);
    
    return 0;
}

/**
 * La funzione legge la matrice da file e memorizza in nr ed nc il numero effettivo di righe e colonne
 * Return:
 * - 1 se non ci sono stati errori
 * - 0 se non si è riuscito ad aprire il file 
 */ 
int riempiMatrice(int m[MAX_MATRICE][MAX_MATRICE], int* nr, int* nc) {
    /* Apertura file */
    FILE* fp = fopen(FILE_MAPPA, "r");
    if(!fp)
        return 0;
    
    /* Lettura dimensioni effettive della matrice */
    fscanf(fp, "%d %d", nr, nc);

    /* Lettura matrice */
    for(int i=0; i<*nr; i++)
        for(int j=0; j<*nc; j++)
            fscanf(fp, "%d", &m[i][j]);
    
    /* Chiusura file */
    fclose(fp);
    return 1;
}

void stampaDatiRegione(Regione r) {
    printf("estr. sup. SX=<%d,%d>, b=%d, h=%d, Area=%d\n", r.estremo.x, r.estremo.y, r.base, r.altezza, r.base*r.altezza);
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
void individuaRegioni(Regione regioni[], int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc, int* max_b, int* max_h, int* max_a) {
    int n_regioni = 0;
    *max_b = 0;
    *max_h = 0;
    *max_a = 0;
    int j;
    for(int i=0; i<nr; i++) {
        j=0;
        while(j<nc) {
            /*
            Aggiungi una nuova regione se:
                - La cella corrente è nera (condizione 1)
                - La cella non è già parte di un'altra regione (condizioni 2 e 3): per questo basta controllare se la cella al di sopra di quella corrente non sia nera (non è necessario controllare la cella di fianco, dato che viene già incrementata j all'interno dell'if) 
            */
            if (m[i][j] == 1 && (i==0 || m[i-1][j] == 0)) {
                //AGGIUNGI NUOVA REGIONE
                regioni[n_regioni] = inizializzaRegione(i,j,0,0);

                /* Calcolo di base e altezza della regione appena trovata */
                int k;
                for(k=i; k<nr && m[k][j] != 0; k++)
                    regioni[n_regioni].altezza++;
                for(k=j; k<nc && m[i][k] != 0; k++)
                    regioni[n_regioni].base++;

                /* Individua se la regione corrente ha uno o più dati massimi */
                int area = regioni[n_regioni].base * regioni[n_regioni].altezza;
                if(regioni[n_regioni].base > regioni[*max_b].base)
                    *max_b = n_regioni;
                if(regioni[n_regioni].altezza > regioni[*max_h].altezza)
                    *max_h = n_regioni;
                if(area > regioni[*max_a].base * regioni[*max_a].altezza)
                    *max_a = n_regioni;

                /* L'indice j viene incrementato della lunghezza della base della regione trovata. Le prossime celle in orizzontale della riga corrente vengono saltate. */
                j+=regioni[n_regioni].base;

                n_regioni++;
            } else {
                //REGIONE NON TROVATA
                j++;
            }
        }
    }
}

Regione inizializzaRegione(int x, int y, int b, int h) {
    Regione r;
    r.estremo.x = x;
    r.estremo.y = y;
    r.base = b;
    r.altezza = h;
    return r;
}