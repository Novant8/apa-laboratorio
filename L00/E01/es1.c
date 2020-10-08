#include <stdio.h>

#define N_MAX 30

typedef struct {
    int pos;
    int length;
}Sottovettore;

void stampaVettore(int v[]);
void stampaSottovettore(int[], int);
int lunghezzaMassima(int v[], int v_len, Sottovettore sv[], int* n_sv);

int main()  {

    int v_len=0;
    int v[N_MAX];
    printf("Inserire una serie di numeri interi maggiori o uguali a 0. Inserire un numero negativo per terminare l'inserimento.\n");
    do {
        scanf("%d", v+v_len);
        v_len++;
    } while(v_len<N_MAX-1 && v[v_len-1]>=0);

    printf("Vettore: ");
    stampaVettore(v);
    printf("\n");

    int n_sv; //NUMERO DI SOTTOVETTORI
    Sottovettore sv[(v_len+1)/2];
    int max = lunghezzaMassima(v, v_len, sv, &n_sv);

    if(max == 0) {
        printf("Il vettore inserito è composto da soli zeri.\n");
    } else {
        printf("Vettori di lunghezza massima (%d):", max);
        for(int i=0; i<n_sv; i++)
            if(sv[i].length == max)
                stampaSottovettore(v, sv[i].pos);
        printf("\n");
    }

    return 0;
}

void stampaVettore(int v[]) {
    printf("[ ");
    for(int i=0; i<N_MAX && v[i]>=0; i++)
        printf("%d ", v[i]);
    printf("]");
}

void stampaSottovettore(int v[], int pos) {
    printf("[");
    for(int i=pos; i<N_MAX && v[i]>0; i++)
        printf(" %d", v[i]);
    printf(" ], ");
}

int lunghezzaMassima(int v[], int v_len, Sottovettore sv[], int* n_sv) {
    int max = 0;
    *n_sv = 0;
    int l_sv = 0; //LUNGHEZZA SOTTOVETTORE
    int sv_start = 0;
    for(int i=0; i<v_len; i++) {
        if(v[i] <= 0) {
            sv[*n_sv].length = l_sv;
            if(l_sv > max)
                max = l_sv;
            l_sv = 0;
            sv[(*n_sv)].pos = sv_start;
            sv_start = i+1;
            (*n_sv)++;
        } else {
            l_sv++;
        }
    }
    return max;
}