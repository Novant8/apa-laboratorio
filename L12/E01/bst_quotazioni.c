#include <stdlib.h>
#include <limits.h>

#include "bst_quotazioni.h"

typedef struct bst_node_s {
    quot_t q;
    struct bst_node_s* l;
    struct bst_node_s* r;
    int count;
} *bst_node_t;

struct bst_quot_s {
    bst_node_t root;
    int min_path;
    int max_path;
};

bst_quot_t bst_quot_init() {
    bst_quot_t bst = (bst_quot_t)malloc(sizeof(struct bst_quot_s));
    bst->root=NULL;
    bst->min_path = INT_MAX;
    bst->max_path = 0;
    return bst; 
}

/**
 * La funzione alloca e restituisce il nodo di un albero binario
 */ 
static bst_node_t bst_newNode(trans_t tr, bst_node_t l, bst_node_t r) {
    bst_node_t n = (bst_node_t)malloc(sizeof(struct bst_node_s));
    n->q.data = tr.data;
    n->q.somma = tr.val * tr.num;
    n->q.num = tr.num;
    n->l = l;
    n->r = r;
    n->count = 1;
    return n;
}

/**
 * La funzione somma valore e numero di due quotazioni della STESSA DATA 
 */ 
static quot_t quotazione_unisci(quot_t q, trans_t tr) {
    q.somma += tr.val * tr.num;
    q.num += tr.num;
    return q;
}

/**
 * La funzione inserisce in foglia un nodo contenente quotazione q all'albero.
 * Viene inoltre salvata la lunghezza del cammino all'interno della variabile path passata per parametro.
 * Return: puntatore alla radice dell'albero aggiornato
 */ 
static bst_node_t bst_aggiungi(bst_node_t root, trans_t tr, int *node_added) {
    if(root == NULL) {
        *node_added = 1;
        return bst_newNode(tr, NULL, NULL);
    }
    int res = data_cmp(tr.data, root->q.data);
    if(res == 0) //Se nell'albero esiste già la quotazione relativa alla data della transazione da aggiungere, allora valore e numero di quella esistente vengono sommati
        root->q = quotazione_unisci(root->q, tr);
    else if (res>0)
        root->r = bst_aggiungi(root->r, tr, node_added);
    else
        root->l = bst_aggiungi(root->l, tr, node_added);
    if(*node_added)
        root->count++;
    return root;
}

/**
 * Funzione wrapper che va ad aggiungere in foglia un nuovo nodo avente quotazione q all'albero bst.
 * Inoltre, vengono eventualmente impostati i valori relativi alla lunghezza del cammino massimo e minimo nell'albero.
 */ 
void bst_quot_aggiungi(bst_quot_t bst, trans_t tr) {
    int node_added=0;
    bst->root=bst_aggiungi(bst->root, tr, &node_added);

    //printf("path = %d\n", path);

    /* Aggiorna lunghezze del cammino massimo e minimo */
    /*if(path < bst->min_path)
        bst->min_path = path;
    if(path > bst->max_path)
        bst->max_path = path;*/
}

/**
 * Funzione cerca una quotazione avvenuta nella data passata per parametro
 * Return:
 * - puntatore alla quotazione trovata se questa esiste
 * - puntatore nullo se questa non esiste 
 */ 
static quot_t* bst_cerca(bst_node_t root, data_t d) {
    if(root==NULL)
        return NULL;
    int res = data_cmp(d, root->q.data);
    if(res == 0)
        return &root->q;
    if(res > 0)
        return bst_cerca(root->r, d);
    return bst_cerca(root->l, d);
}

/**
 * Funzione wrapper che va a cercare una quotazione avvenuta nella data passata per parametro
 * Return:
 * - puntatore alla quotazione trovata se questa esiste
 * - puntatore nullo se questa non esiste 
 */ 
quot_t* bst_quot_cerca(bst_quot_t bst, data_t d) {
    return bst_cerca(bst->root, d);
}

void bst_free(bst_node_t root) {
    if(root == NULL)
        return;
    bst_free(root->l);
    bst_free(root->r);
    free(root);
}

void bst_quot_free(bst_quot_t bst) {
    bst_free(bst->root);
}

/**
 * La funzione stampa le quotazioni contenute nell'albero binario in ordine
 */ 
static void bst_print(FILE* fp, bst_node_t root)  {
    if(root == NULL)
        return;
    bst_print(fp, root->l);
    quotazione_print(fp, root->q);
    printf("\n");
    bst_print(fp, root->r);
}

void bst_quot_print(FILE* fp, bst_quot_t bst) {
    bst_print(fp, bst->root);
}

/**
 * La funzione restituisce il puntatore alla data minima presente nell'albero.
 * Se l'albero è vuoto, viene restituito un puntatore a NULL
 */ 
static data_t bst_quot_minData_R(bst_node_t root) {
    if(root == NULL)
        return data_null();
    if(root->l == NULL)
        return root->q.data;
    return bst_quot_minData_R(root->l);
}

/**
 * La funzione restituisce il puntatore alla data massima presente nell'albero.
 * Se l'albero è vuoto, viene restituito un puntatore a NULL
 */ 
static data_t bst_quot_maxData_R(bst_node_t root) {
    if(root == NULL)
        return data_null();
    if(root->r == NULL)
        return root->q.data;
    return bst_quot_minData_R(root->r);
}

/**
 * Funzione wrapper che restituisce il puntatore alla data minima presente nell'albero.
 * Se l'albero è vuoto, viene restituito un puntatore a NULL
 */ 
data_t bst_quot_minData(bst_quot_t bst) {
    return bst_quot_minData_R(bst->root);
}

/**
 * Funzione wrapper che restituisce il puntatore alla data massima presente nell'albero.
 * Se l'albero è vuoto, viene restituito un puntatore a NULL
 */ 
data_t bst_quot_maxData(bst_quot_t bst) {
    return bst_quot_maxData_R(bst->root);
}

/**
 * Funzione che visita ricorsivamente l'albero in ordine e ritona la quotazione minima compresa tra le due date d1 e d2.
 */
static quot_t* bst_min_quot_R(bst_node_t root, data_t d1, data_t d2, quot_t* min) {
    if(root == NULL) //Condizione di terminazione
        return min;

    /* Ricava la quotazione minima del sottoalbero di sinistra */
    min = bst_min_quot_R(root->l, d1, d2, min);
    
    /* Controllo se la quotazione nella radice rientra nell'intervallo di date */
    if(data_cmp(root->q.data, d1) >= 0 && data_cmp(root->q.data, d2) <= 0)
        /*
        Controllo se la quotazione della radice corrente è minore di quella minima.
        Se si, aggiorna la variabile di ritorno.
        */
        if(root->q.somma/root->q.num < min->somma/min->num)
            min = &root->q;

    /* Ricava la quotazione minima del sottoalbero di destra  */
    min = bst_min_quot_R(root->r, d1, d2, min);
    
    return min;
}

/**
 * Funzione wrapper che restituisce la quotazione minima tra due date.
 * Return:
 * - puntatore alla quotazione minima se questa è stata trovata
 * - NULL se non è stata trovata una quotazione nell'intervallo di date
 */
quot_t* bst_min_quot(bst_quot_t bst, data_t d1, data_t d2) {
    /* Crea una quotazione avente valore teorico massimo e data nulla */
    quot_t quot_min_th;
    quot_min_th.data = data_null();
    quot_min_th.somma = INT_MAX;
    quot_min_th.num = 1;

    quot_t* quot_min = bst_min_quot_R(bst->root, d1, d2, &quot_min_th);
    
    /* 
    Controllo se è stata trovata una quotazione minima:
    se la data della quotazione trovata è nulla, allora non è stata trovata una quotazione nell'intervallo di date (la quotazione è quella teorica creata all'inizio).
    */
    if(data_cmp(quot_min->data, quot_min_th.data) == 0)
        return NULL;

    return quot_min;
}

static void bst_maxMinPaths(bst_node_t root, int path, int *max, int *min) {
    if(root == NULL)
        return;

    //Condizione di terminazione: il nodo corrente è una foglia
    if(root->l == NULL && root->r == NULL) {
        //Controllo se il cammino concluso è massimo
        if(path > *max)
            *max = path;
    } else {
        if(path++ < *min)
            *min = path;
        //Visita sottoalbero destro e sinistro
        bst_maxMinPaths(root->l, path, max, min);
        bst_maxMinPaths(root->r, path, max, min);
    }
}

/**
 * La funzione restituisce il rapporto tra la lunghezza del cammino massimo e quella del cammino minimo.
 */ 
float bst_quot_rapportoMaxMin(bst_quot_t bst) {
    int max=0, min=INT_MAX;
    bst_maxMinPaths(bst->root, 0, &max, &min);
    return (float)max/min;
}

static bst_node_t bst_rotL(bst_node_t root) {
    /* Rotazione nodi */
    bst_node_t x = root->r;
    root->r = x->l;
    x->l = root;
    
    /* Aggiornamento dimensioni */
    x->count = root->count;
    root->count = 1;
    root->count += root->l ? root->l->count : 0;
    root->count += root->r ? root->r->count : 0;

    return x;
}

static bst_node_t bst_rotR(bst_node_t root) {
    /* Rotazione nodi */
    bst_node_t x = root->l;
    root->l = x->r;
    x->r = root;

    /* Aggiornamento dimensioni */
    x->count = root->count;
    root->count = 1;
    root->count += root->l ? root->l->count : 0;
    root->count += root->r ? root->r->count : 0;
    
    return x;
}

static bst_node_t bst_part(bst_node_t root, int k) {
    int t = root-> l ? root->l->count : 0;
    if(t > k) {
        root->l = bst_part(root->l, k);
        root = bst_rotR(root);
    } else if(t < k) {
        root->r = bst_part(root->r, k-t-1);
        root = bst_rotL(root);
    }
    return root;
}

static bst_node_t bst_balance(bst_node_t root) {
    int r;
    if(root == NULL)
        return NULL;
    r = (root->count+1)/2-1;
    root = bst_part(root, r);
    root->l = bst_balance(root->l);
    root->r = bst_balance(root->r);
    return root;
}

void bst_quot_bilancia(bst_quot_t bst) {
    bst->root = bst_balance(bst->root);
}