#include <stdio.h>

#define MAX_FILE 20
#define MAX_SQUADRE 20
#define MAX_GIORNATE 20

typedef struct {
    int m[MAX_SQUADRE][MAX_GIORNATE];
    int squadre;
    int giornate;
}Campionato;

FILE* getFile();
Campionato getCampionato(FILE* fp);

int main() {

    FILE* fp = getFile();
    if(!fp) {
        printf("File non trovato.\n");
        return -1;
    }

    Campionato c = getCampionato(fp);
    int classifica[MAX_SQUADRE] = {0};

    fclose(fp);

    int capolista;
    for(int i=0; i<c.giornate; i++) {
        int max_punteggio = -1;
        for(int j=0; j<c.squadre; j++) {
            classifica[j] += c.m[j][i];
            if(classifica[j] > max_punteggio) {
                max_punteggio = classifica[j];
                capolista = j;
            }
        }
        printf("Capolista giornata %d: Squadra %d\n", i+1, capolista+1);
    }
    printf("\nVincitore campionato: Squadra %d\n", capolista+1);

    return 0;
}

FILE* getFile() {
    char nomeFile[MAX_FILE];
    printf("Inserire il nome del file: ");
    scanf("%s", nomeFile);
    return fopen(nomeFile, "r");
}

Campionato getCampionato(FILE* fp) {
    int rows, cols;
    fscanf(fp, "%d %d", &rows, &cols);

    Campionato c;
    c.squadre = rows;
    c.giornate = cols;
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            fscanf(fp, "%d", &c.m[i][j]);
    
    return c;
}