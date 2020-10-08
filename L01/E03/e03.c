#include <stdio.h>
#include <string.h>

#define MAX_FILE 20
#define MAX_MATRICE 30
#define MAX_SELETTORE 8
#define MAX_DIREZIONE 9

FILE* getFile();
int validaInput(char selettore_str[], int indice, char direzione_str[], int posizioni, int righe, int colonne, char* selettore, char* direzione);
int ruotaMatrice(int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc, char selettore, int indice, char direzione, int posizioni);
void stampaMatrice(int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc);

int main()  {

    FILE* fp = getFile();
    if(!fp)  {
        printf("Errore in lettura del file.\n");
        return -1;
    }

    //Acquisizione numero righe e colonne
    int nr, nc;
    fscanf(fp, "%d %d", &nr, &nc);

    //Lettura matrice da file
    int m[MAX_MATRICE][MAX_MATRICE];
    for(int i=0; i<nr; i++)
        for(int j=0; j<nc; j++)
            fscanf(fp, "%d", &m[i][j]);
            
    printf("Matrice input:\n");
    stampaMatrice(m, nr, nc);
    printf("\n");
    
    //Ciclo input ripetuti all'utente
    char selettore, direzione;
    int indice, posizioni;
    char selettore_str[MAX_SELETTORE], direzione_str[MAX_DIREZIONE]; 
    while(1) {
        printf("Inserire una linea di comando (oppure \"esci\" per uscire).\nFormato: <selettore> <indice> <direzione> <posizioni>.\n");
        scanf("%s %d %s %d", selettore_str, &indice, direzione_str, &posizioni);

        //Validazione dell'input. In caso sia invalido, viene chiesto all'utente di inserire un nuovo comando.
        if(!validaInput(selettore_str, indice, direzione_str, posizioni, nr, nc, &selettore, &direzione)) {
            printf("Uno o più dati inseriti sono invalidi.\n\n");
            continue;
        }

        ruotaMatrice(m, nr, nc, selettore, indice, direzione, posizioni);

        printf("Matrice corrente:\n");
        stampaMatrice(m, nr, nc);
        printf("\n");

    }

}

FILE* getFile() {
    char nomeFile[MAX_FILE];
    printf("Inserire il nome del file: ");
    scanf("%s", nomeFile);
    return fopen(nomeFile, "r");
}

int validaInput(char selettore_str[], int indice, char direzione_str[], int posizioni, int righe, int colonne, char* selettore, char* direzione) {
    // Controllo sulla stringa selettore. In caso sia valida, viene impostato il parametro char selettore passato per referenza
    if(strcmp(selettore_str, "riga") == 0)
        *selettore = 'R';
    else if(strcmp(selettore_str, "colonna") == 0)
        *selettore = 'C';
    else
        return 0;

    //Controllo su indice direzione. In caso la direzione sia valida, viene impostato il parametro char direzione passato per referenza
    if(indice <= 0)
        return 0;
    if(*selettore == 'R') {
        if(indice > righe)
            return 0;
        if(strcmp(direzione_str, "destra") == 0)
            *direzione = 'R';
        else if(strcmp(direzione_str, "sinistra") == 0)
            *direzione = 'L';
        else
            return 0;
    } else {
        if(indice > colonne)
            return 0;
        if(strcmp(direzione_str, "su") == 0)
            *direzione = 'U';
        else if(strcmp(direzione_str, "giu") == 0)
            *direzione = 'D';
        else
            return 0;
    }

    //Controllo su posizioni
    if(posizioni < 0)
        return 0;

    return 1;
}

int ruotaMatrice(int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc, char selettore, int indice, char direzione, int posizioni) {
    int i;
    indice--; //L'indice inserito dall'utente parte da 1, per cui si decrementa di 1 prima di iniziare
    
    int limit = selettore == 'R' ? nc : nr;
    if(posizioni > limit)
        posizioni = posizioni % limit;
    int tmp[posizioni];
    
    if(selettore == 'R') {
        if(direzione == 'R') {
            //ROTAZIONE A DESTRA
            for(i=0; i<posizioni; i++)
                tmp[i] = m[indice][i+nc-posizioni];
            for(i=nr-posizioni-1; i>=0; i--)
                m[indice][i+posizioni] = m[indice][i];
            for(i=0; i<posizioni; i++)
                m[indice][i] = tmp[i];
        } else {
            //ROTAZIONE A SINISTRA
            for(i=0; i<posizioni; i++)
                tmp[i] = m[indice][i];
            for(i=posizioni; i<nc;i++)
                m[indice][i-posizioni] = m[indice][i];
            for(i=0; i<posizioni; i++)
                m[indice][nc-posizioni+i] = tmp[i];
        }
    } else {
        if(direzione == 'D') {
            //ROTAZIONE GIU
            for(i=0; i<posizioni; i++)
                tmp[i] = m[i+nr-posizioni][indice];
            for(i=nr-posizioni-1; i>=0; i--)
                m[i+posizioni][indice] = m[i][indice];
            for(i=0; i<posizioni; i++)
                m[i][indice] = tmp[i];
        } else {
            //ROTAZIONE SU
            for(i=0; i<posizioni; i++)
                tmp[i] = m[i][indice];
            for(i=posizioni; i<nr;i++)
                m[i-posizioni][indice] = m[i][indice];
            for(i=0; i<posizioni; i++)
                m[nr-posizioni+i][indice] = tmp[i];
        }
    }
}

void stampaMatrice(int m[MAX_MATRICE][MAX_MATRICE], int nr, int nc) {
    for(int i=0; i<nr; i++) {
        for(int j=0; j<nc; j++)
            printf("%d ", m[i][j]);
        printf("\n");
    }
}