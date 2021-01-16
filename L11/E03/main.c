#include <stdio.h>
#include <string.h>

#include "key.h"
#include "st.h"
#include "graph.h"

#define GRAPH_FILE "grafo.txt"
#define MAX_CMD 17

typedef enum {
    c_print_v,  //Stampa vertici
    c_adj_m,    //Adiacenza (matrice)
    c_adj_l,    //Adiacenza (lista)
    c_exit,
    c_unknown
} cmd_e;

int st_fill(st_t st, FILE* fp);
void graph_fill(graph_t g, st_t st, FILE* fp);
cmd_e getcmd();
void handlecmd(cmd_e cmd, st_t st, graph_t g);
int adj_vert(cmd_e cmd, graph_t g, st_t st, keys_t k[]);

int main() {

    FILE* fp = fopen(GRAPH_FILE, "r");
    if(fp==NULL) {
        printf("Errore in apertura del file.\n");
        return -1;
    }

    //Inizializza tabella di simboli leggendo i dati da file
    st_t st = st_init();
    int n_v = st_fill(st, fp);

    //Torna all'inizio del file e inizializza il grafo leggendo i dati dal file
    rewind(fp);
    graph_t g = graph_init(n_v);
    graph_fill(g, st, fp);

    fclose(fp);

    //Genera lista delle adiacenze
    graph_genLadj(g);
    
    //Ciclo di comandi
    cmd_e cmd;
    do{
        cmd = getcmd();
        handlecmd(cmd, st, g);
    } while(cmd != c_exit);

    st_free(st);
    graph_free(g);
    
    return 0;
}

cmd_e getcmd() {
    char* cmds[MAX_CMD+1] = {"stampa_vertici", "adiacenza_matrice", "adiacenza_lista", "fine"};
    char cmd_str[MAX_CMD+1];
    printf("Inserire comando: ");
    scanf("%s", cmd_str);

    cmd_e cmd;
    for(cmd=c_print_v; cmd<c_unknown && strcmp(cmds[cmd], cmd_str) != 0; cmd++);
    return cmd;
}

void handlecmd(cmd_e cmd, st_t st, graph_t g) {
    keys_t k[3];
    switch(cmd) {
        case c_print_v:
            graph_display(g, st);
            break;
        case c_adj_m:
        case c_adj_l:
            //Input vertici
            for(int i=0; i<3; i++) {
                printf("VERTICE %d:\n", i+1);
                k[i] = key_read();
            }
            if(adj_vert(cmd, g, st, k))
                printf("I vertici sono adiacenti a coppie\n");
            else
                printf("I vertici non sono adiacenti a coppie\n");
            break;
        case c_exit:
            break;
        default:
            printf("Comando sconosciuto.\n");
    }
}

/**
 * La funzione verifica se tre nodi identificati dalle rispettive chiavi sono adiacenti a coppie
 * Il metodo utilizzato (matrice o lista) dipende dal comando digitato dall'utente passato per parametro.
 * Return:
 * - 1 se i tre vertici sono adiacenti a coppie
 * - 0 se questi non sono adiacenti a coppie
 */ 
int adj_vert(cmd_e cmd, graph_t g, st_t st, keys_t k[]) {
    int idx[3];
    for(int i=0; i<3; i++)
        idx[i] = st_searchByKey(st, k[i]);
    if(cmd==c_adj_m)
        return  graph_isadj_mat(g, st, idx[0], idx[1]) &&
                graph_isadj_mat(g, st, idx[1], idx[2]) &&
                graph_isadj_mat(g, st, idx[2], idx[0]);
    else
        return  graph_isadj_list(g, st, idx[0], idx[1]) &&
                graph_isadj_list(g, st, idx[1], idx[2]) &&
                graph_isadj_list(g, st, idx[0], idx[2]);
}

/**
 * La funzione legge da file gli archi dei grafi e salva in una tabella di simboli tutti i vertici trovati
 * Return: numero di vertici
 */ 
int st_fill(st_t st, FILE* fp) {
    keys_t k1, k2;
    int n_v=0;
    int tmp;
    while(fscanf(fp, "%s %s %s %s %*d", k1.elab, k1.net, k2.elab, k2.net) == 4) {
        //Se l'elaboratore di partenza non si trova nella tabella di simboli, aggiungilo
        if(st_searchByKey(st, k1) < 0) {
            st_insert(st, k1);
            n_v++;
        }
        //Se l'elaboratore di arrivo non si trova nella tabella di simboli, aggiungilo
        if(st_searchByKey(st, k2) < 0){
            st_insert(st, k2);
            n_v++;
        }
    }
    return n_v;
}

/**
 * La funzione inizializza un grafo leggendo i suoi archi da file
 */ 
void graph_fill(graph_t g, st_t st, FILE* fp) {
    keys_t k1, k2;
    int flow;
    while(fscanf(fp, "%s %s %s %s %d", k1.elab, k1.net, k2.elab, k2.net, &flow)==5)
        graph_insertEdge(g, st_searchByKey(st, k1), st_searchByKey(st, k2), flow);
}