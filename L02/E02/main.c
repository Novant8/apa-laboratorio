#include <stdio.h>
#include <string.h>

#define MAX_FILE 20
#define MAX_TRATTA 30 
#define MAX_COMANDO 11
#define FILE_LOG "log.txt"

typedef struct {
    int giorno;
    int mese;
    int anno;
} Data;

typedef struct {
    int ora;
    int minuti;
    int secondi;
} Ora;

typedef struct {
    char codice[MAX_TRATTA];
    char partenza[MAX_TRATTA];
    char destinazione[MAX_TRATTA];
    Data data;
    Ora ora_partenza;
    Ora ora_arrivo;
    int ritardo;
} Tratta;

typedef enum {
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
    r_fine,
    r_sconosciuto
} comando_e;

comando_e leggiComando();
void selezionaDati(comando_e cmd, Tratta tratte[], int n_tratte);
void date(Tratta tratte[], int n_tratte, int bool_ritardo);
void partenza(Tratta tratte[], int n_tratte);
void capolinea(Tratta tratte[], int n_tratte);
void ritardo_tot(Tratta tratte[], int n_tratte);
int data_toint(Data d);
void riempiTratte(FILE* fp, Tratta tratte[], int n_tratte);
void stampaTratta(Tratta t);

int main() {

    FILE* fp = fopen(FILE_LOG, "r");
    if(!fp) {
        printf("Errore in lettura del file di log.\n");
        return -1;
    }

    int n_tratte;
    fscanf(fp, "%d", &n_tratte);

    Tratta tratte[n_tratte];
    riempiTratte(fp, tratte, n_tratte);

    comando_e cmd;

    do {
        cmd = leggiComando();
        selezionaDati(cmd, tratte, n_tratte);
    } while(cmd != r_fine);

    return 0;
}

comando_e leggiComando() {
    char cmd_str[MAX_COMANDO+1];
    char* tabella[r_sconosciuto] = {"date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine" };
    comando_e cmd = r_date;

    printf("Inserire un comando: ");
    scanf("%s", cmd_str);
    
    while(cmd < r_sconosciuto && strcmp(cmd_str, tabella[cmd])!=0)
        cmd++;

    return cmd;
}

void selezionaDati(comando_e cmd, Tratta tratte[], int n_tratte) {
    switch (cmd) {
        case r_date:
            date(tratte, n_tratte, 0);
        break;
        case r_partenza:
            partenza(tratte, n_tratte);
        break;
        case r_capolinea:
            capolinea(tratte, n_tratte);
            break;
        case r_ritardo:
            date(tratte, n_tratte, 1);
        break;
        case r_ritardo_tot:
            ritardo_tot(tratte, n_tratte);
        break;
        case r_fine:
        break;
        default:
            printf("Comando sconosciuto.\n");
    }
}

/**
 * Dato che la procedura nei comandi r_date e r_ritardo è pressoché identica, questa funzione andrà gestire entrambi i comandi.
 * Il flag bool_ritardo indica quale dei due comandi è stato invocato:
 * - 0 se r_date
 * - 1 se r_ritardo
 */ 
void date(Tratta tratte[], int n_tratte, int bool_ritardo) {
    const int N = 2;
    Data date_str[N];
    int date_int[N];
    const char* DATE_DESC[2] = {"partenza", "arrivo"};
    for(int i=0; i<N; i++) {
        printf("Inserire data di %s (formato: gg/mm/AAAA): ", DATE_DESC[i]);
        scanf("%d/%d/%d", &date_str[i].giorno, &date_str[i].mese, &date_str[i].anno);
        date_int[i] = data_toint(date_str[i]);
    }

    if(date_int[0] > date_int[1]) {
        printf("Data di partenza più avanti della data di arrivo!\n\n");
        return;
    }

    printf("Corse partite tra %d/%d/%d e %d/%d/%d %s:\n\n", date_str[0].giorno, date_str[0].mese, date_str[0].anno,
                                                            date_str[1].giorno, date_str[1].mese, date_str[1].anno,
                                                            bool_ritardo ? "e arrivate in ritardo" : "");
    for(int i=0; i<n_tratte; i++) {
        int data_tratta = data_toint(tratte[i].data);
        if(data_tratta >= date_int[0] && data_tratta <= date_int[1] && (!bool_ritardo || tratte[i].ritardo > 0))
            stampaTratta(tratte[i]);
    }
}

void partenza(Tratta tratte[], int n_tratte) {
    
    char stazione[MAX_TRATTA];

    printf("Inserire la stazione desiderata: ");
    scanf("%s", stazione);

    printf("Tratte partite da %s:\n", stazione);
    for(int i=0; i<n_tratte; i++) {
        if(strcmp(stazione, tratte[i].partenza) == 0)
            stampaTratta(tratte[i]);
    }

}

void capolinea(Tratta tratte[], int n_tratte) {
    char stazione[MAX_TRATTA];

    printf("Inserire la stazione desiderata: ");
    scanf("%s", stazione);

    printf("Tratte facenti capolinea a %s:\n", stazione);
    for(int i=0; i<n_tratte; i++) {
        if(strcmp(stazione, tratte[i].destinazione) == 0)
            stampaTratta(tratte[i]);
    }
}

void ritardo_tot(Tratta tratte[], int n_tratte) {
    
    int ritardo_complessivo = 0;
    char cod_tratta[MAX_TRATTA];

    printf("Inserire codice tratta: ");
    scanf("%s", cod_tratta);

    for(int i=0; i<n_tratte; i++) {
        if(strcmp(cod_tratta, tratte[i].codice) == 0)
            ritardo_complessivo+=tratte[i].ritardo;
    }

    printf("Nella tratta %s risulta un ritardo complessivo di %d minuti.\n\n", cod_tratta, ritardo_complessivo);

}

/**
 * La funzione, a partire da una struttura Data, restituisce la data in formato intero, in modo da rendere 
 */
int data_toint(Data d) {
    return d.anno * 10000 + d.mese * 100 + d.giorno;
}

void riempiTratte(FILE* fp, Tratta tratte[], int n_tratte) {
    for(int i=0; i<n_tratte; i++)
        fscanf(fp, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
            tratte[i].codice,
            tratte[i].partenza,
            tratte[i].destinazione,
            &tratte[i].data.anno,
            &tratte[i].data.mese,
            &tratte[i].data.giorno,
            &tratte[i].ora_partenza.ora,
            &tratte[i].ora_partenza.minuti,
            &tratte[i].ora_partenza.secondi,
            &tratte[i].ora_arrivo.ora,
            &tratte[i].ora_arrivo.minuti,
            &tratte[i].ora_arrivo.secondi,
            &tratte[i].ritardo);
}

void stampaTratta(Tratta t) {
    printf("Tratta %s:\nStazione di partenza: %s\nStazione di arrivo: %s\nData: %d/%d/%d\nOra partenza: %d:%02d:%02d\nOra arrivo: %d:%02d:%02d\nRitardo: %d min.\n\n",
        t.codice,
        t.partenza,
        t.destinazione,
        t.data.giorno,
        t.data.mese,
        t.data.anno,
        t.ora_partenza.ora,
        t.ora_partenza.minuti,
        t.ora_partenza.secondi,
        t.ora_arrivo.ora,
        t.ora_arrivo.minuti,
        t.ora_arrivo.secondi,
        t.ritardo);
}