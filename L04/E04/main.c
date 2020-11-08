#include <stdio.h>
#include <string.h>

#define MAX_FILE 20
#define MAX_TRATTA 30
#define MAX_COMANDO 15
#define FILE_LOG "log.txt"

typedef struct {
    int g;
    int m;
    int a;
} Data;

typedef struct {
    int h;
    int m;
    int s;
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
    r_stampa,
    r_ordina_data,
    r_ordina_cod,
    r_ordina_partenza,
    r_ordina_arrivo,
    r_cerca,
    r_fine,
    r_sconosciuto
} comando_e;

comando_e leggiComando();
void selezionaDati(comando_e cmd, Tratta tratte[], int n_tratte, comando_e *ultimo_ordinamento);
void stampa(Tratta tratte[], int n_tratte);
void ordina(Tratta tratte[], int n_tratte, comando_e cmd);
int confronta(Tratta t1, Tratta t2, comando_e cmd);
void cerca(Tratta tratte[], int n_tratte, comando_e ultimo_ordinamento);
void ricercaDicotomica(char cerca[], Tratta tratte[], int n_tratte);
int ricercaDicotomica_R(char cerca[], Tratta tratte[], int l, int r);
void ricercaLineare(char cerca[], Tratta tratte[], int n_tratte);
int tratteOrdinate(Tratta tratte[], int n_tratte);
int data_toint(Data d);
int ora_toint(Ora o);
void riempiTratte(FILE* fp, Tratta tratte[], int n_tratte);
void stampaTratta(Tratta t, FILE* fp);

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
    /* Se le tratte appena prese in input sono già ordinate per stazione di partenza, si può già da subito effettuare la ricerca dicotomica. */
    comando_e ultimo_ordinamento = tratteOrdinate(tratte, n_tratte) ? r_ordina_partenza : r_sconosciuto;

    do {
        cmd = leggiComando();
        selezionaDati(cmd, tratte, n_tratte, &ultimo_ordinamento);
    } while(cmd != r_fine);

    return 0;
}

comando_e leggiComando() {
    char cmd_str[MAX_COMANDO+1];
    char* tabella[r_sconosciuto] = {"stampa", "ordina_data", "ordina_cod", "ordina_partenza", "ordina_arrivo", "cerca", "fine"};
    comando_e cmd = r_stampa;

    printf("Inserire un comando: ");
    scanf("%s", cmd_str);
    
    while(cmd < r_sconosciuto && strcmp(cmd_str, tabella[cmd])!=0)
        cmd++;

    return cmd;
}

void selezionaDati(comando_e cmd, Tratta tratte[], int n_tratte, comando_e *ultimo_ordinamento) {
    switch (cmd) {
        case r_stampa:
            stampa(tratte, n_tratte);
            break;
        case r_ordina_data:
        case r_ordina_cod:
        case r_ordina_partenza:
        case r_ordina_arrivo:
            ordina(tratte, n_tratte, cmd);
            *ultimo_ordinamento = cmd;
            break;
        case r_cerca:
            cerca(tratte, n_tratte, *ultimo_ordinamento);
            break;
        case r_fine:
            break;
        default:
            printf("Comando sconosciuto.\n");
    }
}

void stampa(Tratta tratte[], int n_tratte) {
    printf("Vuoi stampare i dati su schermo o file (s/f)? ");
    getchar(); /* Istruzione extra per catturare il linebreak */
    char mod = getchar();

    FILE* fp;
    switch(mod) {
        case 's':
            fp = stdout;
            break;
        case 'f':
            printf("Inserire nome del file: ");
            char nomefile[MAX_FILE+1];
            scanf("%s", nomefile);

            fp = fopen(nomefile, "w");
            if(!fp) {
                printf("Errore in scrittura su file.\n");
                return;
            }
            break;
        default:
            printf("Modalità di stampa invalida.\n");
            return;
    }

    if(fp != stdout)
        fprintf(fp, "%d\n", n_tratte);

    for(int i=0; i<n_tratte; i++)
        stampaTratta(tratte[i], fp);

    if(fp != stdout) {
        printf("Salvato nuovo file di log!\n");
        fclose(fp);
    }
}

/* BUBBLE SORT */
void ordina(Tratta tratte[], int n_tratte, comando_e cmd) {
    Tratta tmp;
    for(int i=1; i<n_tratte-1; i++)
        for(int j=0; j<n_tratte-i; j++)
            if(confronta(tratte[j], tratte[j+1], cmd)>0) {
                tmp = tratte[j];
                tratte[j] = tratte[j+1];
                tratte[j+1] = tmp;
            }
    printf("Ordinato!\n");
}

int confronta(Tratta t1, Tratta t2, comando_e cmd) {
    switch (cmd) {
        case r_ordina_data:  {
            /* CONFRONTO SULLA DATA */
            int data1 = data_toint(t1.data);
            int data2 = data_toint(t2.data);
            if(data1 != data2)
                return data1-data2;

            /* Se le date sono uguali si confronta l'ora */
            int ora1 = ora_toint(t1.ora_partenza);
            int ora2 = ora_toint(t2.ora_partenza);
            if(ora1 != ora2)
                return ora1-ora2;

            return 0;
        }
        case r_ordina_cod:
            return strcmp(t1.codice, t2.codice);
        case r_ordina_partenza:
            return strcmp(t1.partenza, t2.partenza);
        case r_ordina_arrivo:
            return strcmp(t1.destinazione, t2.destinazione);
        default:
            return 0;
    }
}

void cerca(Tratta tratte[], int n_tratte, comando_e ultimo_ordinamento) {
    char cerca[MAX_TRATTA+1];
    printf("Inserire termine di ricerca: ");
    scanf("%s", cerca);
    
    int res;
    if(ultimo_ordinamento == r_ordina_partenza)
        ricercaDicotomica(cerca, tratte, n_tratte);
    else
        ricercaLineare(cerca, tratte, n_tratte);

}

void ricercaDicotomica(char cerca[], Tratta tratte[], int n_tratte) {
    int search_idx = ricercaDicotomica_R(cerca, tratte, 0, n_tratte-1);

    printf("Tratte trovate (ricerca dicotomica): \n");
    if(search_idx<0)
        return;

    stampaTratta(tratte[search_idx], stdout);

    int i;

    /* STAMPA TRATTE CON STESSA STAZIONE DI PARTENZA A SINISTRA */
    for(i=search_idx-1; i>=0 && strstr(tratte[i].partenza, cerca) == tratte[i].partenza; i--)
        stampaTratta(tratte[i], stdout);

    /* STAMPA TRATTE CON STESSA STAZIONE DI PARTENZA A DESTRA */
    for(i=search_idx+1; i<n_tratte && strstr(tratte[i].partenza, cerca) == tratte[i].partenza; i++)
        stampaTratta(tratte[i], stdout);
}

int ricercaDicotomica_R(char cerca[], Tratta tratte[], int l, int r) {
    if(l==r)
        return -1;

    int med = (r+l)/2;
    int res = strncmp(cerca, tratte[med].partenza, strlen(cerca));
    if(res == 0)
        return med;
    if(res>0)
        return ricercaDicotomica_R(cerca, tratte, med, r);

    return ricercaDicotomica_R(cerca, tratte, l, med);
}

void ricercaLineare(char cerca[], Tratta tratte[], int n_tratte) {
    printf("Tratte trovate (ricerca lineare): \n");
    for(int i=0; i<n_tratte; i++)
        if(strstr(tratte[i].partenza, cerca) == tratte[i].partenza)
            stampaTratta(tratte[i], stdout);
}

int tratteOrdinate(Tratta tratte[], int n_tratte) {
    for(int i=0; i<n_tratte-1; i++)
        if(confronta(tratte[i], tratte[i+1], r_ordina_partenza)>0)
            return 0;
    return 1;
}

/**
 * La funzione, a partire da una struttura Data, restituisce la data in formato intero (AAAAMMGG), in modo da rendere possibile il confronto
 */
int data_toint(Data d) {
    return d.a * 10000 + d.m * 100 + d.g;
}

/**
 * La funzione, a partire da una struttura Ora, restituisce l'ora in formato intero (HHMMSS), in modo da rendere possibile il confronto
 */
int ora_toint(Ora o) {
    return o.h*10000 + o.m*100 + o.s;
}

void riempiTratte(FILE* fp, Tratta tratte[], int n_tratte) {
    for(int i=0; i<n_tratte; i++)
        fscanf(fp, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
            tratte[i].codice,
            tratte[i].partenza,
            tratte[i].destinazione,
            &tratte[i].data.a,
            &tratte[i].data.m,
            &tratte[i].data.g,
            &tratte[i].ora_partenza.h,
            &tratte[i].ora_partenza.m,
            &tratte[i].ora_partenza.s,
            &tratte[i].ora_arrivo.h,
            &tratte[i].ora_arrivo.m,
            &tratte[i].ora_arrivo.s,
            &tratte[i].ritardo);
}

void stampaTratta(Tratta t, FILE* fp) {
    fprintf(fp, "%s %s %s %d/%02d/%d %d:%02d:%02d %d:%02d:%02d %d\n",
        t.codice,
        t.partenza,
        t.destinazione,
        t.data.g,
        t.data.m,
        t.data.a,
        t.ora_partenza.h,
        t.ora_partenza.m,
        t.ora_partenza.s,
        t.ora_arrivo.h,
        t.ora_arrivo.m,
        t.ora_arrivo.s,
        t.ritardo);
}