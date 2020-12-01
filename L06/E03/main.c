#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE 20
#define MAX_TRATTA 30
#define MAX_COMANDO 15
#define N_ORDINAMENTI 4

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
    r_ordina_data,
    r_ordina_cod,
    r_ordina_partenza,
    r_ordina_arrivo,
    r_leggi,
    r_stampa,
    r_cerca,
    r_fine,
    r_sconosciuto
} comando_e;

comando_e leggiComando();
void selezionaDati(comando_e cmd, int* n_tratte, Tratta** tratte, Tratta**** ordinamenti, int *tratte_ordinate_partenza, comando_e* ultimo_ordinamento);
void stampa(int n_tratte, Tratta* tratte[]);
void ordina(int n_tratte, Tratta* tratte[], comando_e cmd);
int confronta(Tratta t1, Tratta t2, comando_e cmd);
void cerca(int n_tratte, Tratta* tratte[], int tratte_ordinate_partenza);
void ricercaDicotomica(char cerca[], int n_tratte, Tratta* tratte[]);
int ricercaDicotomica_R(char cerca[], Tratta* tratte[], int l, int r);
void ricercaLineare(char cerca[], int n_tratte, Tratta* tratte[]);
int tratteOrdinatePartenza(Tratta tratte[], int n_tratte);
int data_toint(Data d);
int ora_toint(Ora o);
void riempiTratte(FILE* fp, int n_tratte, Tratta tratte[], Tratta*** ordinamenti);
void stampaTratta(Tratta t, FILE* fp);
void structAlloc(Tratta** tratte, Tratta**** ordinamenti, int n_tratte);
void structFree(Tratta tratte[], Tratta*** ordinamenti);
int leggiLog(Tratta** tratte, Tratta**** ordinamenti, int* n_tratte, int* tratte_ordinate_partenza);

int main() {

    int n_tratte, tratte_ordinate_partenza;
    Tratta* tratte = NULL;
    Tratta*** ordinamenti = NULL;
    if(!leggiLog(&tratte, &ordinamenti, &n_tratte, &tratte_ordinate_partenza)) {
        printf("Errore in lettura del file.\n");
        return -1;
    }

    comando_e cmd;
    comando_e ultimo_ordinamento = r_ordina_data;

    do {
        cmd = leggiComando();
        selezionaDati(cmd, &n_tratte, &tratte, &ordinamenti, &tratte_ordinate_partenza, &ultimo_ordinamento);
    } while(cmd != r_fine);

    structFree(tratte, ordinamenti);

    return 0;
}

comando_e leggiComando() {
    char cmd_str[MAX_COMANDO+1];
    char* tabella[r_sconosciuto] = {"ordina_data", "ordina_cod", "ordina_partenza", "ordina_arrivo", "leggi", "stampa", "cerca", "fine"};
    comando_e cmd = r_ordina_data;

    printf("Inserire un comando: ");
    scanf("%s", cmd_str);
    
    while(cmd < r_sconosciuto && strcmp(cmd_str, tabella[cmd])!=0)
        cmd++;

    return cmd;
}

void selezionaDati(comando_e cmd, int* n_tratte, Tratta** tratte, Tratta**** ordinamenti, int *tratte_ordinate_partenza, comando_e* ultimo_ordinamento) {
    switch (cmd) {
        case r_stampa:
            stampa(*n_tratte, (*ordinamenti)[*ultimo_ordinamento]);
            break;
        case r_ordina_data:
        case r_ordina_cod:
        case r_ordina_partenza:
        case r_ordina_arrivo:
            ordina(*n_tratte, *(*ordinamenti+cmd), cmd);
            *ultimo_ordinamento = cmd;
            if(cmd==r_ordina_partenza)
                *tratte_ordinate_partenza = 1;
            break;
        case r_leggi:
            leggiLog(tratte, ordinamenti, n_tratte, tratte_ordinate_partenza);
            break;
        case r_cerca:
            cerca(*n_tratte, (*ordinamenti)[r_ordina_partenza], *tratte_ordinate_partenza);
            break;
        case r_fine:
            break;
        default:
            printf("Comando sconosciuto.\n");
    }
}

void stampa(int n_tratte, Tratta* tratte[]) {
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
        stampaTratta(*tratte[i], fp);

    if(fp != stdout) {
        printf("Salvato nuovo file di log!\n");
        fclose(fp);
    }
}

/**
 * La funzione ordina le tratte mediante bubble sort.
 * Il dato sulla base di cui ordinare dipende dal comando che viene invocato, identificato dal parametro cmd
 */ 
void ordina(int n_tratte, Tratta* tratte[], comando_e cmd) {
    Tratta* tmp;
    for(int i=1; i<n_tratte-1; i++)
        for(int j=0; j<n_tratte-i; j++)
            if(confronta(*tratte[j], *tratte[j+1], cmd)>0) {
                tmp = tratte[j];
                tratte[j] = tratte[j+1];
                tratte[j+1] = tmp;
            }
    printf("Ordinato!\n");
}

/**
 * La funzione confronta due tratte in base a un dato che dipende dal comando che viene invocato, identificato dal parametro cmd
 */ 
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

void cerca(int n_tratte, Tratta* tratte[], int tratte_ordinate_partenza) {
    char cerca[MAX_TRATTA+1];
    printf("Inserire termine di ricerca: ");
    scanf("%s", cerca);
    
    int res;
    if(tratte_ordinate_partenza)
        ricercaDicotomica(cerca, n_tratte, tratte);
    else
        ricercaLineare(cerca, n_tratte, tratte);

}

void ricercaDicotomica(char cerca[], int n_tratte, Tratta* tratte[]) {
    int search_idx = ricercaDicotomica_R(cerca, tratte, 0, n_tratte-1);

    printf("Tratte trovate (ricerca dicotomica): \n");
    if(search_idx<0)
        return;

    stampaTratta(*tratte[search_idx], stdout);

    int i;

    /* STAMPA TRATTE CON STESSA STAZIONE DI PARTENZA A SINISTRA */
    for(i=search_idx-1; i>=0 && strstr(tratte[i]->partenza, cerca) == tratte[i]->partenza; i--)
        stampaTratta(*tratte[i], stdout);

    /* STAMPA TRATTE CON STESSA STAZIONE DI PARTENZA A DESTRA */
    for(i=search_idx+1; i<n_tratte && strstr(tratte[i]->partenza, cerca) == tratte[i]->partenza; i++)
        stampaTratta(*tratte[i], stdout);
}

int ricercaDicotomica_R(char cerca[], Tratta* tratte[], int l, int r) {
    if(l>r)
        return -1;

    int med = (r+l)/2;
    int res = strncmp(cerca, tratte[med]->partenza, strlen(cerca));
    if(res == 0)
        return med;
    if(res>0)
        return ricercaDicotomica_R(cerca, tratte, med+1, r);

    return ricercaDicotomica_R(cerca, tratte, l, med-1);
}

void ricercaLineare(char cerca[], int n_tratte, Tratta* tratte[]) {
    printf("Tratte trovate (ricerca lineare): \n");
    int cerca_len = strlen(cerca);
    for(int i=0; i<n_tratte; i++)
        if(strncmp(cerca, tratte[i]->partenza, cerca_len)==0)
            stampaTratta(*tratte[i], stdout);
}

/**
 * La funzione controlla se le tratte presenti in un array sono ordinate in modo crescente per stazione di partenza
 */ 
int tratteOrdinatePartenza(Tratta tratte[], int n_tratte) {
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

void riempiTratte(FILE* fp, int n_tratte, Tratta tratte[], Tratta*** ordinamenti) {
    for(int i=0; i<n_tratte; i++) {
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

        for(int j=0; j<N_ORDINAMENTI; j++)
            ordinamenti[j][i] = tratte+i;
    }
}

void stampaTratta(Tratta t, FILE* fp) {
    fprintf(fp, "%s %s %s %d/%02d/%d %02d:%02d:%02d %02d:%02d:%02d %d\n",
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

/**
 * La funzione esegue l'allocazione sia dell'array di struct tratte, sia della matrice di puntatori di struct ordinamenti 
 */ 
void structAlloc(Tratta** tratte, Tratta**** ordinamenti, int n_tratte) {
    if(*tratte != NULL)
        structFree(*tratte, *ordinamenti);

    *tratte = (Tratta*)malloc(sizeof(Tratta)*n_tratte);
    *ordinamenti = (Tratta***)malloc(sizeof(Tratta**)*N_ORDINAMENTI);
    for(int i=0; i<N_ORDINAMENTI; i++)
        *(*ordinamenti+i) = (Tratta**)malloc(sizeof(Tratta*)*n_tratte);
    
}

/**
 * La funzione esegue la free sia dell'array di struct tratte, sia della matrice di puntatori di struct ordinamenti 
 */ 
void structFree(Tratta tratte[], Tratta*** ordinamenti) {
    free(tratte);
    for(int i=0; i<N_ORDINAMENTI; i++)
        free(ordinamenti[i]);
    free(ordinamenti);
}

/**
 * La funzione chiede in input un file di log, lo legge e salva i dati all'interno delle relative struct
 */
int leggiLog(Tratta** tratte, Tratta**** ordinamenti, int* n_tratte, int* tratte_ordinate_partenza) {
    char nomeFile[MAX_FILE+1];
    printf("Inserire nome del file di log: ");
    scanf("%s", nomeFile);

    FILE* fp = fopen(nomeFile, "r");
    if(!fp)
        return 0;

    fscanf(fp, "%d", n_tratte);

    structAlloc(tratte, ordinamenti, *n_tratte);

    riempiTratte(fp, *n_tratte, *tratte, *ordinamenti);

    /* Se le tratte appena prese in input sono già ordinate per stazione di partenza, si può già da subito effettuare la ricerca dicotomica. */
    *tratte_ordinate_partenza = tratteOrdinatePartenza(*tratte, *n_tratte);

    return 1;
}