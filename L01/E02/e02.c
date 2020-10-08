#include <stdio.h>

#define MAX_NOMEFILE 20
#define MAX_PAROLA 30
#define MAX_RIGA 200

#define FILE_SORGENTE "sorgente.txt"
#define FILE_DIZIONARIO "dizionario.txt"
#define FILE_DESTINAZIONE "ricodificato.txt"

typedef struct {
    char parola[MAX_PAROLA];
    int codice;
} Ricodifica;

int cercaIndiceDizionario(const Ricodifica dizionario[], const int n_ricodifiche, const char s[], const int inizio, int* str_len);

int main() {
    
    //Apertura file dizionario e sorgente in lettura
    FILE* fp_src = fopen(FILE_SORGENTE, "r");
    FILE* fp_diz = fopen(FILE_DIZIONARIO, "r");
    if(!fp_src || !fp_diz) {
        printf("Errore in lettura file (%s; %s).\n", FILE_SORGENTE, FILE_DIZIONARIO);
        return -1;
    }

    //Apertura file destinazione in scrittura
    FILE* fp_dst = fopen(FILE_DESTINAZIONE, "w");
    if(!fp_dst) {
        printf("Errore in scrittura file (%s)", FILE_DESTINAZIONE);
    }

    //Estrazione numero di voci del dizionario
    int n_ricodifiche;
    fscanf(fp_diz, "%d", &n_ricodifiche);

    //Riempimento della struttura dati che sarà il dizionario
    Ricodifica dizionario[n_ricodifiche];
    for(int i=0; i<n_ricodifiche; i++)
        fscanf(fp_diz, " $%d$ %s", &(dizionario[i].codice), dizionario[i].parola);

    //Non serve più leggere dal file dizionario, per cui si chiude
    fclose(fp_diz);

    //Ciclo di lettura di ogni riga
    char riga[MAX_RIGA];
    while(fgets(riga, MAX_RIGA, fp_src) != NULL) {
        int i=0;
        /*
        Si effettua un ciclo di lettura carattere per carattere della riga appena estratta dal file sorgente (si termina il ciclo quando si incontra un terminatore).
        All'interno di ogni iterazione viene impostato il carattere in posizione i come inizio di una sottostringa.
        A questo punto viene controllato se la sottostringa (di lunghezza variabile) coincide con una delle parole contenute nel dizionario (tramite la funzione cercaIndiceDizionario).
        In caso positivo viene scritto su file il codice relativo alla voce del dizionario (e i caratteri facenti parte della sottostringa vengono saltati, incrementando i della lunghezza della stessa).
        In caso non siano state trovate voci del dizionario, allora viene scritto su file il singolo carattere in posizione i e si procede al successivo.
        */
        while(i<MAX_RIGA && riga[i] != '\0') {
            int str_len;
            int diz_idx = cercaIndiceDizionario(dizionario, n_ricodifiche, riga, i, &str_len);
            if(diz_idx >= 0) {
                fprintf(fp_dst, "$%d$", dizionario[diz_idx].codice);
                i+=str_len;
            } else {
                fputc(riga[i], fp_dst);
                i++;
            }
        }
        //fputc('\n', fp_dst);
    }

    //Chiusura file sorgente e destinazione
    fclose(fp_dst);
    fclose(fp_src);

    return 0;
}

/**
 * La seguente funzione verifica la presenza di una voce del dizionario come sottostringa di s, il cui primo carattere è indicato da un dato indice (inizio) e, in caso positivo, restituisce il relativo indice della struttura.
 * Parametri:
 * - dizionario: Array di strutture contenenti le parole nel dizionario e il loro relativo codice
 * - n_ricodifiche: Lunghezza del dizionario
 * - s: Stringa in cui cercare la presenza di parole nel dizionario (in questo caso, una riga del file sorgente.txt)
 * - inizio: indice da cui iniziare a cercare
 * - str_len: passato per referenza; in caso la voce del dizionario venga trovata, contiene la lunghezza della sottostringa da sostituire
 * Valore di ritorno:
 * - Indice dell'array dizionario che punta la prima occorrenza trovata
 * - -1 se non si verifica alcuna occorrenza
*/
int cercaIndiceDizionario(const Ricodifica dizionario[], const int n_ricodifiche, const char s[], const int inizio, int* str_len) {

    for(int i=0; i<n_ricodifiche; i++) {
        int diverso = 0; //Booleana
        *str_len = 0;
        //Si effettua un ciclo carattere per carattere della parola del dizionario corrente, fermandosi al primo carattere diverso da quello corrente di s (oppure all'ultimo carattere)
        while(!diverso && dizionario[i].parola[*str_len] != '\0') {
            if(s[inizio+(*str_len)] != dizionario[i].parola[*str_len])
                diverso = 1;
            (*str_len)++;
        }
        //Se non è stato individuato alcun carattere diverso (quindi se si è usciti dal ciclo per la seconda/terza condizione), la voce del dizionario è stata trovata
        if(!diverso)
            return i;
    }

    return -1;
}