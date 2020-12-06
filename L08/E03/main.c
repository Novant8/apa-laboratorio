#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INV 8
#define MAX_STR 50
#define MAX_CMD 24
#define FILE_INV "inventario.txt"
#define FILE_CH "pg.txt"

typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} stat_t;

typedef struct {
    char name[MAX_STR+1];
    char type[MAX_STR+1];
    stat_t stat;
} inv_t;

typedef struct {
    int inUse;
    inv_t* arrEq[MAX_INV];
} tabEquip_t;

typedef struct {
    int cod;
    char name[MAX_STR+1];
    char class[MAX_STR+1];
    tabEquip_t* equip;
    stat_t stat;
} ch_t;

typedef struct node_ch {
    ch_t ch;
    struct node_ch* next;
} nodeCh_t;

typedef struct {
    nodeCh_t* headCh;
    nodeCh_t* tailCh;
    int nCh;
} tabCh_t;

typedef struct {
    inv_t* arrInv;
    int nInv;
} tabInv_t;

typedef enum {
    c_add_ch,
    c_get_ch_stats,
    c_del_ch,
    c_add_equipment,
    c_rmv_equipment,
    c_end,
    c_unknown
} cmd_e;

cmd_e readcmd();
void handlecmd(cmd_e cmd, tabCh_t* tabch, tabInv_t* tabinv);
int addCharacters(tabCh_t* tabch);
int fillInventory(tabInv_t* tabinv);
void addCh_list(ch_t ch, tabCh_t* tabch);
int delCh_list(int cod, tabCh_t* tabch);
int equipItem(inv_t* item, ch_t* ch);
int removeItem(inv_t* item, ch_t* ch);
nodeCh_t* new_nodeCh(ch_t ch, nodeCh_t* next);
inv_t* searchItem(char item_name[], tabInv_t* tabinv);
ch_t* searchCharacter(int cod, nodeCh_t* head);
stat_t getStats(ch_t* ch);
stat_t statsSum(stat_t s1, stat_t s2);
void print_list(nodeCh_t* head);
ch_t readCh(FILE* fp);
inv_t readItem(FILE* fp);
void printCh(ch_t ch);
void printEquipment(ch_t ch);
void printStats(stat_t s);
void free_ch(nodeCh_t* head);

int main() {

    tabCh_t* tabch = (tabCh_t*)malloc(sizeof(tabCh_t));
    tabch->headCh = tabch->tailCh = NULL;
    tabInv_t* tabinv = (tabInv_t*)malloc(sizeof(tabInv_t));
    if(!addCharacters(tabch)) {
        printf("Errore durante la lettura del file dei personaggi.\n");
        return -1;
    }
    if(!fillInventory(tabinv)) {
        printf("Errore durante la lettura del file dell'inventario.\n");
        return -1;
    }

    cmd_e cmd;
    do {
        cmd = readcmd();
        handlecmd(cmd, tabch, tabinv);
    } while(cmd != c_end);

    //FREE
    free_ch(tabch->headCh);
    free(tabch);
    free(tabinv->arrInv);
    free(tabinv);

    return 0;
}

cmd_e readcmd() {
    char* cmds[c_unknown] = {"aggiungi_personaggio", "stampa_statistiche", "elimina_personaggio", "aggiungi_equipaggiamento", "rimuovi_equipaggiamento", "fine"};
    char cmd_str[MAX_CMD+1];
    printf("Inserire comando: ");
    scanf("%s", cmd_str);

    cmd_e cmd;
    for(cmd=c_add_ch; cmd < c_unknown && strcmp(cmds[cmd], cmd_str); cmd++);

    return cmd;
}

void handlecmd(cmd_e cmd, tabCh_t* tabch, tabInv_t* tabinv) {
    ch_t *ch;
    ch_t ch_i;
    int cod;
    switch(cmd) {
        case c_add_ch:
            printf("Inserire i dati del personaggio: ");
            ch_i = readCh(stdin);
            if(ch_i.cod > 0)
                addCh_list(ch_i, tabch);
            else
                printf("Dati inseriti invalidi.\n");
            break;
        case c_get_ch_stats:
            printf("Inserire codice personaggio: ");
            scanf("%d", &cod);
            ch = searchCharacter(cod, tabch->headCh);
            if(ch != NULL) {
                stat_t stats = getStats(ch);
                printf("PERSONAGGIO:\n");
                printCh(*ch);
                printf("EQUIPAGGIAMENTO:\n");
                printEquipment(*ch);
                printf("STATISTICHE TOTALI: ");
                printStats(stats);
            } else {
                printf("Personaggio non trovato.\n");
            }
            break;
        case c_del_ch:
            printf("Inserire codice personaggio: ");
            scanf("%d", &cod);
            if(delCh_list(cod, tabch))
                printf("Personaggio cancellato!\n");
            else
                printf("Nessun personaggio trovato.\n");
            break;
        case c_add_equipment:
        case c_rmv_equipment:
            printf("Inserire codice personaggio: ");
            scanf("%d", &cod);
            printf("Inserire nome oggetto: ");
            char item_name[MAX_STR+1];
            scanf("%s", item_name);

            inv_t* item = searchItem(item_name, tabinv);
            ch_t* ch = searchCharacter(cod, tabch->headCh);
            if(item != NULL)
                if(cmd == c_add_equipment)
                    equipItem(item, ch);
                else
                    removeItem(item, ch);
            else
                printf("Dati inseriti non validi.\n");
            break;
        case c_end:
            break;
        case c_unknown:
            printf("Comando sconosciuto.\n");
    }
}

/**
 * La funzione legge i dati dei personaggi da file e li aggiunge alla lista.
 * Return: 
 * - 1 se la lettura va a buon fine
 * - 0 se c'è stato un errore nella lettura
 */ 
int addCharacters(tabCh_t* tabch) {
    FILE* fp = fopen(FILE_CH, "r");
    if(!fp)
        return 0;

    ch_t ch_tmp;
    while((ch_tmp = readCh(fp)).cod!=-1) { //Se è stato letto un personaggio valido
        addCh_list(ch_tmp, tabch);
        tabch->nCh++;
    }

    fclose(fp);

    return 1;
}

/**
 * La funzione legge i dati sugli oggetti da file e li aggiunge alla lista.
 * Return: 
 * - 1 se la lettura va a buon fine
 * - 0 se c'è stato un errore nella lettura
 */ 
int fillInventory(tabInv_t* tabinv) {
    FILE* fp = fopen(FILE_INV, "r");
    if(!fp)
        return 0;

    /* Lettura quantità oggetti */
    fscanf(fp, "%d", &tabinv->nInv);

    /* Allocazione array inventario */
    tabinv->arrInv = (inv_t*)malloc(sizeof(inv_t)*tabinv->nInv);

    /* Ciclo di lettura da file */
    for(int i=0; i<tabinv->nInv; i++)
        tabinv->arrInv[i] = readItem(fp);
    
    fclose(fp);
    
    return 1;
}

/**
 * La funzione aggiunge un personaggio alla lista IN CODA
 */ 
void addCh_list(ch_t ch, tabCh_t* tabch) {
    if(tabch->headCh == NULL) {
        tabch->headCh = tabch->tailCh = new_nodeCh(ch, NULL);
    } else {
        nodeCh_t* new = new_nodeCh(ch, NULL);
        tabch->tailCh->next = new;
        tabch->tailCh = new;
    }
}

/**
 * La funzione rimuove un personaggio identificato dal codice cod dalla lista
 * Return:
 * - 1 se il personaggio è stato cancellato
 * - 0 se non è stato trovato un personaggio avente codice cod
 */ 
int delCh_list(int cod, tabCh_t* tabch)  {
    nodeCh_t *n, *tmp;
    for(n=tabch->headCh; n->next != NULL; n=n->next)
        if(n->next->ch.cod == cod) {
            tmp = n->next;
            n->next = n->next->next;
            free(tmp);
            return 1;
        }
    return 0;
}

/**
 * La funzione aggiunge l'oggetto puntato da item nell'inventario del personaggio puntato da ch
 */ 
int equipItem(inv_t* item, ch_t* ch) {
    int n_items = ch->equip->inUse;
    if(n_items >= MAX_INV)
        return 0;
    ch->equip->arrEq[n_items] = item;
    ch->equip->inUse++;
    return 1;
}

/**
 * La funzione rimuove l'oggetto puntato da item nell'inventario del personaggio puntato da ch
 */ 
int removeItem(inv_t* item, ch_t* ch) {
    int n_items = ch->equip->inUse;
    if(n_items >= MAX_INV)
        return 0;
    
    int i,j;
    /* Ricerca dell'indice dell'array contenente il puntatore item */
    for(int i=0; i<n_items; i++)
        if(ch->equip->arrEq+i == item) {
            /* Shift a sinistra di uno dell'array equipaggiamento, solo per gli indici maggiori di quello da rimuovere */
            for(j=i; j<n_items-1; j++)
                ch->equip->arrEq[i] = ch->equip->arrEq[i+1];
            break;
        }
    ch->equip->inUse--;
    return 1;
}

nodeCh_t* new_nodeCh(ch_t ch, nodeCh_t* next) {
    nodeCh_t* new = (nodeCh_t*)malloc(sizeof(nodeCh_t));
    new->ch = ch;
    new->next = next;
    
    return new;
}

inv_t* searchItem(char item_name[], tabInv_t* tabinv) {
    for(int i=0; i<tabinv->nInv; i++)
        if(strcmp(item_name, tabinv->arrInv[i].name)==0)
            return tabinv->arrInv+i;
    return NULL;
}

ch_t* searchCharacter(int cod, nodeCh_t* head) {
    if(head == NULL)
        return NULL;
    if(head->ch.cod == cod)
        return &head->ch;
    return searchCharacter(cod, head->next);
}

stat_t getStats(ch_t* ch) {
    stat_t s = ch->stat;

    /* Primo ciclo che somma tutte le statistiche */
    for(int i=0; i<ch->equip->inUse; i++)
        s = statsSum(s, ch->equip->arrEq[i]->stat);
    
    /* Controllo che tutte le statistiche siano positive o nulle. In caso contrario, vengono impostate a 0 */
    if(s.hp<0) s.hp=0;
    if(s.mp<0) s.mp=0;
    if(s.atk<0) s.atk=0;
    if(s.def<0) s.def=0;
    if(s.mag<0) s.mag=0;
    if(s.spr<0) s.spr=0;
    
    return s;
}

stat_t statsSum(stat_t s1, stat_t s2) {
    stat_t sum;
    sum.hp = s1.hp + s2.hp;
    sum.mp = s1.mp + s2.mp;
    sum.atk = s1.atk + s2.atk;
    sum.def = s1.def + s2.def;
    sum.mag = s1.mag + s2.mag;
    sum.spr = s1.spr + s2.spr;
    return sum;
}

void print_list(nodeCh_t* head) {
    nodeCh_t* n;
    for(n=head; n!=NULL; n=n->next)
        printCh(n->ch);
}

/**
 * La funzione legge i dati relativi a un personaggio da file:
 * Return:
 * - Il personaggio letto
 * - Un personaggio avente codice -1 se la lettura non va a buon fine
 */ 
ch_t readCh(FILE* fp) {
    ch_t ch;
    int res = fscanf(fp, "PG%d %s %s %d %d %d %d %d %d\n",
                        &ch.cod,
                        ch.name,
                        ch.class,
                        &ch.stat.hp,
                        &ch.stat.mp,
                        &ch.stat.atk,
                        &ch.stat.def,
                        &ch.stat.mag,
                        &ch.stat.spr);
    if(res!=9) {
        ch.cod=-1;
    } else {
        ch.equip = (tabEquip_t*)malloc(sizeof(tabEquip_t));
        ch.equip->inUse = 0;
    }
    
    return ch;
}

/**
 * La funzione legge i dati relativi a un oggetto da file:
 * Return:
 * - L'oggetto letto
 * - Un oggetto avente nome vuoto se la lettura non va a buon fine
 */ 
inv_t readItem(FILE* fp) {
    inv_t item;
    int res = fscanf(fp, "%s %s %d %d %d %d %d %d",
                        item.name,
                        item.type,
                        &item.stat.hp,
                        &item.stat.mp,
                        &item.stat.atk,
                        &item.stat.def,
                        &item.stat.mag,
                        &item.stat.spr);
    if(res!=8)
        item.name[0] = '\0';
    
    return item;
}

void printCh(ch_t ch) {
    printf("PG%04d %s %s %d %d %d %d %d %d\n",
                ch.cod,
                ch.name,
                ch.class,
                ch.stat.hp,
                ch.stat.mp,
                ch.stat.atk,
                ch.stat.def,
                ch.stat.mag,
                ch.stat.spr);
}

void printEquipment(ch_t ch) {
    for(int i=0; i<ch.equip->inUse; i++) {
        printf("%d. %s (tipo: %s) \t Statistiche: ", i+1, ch.equip->arrEq[i]->name, ch.equip->arrEq[i]->type);
        printStats(ch.equip->arrEq[i]->stat);
    }
}

void printStats(stat_t s) {
    printf("{hp: %d, mp: %d, atk: %d, def: %d, mag: %d, spr: %d}\n",
                s.hp,
                s.mp,
                s.atk,
                s.def,
                s.mag,
                s.spr);
}

void free_ch(nodeCh_t* head) {
    if(head == NULL)
        return;
    free_ch(head->next);
    free(head->ch.equip);
    free(head);
}