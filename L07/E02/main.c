#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAMPO 50
#define MAX_FILE 20
#define MAX_COMANDO 16 //"aggiungi_tastiera"

typedef struct {
    int d;
    int m;
    int y;
} Date;

typedef struct {
    int cod;
    char nome[MAX_CAMPO+1];
    char cognome[MAX_CAMPO+1];
    Date data_nascita;
    char via[MAX_CAMPO+1];
    char citta[MAX_CAMPO+1];
    int cap;
} Item;

typedef struct node {
    Item i;
    struct node *next; 
} List;

typedef enum {
    c_add_file,
    c_add_keyboard,
    c_search,
    c_extract_cod,
    c_extract_date,
    c_print,
    c_end,
    c_unknown
} cmd_e;

void handlecmd(cmd_e cmd, List** head);
cmd_e getcmd();
int add_from_file(List** head);
int add_from_keyboard(List** head);
List* add_to_list(List* head, Item i);
List* search_item(List* head, int cod);
Item list_extract_cod(List** head, int cod);
Item list_extract_date(List** head, Date date_a, Date date_b);
Item get_item(FILE* fp);
Item item_void();
int item_is_void(Item i);
List* new_node(Item i, List* next);
void print_list(List* head);
void print_item(FILE* fp, Item i);
void free_list(List* head);
FILE* getFile(char* mod);
int date_toint(Date d);

int main() {
    
    cmd_e cmd;
    List* head = NULL;

    if(!add_from_file(&head)) {
        printf("Errore in lettura file\n");
        return -1;
    }

    do {
        cmd = getcmd();
        handlecmd(cmd, &head);
    } while (cmd!=c_end);

    free_list(head);

    return 0;
}

cmd_e getcmd() {
    char cmd_str[MAX_COMANDO+1];
    char* tbl[c_unknown] = {"carica_file", "aggiungi_tastiera", "cerca", "estrai_cod", "estrai_date", "stampa", "fine"};
    cmd_e cmd = c_add_file;

    printf("Inserire un comando: ");
    scanf("%s", cmd_str);
    
    while(cmd < c_unknown && strcmp(cmd_str, tbl[cmd])!=0)
        cmd++;

    return cmd;
}

/**
 * Funzione che, dato un certo comando acquisito dalla funzione getcmd(), gestisce le istruzioni da eseguire.
 */ 
void handlecmd(cmd_e cmd, List** head) {
    switch(cmd) {
        case c_add_file:
            if(!add_from_file(head))
                printf("Errore in lettura file.\n");
            break;
        case c_add_keyboard:
            printf("Inserire i dati: ");
            if(!add_from_keyboard(head))
                printf("Dati inseriti non validi.\n");
            break;
        case c_search:
            printf("Inserire codice: ");
            int cod_search;
            scanf("%d", &cod_search);
            List* res = search_item(*head, cod_search);
            if(res!=NULL) {
                printf("Trovato elemento: ");
                print_item(stdout, res->i);
            } else {
                printf("Nessun elemento trovato.\n");
            }
            break;
        case c_extract_cod:
            printf("Inserire codice (numero intero): ");
            int cod_extr;
            scanf("%d", &cod_extr);
            Item extr_c = list_extract_cod(head, cod_extr);
            if(!item_is_void(extr_c)) {
                printf("Estratto elemento: ");
                print_item(stdout, extr_c);
            } else {
                printf("Nessun elemento estratto.\n");
            }
            break;
        case c_extract_date:
            printf("Inserire due date (gg/mm/aaaa): ");
            Date date_a, date_b;
            if(scanf("%d/%d/%d %d/%d/%d", &date_a.d, &date_a.m, &date_a.y, &date_b.d, &date_b.m, &date_b.y)!=6) {
                printf("Input invalido.\n");
                return;
            }
            if(date_toint(date_a) > date_toint(date_b)) {
                Date tmp = date_a; 
                date_a = date_b;
                date_b = tmp;
            }
            printf("Elementi estratti:\n");
            Item extr_d;
            while(!item_is_void((extr_d = list_extract_date(head, date_a, date_b))))
                print_item(stdout, extr_d);
            break;
        case c_print:
            print_list(*head);
            break;
        case c_end:
            break;
        default:
            printf("Comando sconosciuto.\n");
    }
}

/**
 *  La funzione prende in input il nome di un file, dal quale vengono estratte le informazioni e salvate nella lista puntata da *head.
 */ 
int add_from_file(List** head) {
    FILE* fp = getFile("r");

    if(fp==NULL)
        return 0;

    Item i;

    while(1) {
        i = get_item(fp);
        if(i.cod < 0)
            break;
        *head = add_to_list(*head, i);
    }

    fclose(fp);

    return 1;
}

/**
 * La funzione prende in input da tastiera i dati di una persona, e li aggiunge alla lista.
 */ 
int add_from_keyboard(List** head) {
    getchar(); /* Istruzione extra per catturare il linebreak da stdin */
    Item i = get_item(stdin);

    if(i.cod<0)
        return 0;

    *head = add_to_list(*head, i);

    return 1;
}

/**
 * La funzione aggiunge un elemento di tipo Item alla lista
 */ 
List* add_to_list(List* head, Item i) {
    if(head == NULL || date_toint(head->i.data_nascita) < date_toint(i.data_nascita))
        return new_node(i, head);

    List* n;
    for(n=head; n->next != NULL && date_toint(n->next->i.data_nascita) > date_toint(i.data_nascita); n = n->next);
    n->next = new_node(i, n->next);
    
    return head;
}

/**
 * La funzione cerca nella lista l'elemento avente codice cod passato come parametro.
 * Return:
 * - Il puntatore al dato trovato
 * - NULL se il codice non corrisponde a nessun dato
 */ 
List* search_item(List* head, int cod) {
    if(head == NULL || head->i.cod == cod)
        return head;
    return search_item(head->next, cod);
}

/**
 * La funzione estrae dalla lista l'elemento avente codice cod passato come parametro.
 * Return:
 * - Il puntatore al dato estratto
 * - NULL se il codice non corrisponde a nessun dato
 */ 
Item list_extract_cod(List** head, int cod) {
    if(head == NULL)
        return item_void();
    
    List *n, *p;
    Item extr;
    for(n = *head, p = NULL; n != NULL; p = n, n=n->next)
        if(n->i.cod == cod) {
            if(p==NULL)
                *head = n->next;
            else
                p->next = n->next;
            extr = n->i;
            free(n);
            return extr;
        }

    return item_void();
}


/**
 * La funzione estrae dalla lista il primo elemento compreso tra due date (date_a e date_b).
 * Si assume che date_b sia sempre maggiore di date_a.
 * Return:
 * - Il puntatore al dato estratto
 * - NULL se non è stato estratto nulla
 */ 
Item list_extract_date(List** head, Date date_a, Date date_b) {
    if(head == NULL)
        return item_void();

    Item extr;
    List *n, *p;
    for(n = *head, p = NULL; n != NULL && date_toint(n->i.data_nascita) >= date_toint(date_a); p = n, n=n->next)
        if(date_toint(n->i.data_nascita) <= date_toint(date_b)) {
            if(p==NULL)
                *head = n->next;
            else
                p->next = n->next;
            extr = n->i;
            free(n);
            return extr;
        }

    return item_void();
}

/**
 * La funzione legge da file i dati anagrafici relativi a una persona, e restituisce il relativo oggetto di tipo Item
 */
Item get_item(FILE* fp) {
    Item i;
    int res = fscanf(fp, "A%d %s %s %d/%d/%d %s %s %d\n",
                        &i.cod,
                        i.nome,
                        i.cognome,
                        &i.data_nascita.d,
                        &i.data_nascita.m,
                        &i.data_nascita.y,
                        i.via,
                        i.citta,
                        &i.cap);

    if(res != 9)
        i.cod = -1;

    return i;
}

Item item_void() {
    Item i;
    i.cod = -1;
    return i;
}

int item_is_void(Item i) {
    return i.cod==-1;
}
 
List* new_node(Item i, List* next) {
    List* n = (List*)malloc(sizeof(List));
    if(n==NULL) {
        return NULL;
    } else {
        n->i = i;
        n->next = next;
    }
    return n;
}

/**
 * La funzione chiede in input il nome di un file di output, in cui verranno salvati tutti i dati contenuti nella lista
 */ 
void print_list(List* head) {
    if(head == NULL) {
        printf("Lista vuota\n");
        return;
    }

    FILE* fp = getFile("w");
    if(!fp) {
        printf("Errore in scrittura file.\n");
        return;
    }
    
    for(List* n=head; n!=NULL; n=n->next)
        print_item(fp, n->i);
    
    printf("Fatto!\n");

    fclose(fp);
}

/**
 * La funzione stampa su file i dati relativi a un singolo Item
 */ 
void print_item(FILE* fp, Item i) {
    fprintf(fp, "A%04d %s %s %d/%02d/%d %s %s %05d\n",
            i.cod,
            i.nome,
            i.cognome,
            i.data_nascita.d,
            i.data_nascita.m,
            i.data_nascita.y,
            i.via,
            i.citta,
            i.cap);
}

void free_list(List* head) {
    if(head == NULL)
        return;
    free_list(head->next);
    free(head);
}

FILE* getFile(char* mod) {
    char fileName[MAX_FILE+1];

    printf("Inserire nome file: ");
    scanf("%s", fileName);

    return fopen(fileName, mod);
}

int date_toint(Date d) {
    return d.y * 10000 + d.m * 100 + d.d;
}