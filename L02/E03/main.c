#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_RIGA 200
#define MAX_PAROLA 25
#define MAX_SEQUENZA 5
#define MAX_OCCORRENZE 10

#define FILE_SEQUENZE "sequenze.txt"
#define FILE_TESTO "testo.txt"

typedef struct {
    int pos;
    char parola[MAX_PAROLA+1];
} Occorrenza;

typedef struct {
    char seq[MAX_SEQUENZA];
    int n_occorrenze;
    Occorrenza occorrenze[MAX_OCCORRENZE];
} Sequenza;

int leggiParola(char riga[MAX_RIGA], char parola[MAX_PAROLA], int* pos_riga);
int sequenzaInParola(char seq[MAX_SEQUENZA], char parola[MAX_PAROLA]);
void stampaOccorrenze(Sequenza sequenze[], int n_sequenze);
Occorrenza inizializzaOccorrenza(int pos, char parola[MAX_PAROLA]);

int main() {

    FILE* fp_sq = fopen(FILE_SEQUENZE, "r");
    FILE* fp_tx = fopen(FILE_TESTO, "r");

    if(!fp_sq || !fp_tx) {
        printf("Errore durante la lettura dei file di input.\n");
        return 1;
    }
    
    int n_sequenze;
    fscanf(fp_sq, "%d", &n_sequenze);

    /* Memorizza tutte le sequenze trovate nel file nell'array di strutture Sequenza */
    Sequenza sequenze[n_sequenze];
    int i;
    for(i=0; i<n_sequenze; i++) {
        fscanf(fp_sq, "%s", sequenze[i].seq);
        sequenze[i].n_occorrenze = 0; /* Azzera il numero di occorrenze della sequenza */
    }
    
    char riga[MAX_RIGA+1];
    char parola[MAX_PAROLA+1];
    int n_parole = 0;
    int occ_attuale; /* Contiene l'indice di occorrenza attuale di una sequenza (dopo che questa è stata trovata). Usata per rendere il codice di sotto più pulito. */
    int pos_riga;
    Occorrenza occorrenza;
    while(fgets(riga, MAX_RIGA, fp_tx)) {
        pos_riga = 0;
        while(leggiParola(riga, parola, &pos_riga)) {
            if(parola[0] != '\0') { /* Se la parola corrente restituita da leggiParola non è vuota */
                occorrenza = inizializzaOccorrenza(n_parole, parola); /* La parola viene inserita in una struttura Occorrenza */
                /* Controllo per ogni sequenza se è contenuta nella parola corrente */
                for(i=0; i<n_sequenze; i++)
                    if(sequenze[i].n_occorrenze < MAX_OCCORRENZE && sequenzaInParola(sequenze[i].seq, parola)) {
                        occ_attuale = sequenze[i].n_occorrenze;
                        sequenze[i].occorrenze[occ_attuale] = occorrenza;
                        sequenze[i].n_occorrenze++;
                    }
                n_parole++;
            }
        }
    }
    
    fclose(fp_tx);

    stampaOccorrenze(sequenze, n_sequenze);

    return 0;
}

/**
 * La funzione, a partire da una certa posizione, specifica se la riga è terminata e, in caso contrario, salva in una stringa la prossima parola
 * Parametri:
 * - riga[]: array contenente la riga corrente
 * - parola[]: array in cui verrà salvata la prima parola trovata
 * - pos_riga: indice dell'array riga[] da cui cominciare a cercare la nuova parola. Passato per referenza, in quanto tiene conto della posizione corrente nella riga anche per il main
 * Return:
 * - 0 se la riga è terminata
 * - 1 se la riga non è terminata
 */
int leggiParola(char riga[MAX_RIGA], char parola[MAX_PAROLA], int* pos_riga) {
    int i=0;

    /* Se la posizione corrente nella riga è un terminatore allora la riga è terminata. */
    if(riga[*pos_riga] == '\0')
        return 0;

    /* Inserisci nella parola ogni carattere alfanumerico trovato. Fermati al primo carattere non alfanumerico. */
    while(isalnum(riga[*pos_riga]))
        parola[i++] = riga[(*pos_riga)++];
    parola[i] = '\0';
    (*pos_riga)++;

    return 1;
}

/**
 * La funzione verifica se una sequenza è contenuta in una parola. In sostanza è un'implementazione semplificata di strstr.
 * Return:
 * - 0 se la sequenza non è stata trovata
 * - 1 se la sequenza è stata trovata
 */ 
int sequenzaInParola(char seq[MAX_SEQUENZA], char parola[MAX_PAROLA]) {
    int i, j, diverso;
    
    i=0;
    while(parola[i] != '\0') {
        j=0;
        /* A partire dall'indice i della parola, analizza se i prossimi caratteri coincidono con quelli della sequenza */
        while (parola[i+j] != '\0' && tolower(parola[i+j]) == tolower(seq[j])) { /* Interrompi il ciclo quando il carattere nella parola è diverso da quello della sequenza (oppure appena si raggiunge il termine della parola) */
            j++;
            /* Se è stata scannerizzata tutta la parola senza uscire dal ciclo significa che la sequenza è presente */
            if(seq[j] == '\0')
                return 1;
        }
        i++;
    }

    return 0;
}

/**
 * La funzione stampa in output le occorrenze trovate per ogni sequenza.
 */ 
void stampaOccorrenze(Sequenza sequenze[], int n_sequenze) {
    for(int i=0; i<n_sequenze; i++) {
        if(sequenze[i].n_occorrenze > 0) {
            printf("La sequenza \"%s\" è contenuta in ", sequenze[i].seq);
            for(int j=0; j<sequenze[i].n_occorrenze; j++)
                printf("\"%s\" (posizione %d), ", sequenze[i].occorrenze[j].parola, sequenze[i].occorrenze[j].pos+1);
            printf("\n");
        } else {
            printf("La sequenza \"%s\" non è contenuta in alcuna parola.\n", sequenze[i].seq);
        }
        printf("\n");
    }
}

Occorrenza inizializzaOccorrenza(int pos, char parola[MAX_PAROLA]) {
    Occorrenza o;
    o.pos = pos;
    strcpy(o.parola, parola);
    return o;
}