#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ELEMENTS_FILE "elementi.txt"
#define N_PERFORMANCE_DIAGS 3
#define MAX_DIAGONAL 5
#define MAX_STR 100

typedef enum {
    transition, backward, forward
} type_e;

typedef struct {
    char name[MAX_STR+1];
    type_e type;
    int in_dir;
    int out_dir;
    int prec;
    int final;
    float score;
    int difficulty;
} acrob_t;

typedef struct {
    acrob_t* elements[MAX_DIAGONAL];
    int n_elements;
    float score;
    int difficulty;
} diagonal_t;

typedef struct {
    diagonal_t* diagonals[N_PERFORMANCE_DIAGS];
    float score;
    int difficulty;
    int bonus;
} performance_t;

acrob_t* getElements(int *n_elements);
void readElement(FILE* fp, acrob_t* element);
void maxScore(acrob_t* elements, int n_elements, int DD, int DP);
int generateDiags(int pos, acrob_t* elements, diagonal_t diag, diagonal_t* diagonals, int n_elements, int DD, int count);
void maxScore_R(int pos, diagonal_t* diagonals, performance_t performance, performance_t* max_per, int n_diagonals, int start, int DP);
int check_diags(int pos, acrob_t to_add, diagonal_t diag, int DD);
int check_performance(performance_t performance, diagonal_t next, int DP);
int valid_performance(performance_t performance);
void performance_score(performance_t *performance);
void printPerformance(performance_t performance);

int main() {

    int n_elements;
    acrob_t* elements = getElements(&n_elements);
    if(elements == NULL) {
        printf("Errore in lettura del file\n");
        return -1;
    }

    int DD, DP;
    printf("Inserire difficoltà massima diagonale: ");
    scanf("%d", &DD);

    printf("Inserire difficoltà massima programma: ");
    scanf("%d", &DP);

    maxScore(elements, n_elements, DD, DP);

    free(elements);
    
    return 0;
}

acrob_t* getElements(int *n_elements) {
    FILE* fp = fopen(ELEMENTS_FILE, "r");
    if(fp == NULL)
        return NULL;

    fscanf(fp, "%d", n_elements);
    acrob_t* elements = (acrob_t*)malloc(*n_elements*sizeof(acrob_t));
    
    acrob_t tmp;
    for(int i=0; i<*n_elements; i++)
        readElement(fp, elements+i);
    fclose(fp);
    
    return elements;
}

void readElement(FILE* fp, acrob_t* element) {
    fscanf(fp, "%s %d %d %d %d %d %f %d", 
                element->name,
                &element->type,
                &element->in_dir,
                &element->out_dir,
                &element->prec,
                &element->final,
                &element->score, 
                &element->difficulty);
}

void maxScore(acrob_t* elements, int n_elements, int DD, int DP) {
    int max_diagonals = pow(n_elements, MAX_DIAGONAL);
    acrob_t* diag_el[MAX_DIAGONAL];
    diagonal_t* diagonals = (diagonal_t*)malloc(sizeof(diagonal_t)*max_diagonals);
    
    //Generazione diagonale temporanea usata in ricorsione
    diagonal_t diag;
    diag.n_elements = 0;
    diag.score = 0;
    diag.difficulty = 0;

    int n_diagonals = generateDiags(0, elements, diag, diagonals, n_elements, DD, 0);
    
    printf("generate %d diagonali\n\n", n_diagonals);

    performance_t performance, max_per;
    performance.difficulty = 0;

    //Ricavo performance avente punteggio massimo
    maxScore_R(0, diagonals, performance, &max_per, n_diagonals, 0, DP);

    printPerformance(max_per);

    free(diagonals);
}

int generateDiags(int pos, acrob_t* elements, diagonal_t diag, diagonal_t* diagonals, int n_elements, int DD, int count) {
    int i;
    if(pos > MAX_DIAGONAL)
        return count;
    if(pos > 0) {
        diagonals[count] = diag;
        /*printf("{ ");
        for(i=0; i<diag.n_elements; i++)
            printf("%s ", diag.elements[i]->name);
        printf("}\n");*/
        count++;
    }
    for(i=0; i<n_elements; i++)
        if(check_diags(pos, elements[i], diag, DD)) {
            diag.elements[pos] = elements+i;
            diag.score += elements[i].score;
            diag.difficulty += elements[i].difficulty;
            diag.n_elements++;
            count = generateDiags(pos+1, elements, diag, diagonals, n_elements, DD, count);
            diag.score -= elements[i].score;
            diag.difficulty -= elements[i].difficulty;
            diag.n_elements--;
        }
    return count;
}

void maxScore_R(int pos, diagonal_t* diagonals, performance_t performance, performance_t* max_per, int n_diagonals, int start, int DP) {
    if(pos >= N_PERFORMANCE_DIAGS) {
        if(valid_performance(performance)) { 
            performance_score(&performance);
            if(performance.score > max_per->score)
                *max_per = performance;
        }
        return;
    }
    for(int i=start; i<n_diagonals; i++) {
        if(check_performance(performance, diagonals[i], DP)) {
            performance.diagonals[pos] = diagonals+i;
            performance.difficulty += diagonals[i].difficulty;
            maxScore_R(pos+1, diagonals, performance, max_per, n_diagonals, i, DP);
            performance.difficulty -= diagonals[i].difficulty;
        }
    }
}

/**
 * Controllo delle condizioni di pruning per la validità di una diagonale
 */ 
int check_diags(int pos, acrob_t to_add, diagonal_t diag, int DD) {
    if(pos == 0) {
        if(to_add.prec)
            return 0;
        if(!to_add.in_dir)
            return 0;
    } else { //pos>0
        if(diag.elements[pos-1]->final)
            return 0;
        if(diag.elements[pos-1]->out_dir != to_add.in_dir)
            return 0;
    }
    if(diag.difficulty + to_add.difficulty > DD)
        return 0;
    return 1;
}

/**
 * Controllo delle condizioni di pruning per la validità della performance
 */ 
int check_performance(performance_t performance, diagonal_t next, int DP) {
    if(performance.difficulty + next.difficulty > DP)
        return 0;
    return 1;
}

/**
 * Controllo delle condizioni di validità dopo la generazione di una performance
 */ 
int valid_performance(performance_t performance) {
    /* 
        Booleane che indicano se le condizioni di validità vengono rispettate. Rispettivamente:
        - fwd_el: il programma contiene un elemento in avanti
        - bwd_el: il programma contiene un elemento all'indietro
        - two_el: una diagonale contiene almeno due elementi in sequenza
    */
    int fwd_el = 0, bwd_el = 0, two_el = 0;
    for(int i=0; i<N_PERFORMANCE_DIAGS; i++) {
        if(performance.diagonals[i]->n_elements >= 2)
            two_el = 1;
        for(int j=0; j<performance.diagonals[i]->n_elements; j++) {
            switch(performance.diagonals[i]->elements[j]->type) {
                case 1:
                    fwd_el = 1;
                    break;
                case 2:
                    bwd_el = 1;
                break;
            }
        }
    }

    //Tutte le tre condizioni devono essere vere per la validità del programma
    return fwd_el && bwd_el && two_el;
}

/**
 * Funzione che calcola il punteggio di una performance e imposta il dato relativo.
 * Se la performance contiene delle diagonali contenenti elementi finali, allora quella avente punteggio più alto viene impostata come ultima e il suo punteggio viene moltiplicato per 1.5.
 */ 
void performance_score(performance_t *performance) {

    int max_idx = -1;
    float max_score = 0;
    int last_el_idx;
    diagonal_t* diag;
    //Ricava la diagonale contenente un elemento finale e avente il punteggio più alto
    for(int i=0; i<N_PERFORMANCE_DIAGS; i++) {
        last_el_idx = performance->diagonals[i]->n_elements-1;
        diag = performance->diagonals[i];
        if(/*diag->elements[last_el_idx]->final &&*/ diag->score > max_score) {
            max_idx = i;
            max_score = diag->score;
        }
    }
    
    //Se è stata trovata una diagonale contenente un elemento finale e avente punteggio più alto, e questa non è l'ultima, allora effettua uno scambio
    if(max_idx>=0 && max_idx<N_PERFORMANCE_DIAGS-1) {
        diag = performance->diagonals[max_idx];
        performance->diagonals[max_idx] = performance->diagonals[N_PERFORMANCE_DIAGS-1];
        performance->diagonals[N_PERFORMANCE_DIAGS-1] = diag;
    }

    //Ciclo di calcolo del punteggio
    performance->score = 0;
    acrob_t* last_el;
    float multiplier = 1;
    for(int i=0; i<N_PERFORMANCE_DIAGS; i++) {
        last_el_idx = performance->diagonals[i]->n_elements-1;
        last_el = performance->diagonals[i]->elements[last_el_idx];
        /*
        Se:
        - La diagonale è l'ultima del programma
        - L'ultimo elemento della diagonale è un elemento finale
        - La difficoltà dell'ultimo elemento è maggiore o uguale a 8
        allora la diagonale va moltiplicata per 1.5. 
        */
        if(i==N_PERFORMANCE_DIAGS-1 /*&& last_el->final*/ && last_el->difficulty >= 8) {
            multiplier = 1.5;
            performance->bonus = 1;
        } else {
            multiplier = 1;
            performance->bonus = 0;
        }
        performance->score += performance->diagonals[i]->score * multiplier;
    }
}

void printPerformance(performance_t performance) {
    printf("TOT = %.3f\n", performance.score);
    for(int i=0; i<N_PERFORMANCE_DIAGS; i++) {
        printf("DIAG #%d > %.3f %s\n", i+1, performance.diagonals[i]->score, i==N_PERFORMANCE_DIAGS-1 && performance.bonus ? "* 1.5 (BONUS)" : "");
        for(int j=0; j<performance.diagonals[i]->n_elements; j++)
            printf("%s ", performance.diagonals[i]->elements[j]->name);
        printf("\n");
    }
}