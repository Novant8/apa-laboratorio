#include <stdio.h>
#include <string.h>

#include "Graph.h"

#define FILE_LABIRINTO "labirinto.txt"
#define MAX_CMD 17
#define MAX_FILE 30

typedef enum {
    c_help,
    c_verifica_percorso,
    c_percorso_migliore,
    c_fine,
    c_sconosciuto
} cmd_e;

cmd_e getcmd();
void handlecmd(cmd_e cmd, Graph g);

//VARIABILE GLOBALE: stringhe comandi
char* cmds[MAX_CMD+1] = {"help", "verifica_percorso", "percorso_migliore", "fine"};

int main() {
    FILE* fp = fopen(FILE_LABIRINTO, "r");
    if(fp==NULL) {
        printf("Errore in lettura file\n");
        return -1;
    }

    Graph g = GRAPHload(fp);
    cmd_e cmd;
    
    do {
        cmd = getcmd();
        handlecmd(cmd, g);
    } while(cmd!=c_fine);

    GRAPHfree(g);
    return 0;
}

cmd_e getcmd() {
    cmd_e cmd;
    char cmd_str[MAX_CMD+1];
    printf("Inserire comando (\"help\" per una lista dei comandi): ");
    scanf("%s", cmd_str);
    for(cmd=c_help; cmd<c_sconosciuto && strcmp(cmd_str, cmds[cmd]);cmd++);
    return cmd;
}

void handlecmd(cmd_e cmd, Graph g) {
    char filename[MAX_FILE];
    FILE* fp;
    Path p;
    int M, PF;
    char stanza[MAX_STANZA];

    //Input M e PF se i comandi lo prevedono
    if(cmd<c_fine && cmd>c_help) {
        printf("Inserire numero massimo di mosse: ");
        scanf("%d", &M);
        printf("Inserire numero di punti ferita: ");
        scanf("%d", &PF);
    }

    switch(cmd) {
        case c_help:
            printf("Lista comandi:\n");
            for(;cmd<c_sconosciuto;cmd++)
                printf("%s\n", cmds[cmd]);
            break;
        case c_verifica_percorso:
            printf("Inserire nome file percorso: ");
            scanf("%s", filename);
            fp = fopen(filename, "r");
            if(fp==NULL) {
                printf("Errore in lettura file\n");
                return;
            }
            p = GRAPHpathLoad(g, fp);
            if(GRAPHpathCheck(g, &p, M, PF))
                printf("Il percorso è valido. Ricchezza ottimale: %d\n", p.ricchezza);
            else
                printf("Il percorso non è valido.\n");
            break;
        case c_percorso_migliore:
            p = GRAPHpathBest(g, M, PF);
            printf("Un percorso migliore è il seguente: ");
            for(int i=0; i<p.n_stanze; i++) {
                GRAPHgetStanza(g, p.stanze[i], stanza);
                printf("%s ", stanza);
            }
            printf("\nRicchezza: %d\n", p.ricchezza);
            break;
        case c_fine:
            break;
        default:
            printf("Comando sconosciuto\n");
    }
}