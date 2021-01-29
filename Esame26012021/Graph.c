#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "ST.h"

typedef struct stanza_s {
    int profondita;
    int oro;
    int tesoro;
} Stanza;

typedef struct vertex_s {
    int c; //indica se le stanze sono connesse
    int t; //indica se è presente una trappola
} Cunicolo;

struct graph_s {
    Stanza* stanze;
    Cunicolo** madj;
    int n_stanze;
    ST st;
};

static Cunicolo** malloc2dSqCunicolo(int n) {
    Cunicolo** m = (Cunicolo**)malloc(sizeof(Cunicolo*)*n);
    for(int i=0; i<n; i++)
        m[i] = (Cunicolo*)calloc(n, sizeof(Cunicolo));
    return m;
}

Graph GRAPHinit(int n_stanze) {
    Graph g = (Graph)malloc(sizeof(struct graph_s));
    g->stanze = (Stanza*)malloc(sizeof(Stanza)*n_stanze);
    g->madj = malloc2dSqCunicolo(n_stanze);
    g->n_stanze = n_stanze;
    g->st = STinit(n_stanze);
    return g;
}

Path PATHinit(int n_stanze) {
    Path p;
    p.n_stanze = n_stanze;
    p.stanze = (int*)malloc(sizeof(int)*n_stanze);
    p.ricchezza = 0;
    return p;
}

Graph GRAPHload(FILE* fp) {
    int n_stanze;
    int trappola;
    char stanza1[MAX_STANZA+1], stanza2[MAX_STANZA+1];
    int i,j;
    fscanf(fp, "%d", &n_stanze);
    Graph g = GRAPHinit(n_stanze);
    for(i=0; i<n_stanze; i++) {
        fscanf(fp, "%s %d %d %d", stanza1, &g->stanze[i].profondita, &g->stanze[i].tesoro, &g->stanze[i].oro);
        STadd(g->st, stanza1);
    }
    while(fscanf(fp, "%s %s %d", stanza1, stanza2, &trappola) != EOF) {
        i = STgetIndex(g->st, stanza1);
        j = STgetIndex(g->st, stanza2);
        g->madj[i][j].c = g->madj[j][i].c = 1;
        g->madj[i][j].t = g->madj[j][i].t = trappola;
    }
    return g;
}

Path GRAPHpathLoad(Graph g, FILE* fp) {
    Path p;
    char stanza[MAX_STANZA+1];
    int n_stanze;
    int i, idx;
    fscanf(fp, "%d", &n_stanze);
    p = PATHinit(n_stanze);
    for(i=0; i<n_stanze; i++) {
        fscanf(fp, "%s", stanza);
        idx = STgetIndex(g->st, stanza);
        p.stanze[i] = idx;
    }
    return p;
}

static int visited(Path* p, int stanza, int len) {
    for(int i=0; i<len; i++)
        if(stanza == p->stanze[i])
            return 1;
    return 0;
}

int GRAPHpathCheck(Graph g, Path* p, int M, int PF) {
    int i, s1, s2;

    //Controllo sulla prima stanza del percorso: se non è un ingresso allora il percorso non è valido
    if(g->stanze[0].profondita>0)
        return 0;

    p->ricchezza = 0;
    int max_tesoro = g->stanze[0].tesoro;
    for(i=1; i<p->n_stanze; i++) {
        s1 = p->stanze[i-1];
        s2 = p->stanze[i];
        //Se le due stanze non sono connesse, allora il percorso non è valido
        if(!g->madj[s1][s2].c)
            return 0;
        //Se la stanza visitata è un ingresso/uscita e non è l'ultima stanza del percorso, allora il percorso non è valido
        if(g->stanze[p->stanze[i]].profondita == 0 && i!=p->n_stanze-1)
            return 0;
        //Controllo se è presente una trappola: in quel caso viene tolto un punto ferita
        if(g->madj[s1][s2].t)
            PF--;
        if(PF<=0) {
            i++; //Incremento dell'indice per marcare la stanza corrente come ultima (anzichè quella precedente)
            break;
        }
        if(i>=M)
            break;
        if(!visited(p, s2, i)) {
            p->ricchezza+=g->stanze[s2].oro;
            if(g->stanze[s2].tesoro > max_tesoro)
                max_tesoro = g->stanze[s2].tesoro;
        }
    }

    p->ricchezza += max_tesoro;

    //Controllo se il percorso risulta "incompleto"
    if(g->stanze[p->stanze[i-1]].profondita > 0 && i<M && PF>0)
        return 0;

    //Controllo sull'interruzione del percorso
    switch(g->stanze[p->stanze[i-1]].profondita) {
        case 0:
            break;
        case 1: //soccorso, livello 1
            p->ricchezza *= (float)2/3;
            break;
        case 2: //soccorso, livello 2
            p->ricchezza /= 2;
            break;
        default:
            return 0;
    }
    return 1;
}

static void GRAPHpathBestR(int pos, Graph g, int M, int PF, Path p, Path* best, int max_tesoro) {
    int i, tmp, tmp_tesoro;
    /* Condizione di terminazione: raggiunto il numero di mosse massimo, punti ferita terminati oppure la stanza appena visitata è un'uscita */
    if(pos >= M || PF <= 0 || (pos > 1 && g->stanze[p.stanze[pos-1]].profondita == 0)) {
        //Controllo sull'interruzione del percorso
        switch(g->stanze[p.stanze[pos-1]].profondita)  {
            case 0: //percorso normale
                break;
            case 1: //soccorso, livello 1
                p.ricchezza *= (float)2/3;
                break;
            case 2: //soccorso, livello 2
                p.ricchezza /= 2;
                break;
            default: //fallimento
                return;
        }
        p.ricchezza += max_tesoro;
        if(p.ricchezza > best->ricchezza) {
            for(i=0; i<pos; i++)
                best->stanze[i] = p.stanze[i];
            best->ricchezza = p.ricchezza;
            best->n_stanze = pos;
        }
        return;
    }
    for(i=0; i<g->n_stanze; i++) {
        if(pos==0) {
            //La prima stanza da visitare deve avere profondità 0
            if(g->stanze[i].profondita == 0) {
                p.stanze[pos] = i;
                GRAPHpathBestR(pos+1, g, M, PF, p, best, max_tesoro);
            }
        } else if(g->madj[p.stanze[pos-1]][i].c) { //Se la stanza i e quella visitata precedentemente sono connesse...
            //Salvataggio soluzione
            p.stanze[pos] = i;
            //Collezione dell'oro: aumento della ricchezza del percorso e rimozione dell'oro dalla stanza
            p.ricchezza += g->stanze[i].oro;
            tmp = g->stanze[i].oro;
            g->stanze[i].oro = 0;
            //Ricorsione
            GRAPHpathBestR(pos+1, g, M, g->madj[p.stanze[pos-1]][i].t ? PF-1 : PF, p, best, g->stanze[i].tesoro > max_tesoro ? g->stanze[i].tesoro : max_tesoro);
            //Ripristino dei dati a prima della ricorsione
            g->stanze[i].oro = tmp;
            p.ricchezza -= g->stanze[i].oro;
        }
    }
}

void GRAPHgetStanza(Graph g, int id, char* key) {
    STgetKey(g->st, id, key);
}

Path GRAPHpathBest(Graph g, int M, int PF) {
    Path best, p;
    best = PATHinit(M);
    p = PATHinit(M);
    GRAPHpathBestR(0, g, M, PF, p, &best, 0);
    return best;
}

static void free2dCunicolo(Cunicolo** v, int n) {
    for(int i=0; i<n; i++)
        free(v[i]);
    free(v);
}

void GRAPHfree(Graph g) {
    free2dCunicolo(g->madj, g->n_stanze);
    free(g->stanze);
    STfree(g->st);
    free(g);
}