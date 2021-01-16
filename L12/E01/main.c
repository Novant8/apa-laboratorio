#include <stdio.h>
#include <string.h>

#define N_FILE 3
#define MAX_FILE_STR 7
#define MAX_CMD_LEN 22

#include "bst_quotazioni.h"
#include "dataora.h"
#include "lista_titoli.h"
#include "quotazione.h"
#include "titolo_azionario.h"

typedef enum {
    c_cerca_titolo,
    c_cerca_data,
    c_min_quot_date,
    c_min_quot,
    c_bilancia,
    c_fine,
    c_sconosciuto
} cmd_e;

cmd_e cmd_input();
void cmd_gestisci(cmd_e cmd, lista_titoli_t l);

int riempi_titoli(lista_titoli_t l);

int main() {

    /* INIZIALIZZAZIONE E LETTURA DI LISTA DA FILE */
    lista_titoli_t l = lista_titoli_init(); 
    if(!riempi_titoli(l)) {
        lista_titoli_free(l);
        printf("Errore in lettura file.\n");
        return -1;
    }

    /* CICLO DI INPUT COMANDI */
    cmd_e cmd;
    do {
        cmd = cmd_input();
        cmd_gestisci(cmd, l);
    } while(cmd != c_fine);

    lista_titoli_free(l);

    return 0;
}

cmd_e cmd_input() {
    char cmd_in[MAX_CMD_LEN+1];
    char* cmd_str[MAX_CMD_LEN+1] = {"cerca_titolo", "cerca_data", "quotazione_minima_date", "quotazione_minima", "bilancia", "fine"};
    printf("Inserire comando: ");
    scanf("%s", cmd_in);

    cmd_e cmd;
    for(cmd = c_cerca_titolo; cmd<c_sconosciuto && strcmp(cmd_str[cmd], cmd_in) != 0; cmd++);

    return cmd;
}

void cmd_gestisci(cmd_e cmd, lista_titoli_t l) {
    titolo_t t;
    char str[MAX_TITOLO_LEN+1];
    data_t d;
    data_t d1;
    data_t tmp;
    quot_t* qp;
    float s,r;

    /* Chiedi titolo in input se il comando è valido */
    if(cmd < c_fine) {
        printf("Inserire codice di un titolo azionario: ");
        scanf("%s", str);
        t=lista_titoli_cerca(l, str);
        if(t == NULL) {
            printf("Nessun titolo trovato.\n");
            return;
        }
    }

    switch(cmd) {
        case c_cerca_titolo:
            printf("Titolo trovato: ");
            titolo_print(stdout, t);
            printf("\n");
            break;
        case c_cerca_data:
            printf("Inserire una data (aaaa/mm/gg): ");
            d = data_read(stdin);
            qp = titolo_getQuot(t, d);
            if(qp!=NULL) {
                printf("Quotazione trovata: ");
                quotazione_print(stdout, *qp);
                printf("\n");
            } else {
                printf("Quotazione non trovata.\n");
            }
            break;
        case c_min_quot_date:
            printf("Inserire una data (aaaa/mm/gg): ");
            d = data_read(stdin);
            printf("Inserire una data (aaaa/mm/gg): ");
            d1 = data_read(stdin);
            /* Controllo che le date prese in input siano valide */
            if(data_isnull(d) || data_isnull(d1)) {
                printf("Una o più date inserite invalide.\n");
                return;
            }
            /* Scambia date in modo che d contenga la minima e d1 la massima */
            if(data_cmp(d, d1) > 0) {
                tmp = d;
                d = d1;
                d1 = tmp;
            }
            qp = titolo_minQuot_date(t, d, d1);
            if(qp != NULL) {
                printf("Quotazione minima: ");
                quotazione_print(stdout, *qp);
                printf("\n");
            } else {
                printf("Nessuna quotazione trovata tra le due date.\n");
            }
            break;
        case c_min_quot:
            qp = titolo_minQuot(t);
            if(qp != NULL) {
                printf("Quotazione minima: ");
                quotazione_print(stdout, *qp); 
                printf("\n");
            } else {
                printf("Nessuna quotazione trovata tra le due date.\n");
            }
            break;
        case c_bilancia:
            printf("Inserire un valore di soglia (max/min): ");
            scanf("%f", &s);
            r = titolo_rapportoMaxMin(t);
            if(r > s) {
                printf("Rapporto prima del bilanciamento: %f\n", r);
                titolo_bilanciaQuotazioni(t);
                printf("Rapporto dopo il bilanciamento: %f\n", titolo_rapportoMaxMin(t));
            } else {
                printf("Il rapporto %f non supera la soglia inserita.\n", r);
            }            
            break;
        case c_fine:
            break;
        default:
            printf("Comando sconosciuto.\n");
    }
}

/**
 * La funzione legge dai tre file di esempio i dati sui titoli (e quotazioni) e ne riempie la lista relativa
 */
int riempi_titoli(lista_titoli_t l) {
    FILE* fp;
    int n_titoli;
    //int new; //Booleana che risulta vera se il titolo letto non è presente nella lista e deve essere creato
    char cod[MAX_TITOLO_LEN+1];
    int n_q;
    trans_t tr;
    titolo_t t;

    char file_str[MAX_FILE_STR+1];
    for(int i=0; i<N_FILE; i++) {
        sprintf(file_str, "F%d.txt", i+1);
        
        fp = fopen(file_str, "r");
        if(fp == NULL)
            return 0;

        fscanf(fp, "%d", &n_titoli);
        for(int j=0; j<n_titoli; j++) {
            //new = 0;

            if(fscanf(fp, "%s %d", cod, &n_q) != 2)
                return 0;

            /* 
            Cerca nella lista un titolo avente codice pari a quello appena letto da file. 
            Se il titolo non è presente nella lista, allora ne viene inizializzato uno nuovo e viene aggiunto alla lista, altrimenti vengono aggiunte le transazioni a quello esistente.
            */
            t = lista_titoli_cerca(l, cod);
            if(t==NULL) {
                t = titolo_init(cod);
                lista_titoli_aggiungi(l, t);
            }

            /* Ciclo di aggiunta delle transazioni relative al titolo */
            for(int i=0; i<n_q; i++) {
                tr=transazione_leggi(fp); //(le transazioni vengono immagazzinate in una struttura quotazione)
                if(transazione_cmp(tr, transazione_null()))
                    return 0;
                titolo_aggiungi_transazione(t, tr);
            }
        }
    }
}