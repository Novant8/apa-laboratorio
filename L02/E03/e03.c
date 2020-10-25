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

int getParola(char riga[MAX_RIGA], char parola[MAX_PAROLA], int* pos_riga);
int sequenzaInParola(char seq[MAX_SEQUENZA], char parola[MAX_PAROLA]);
void stampaOccorrenze(Sequenza sequenze[], int n_sequenze);

int main() {

    FILE* fp_sq = fopen(FILE_SEQUENZE, "r");
    FILE* fp_tx = fopen(FILE_TESTO, "r");

    if(!fp_sq || !fp_tx) {
        printf("Errore durante la lettura dei file di input.\n");
        return 1;
    }
    
    int n_sequenze;
    fscanf(fp_sq, "%d", &n_sequenze);

    Sequenza sequenze[n_sequenze];
    int i;
    for(i=0; i<n_sequenze; i++) {
        fscanf(fp_sq, "%s", sequenze[i].seq);
        sequenze[i].n_occorrenze = 0;
    }
    
    char riga[MAX_RIGA+1];
    char parola[MAX_PAROLA+1];
    int n_parole = 0;
    int sequenza;
    int occ_attuale;
    int pos_riga;
    while(fgets(riga, MAX_RIGA, fp_tx)) {
        pos_riga = 0;
        while(getParola(riga, parola, &pos_riga))
            if(parola[0] != '\0') {
                for(i=0; i<n_sequenze; i++)
                    if(sequenze[i].n_occorrenze < 10 && sequenzaInParola(sequenze[i].seq, parola)) {
                        occ_attuale = sequenze[i].n_occorrenze;
                        sequenze[i].occorrenze[occ_attuale].pos = n_parole;
                        strcpy(sequenze[i].occorrenze[occ_attuale].parola, parola);
                        sequenze[i].n_occorrenze++;
                    }
                n_parole++;
            }
    }
    
    fclose(fp_tx);

    stampaOccorrenze(sequenze, n_sequenze);

    return 0;
}

int getParola(char riga[MAX_RIGA], char parola[MAX_PAROLA], int* pos_riga) {
    int i=0;

    if(riga[*pos_riga] == '\0')
        return 0;

    while(isalnum(riga[*pos_riga]))
        parola[i++] = riga[(*pos_riga)++];
    parola[i] = '\0';
    (*pos_riga)++;

    return 1;
}

int sequenzaInParola(char seq[MAX_SEQUENZA], char parola[MAX_PAROLA]) {
    int i, j, diverso;
    int seq_len = strlen(seq);

    if(seq_len > strlen(parola))
        return 0;

    i=0;
    while(parola[i+seq_len-1] != '\0') {
        j=0;
        diverso = 0;
        while(seq[j] != '\0' && !diverso) {
            if(tolower(seq[j]) != tolower(parola[i+j]))
                diverso = 1;
            j++;
        }
        if(!diverso)
            return 1;
        i++;
    }

    return 0;
}

void stampaOccorrenze(Sequenza sequenze[], int n_sequenze) {
    for(int i=0; i<n_sequenze; i++) {
        if(sequenze[i].n_occorrenze > 0) {
            printf("La sequenza %s è contenuta in ", sequenze[i].seq);
            for(int j=0; j<sequenze[i].n_occorrenze; j++)
                printf("%s (posizione %d), ", sequenze[i].occorrenze[j].parola, sequenze[i].occorrenze[j].pos+1);
            printf("\n");
        } else {
            printf("La sequenza %s non è contenuta in alcuna parola.\n", sequenze[i].seq);
        }
        printf("\n");
    }
}