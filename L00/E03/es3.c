#include <stdio.h>

void ruota(int v[], int N, int P, int dir);
void ruota_alt(int v[], int N, int P, int dir);
void stampaVettore(int v[]);

int main() {
    
    //INPUT LUNGHEZZA VETTORE
    int N;
    do {
        printf("Inserire la lunghezza del vettore: ");
        scanf("%d", &N);
        if(N<=0)
            printf("Lunghezza invalida.\n");
    } while(N<=0);

    //INPUT VETTORE
    int v[N];
    printf("Inserire i dati del vettore:\n");
    for(int i=0; i<N; i++) {
        printf("Dato %d: ", i+1);
        scanf("%d", v+i);
    }

    //Stampa vettore appena preso in input
    printf("Vettore input: ");
    stampaVettore(v);

    int P,dir;
    while(1) {
        //INPUT P
        do {
            printf("Inserire il numero di rotazioni (0 per terminare): ");
            scanf("%d", &P);
            if(P<0)
                printf("Numero di rotazioni invalide.\n");
        } while(P<0);

        //Termina il ciclo se il numero di rotazioni è nullo
        if(P==0) {
            printf("Termine ciclo...\n");
            break;
        }

        //INPUT dir
        do {
            printf("Inserire la direzione in cui ruotare (dx:1, sx:-1): ");
            scanf("%d", &dir);
            if(dir != 1 && dir != -1)
                printf("Direzione invalida.\n");
        } while(dir != 1 && dir != -1);

        //Tutti i dati sono validi, si procede con la rotazione e la stampa del vettore
        ruota_alt(v, N, P, dir);
        printf("Vettore risultato: ");
        stampaVettore(v);
    }

    return 0;
}

/**
 * Funzione che ruota un vettore a destra o sinistra di P posizioni. Complessità O(N*P).
 * L'algoritmo consiste nel ruotare il vettore di 1 posizione per P volte, usando come variabile temporanea un solo intero.
 */ 
void ruota(int v[], int N, int P, int dir) {
    int tmp;
    for(int i=0; i<P; i++) {
        if(dir > 0) {
            //ROTAZIONE A DESTRA
            int last = v[N-1];
            for(int j=N-1; j>0; j--)
                v[j] = v[j-1];
            v[0] = last;
        } else {
            //ROTAZIONE A SINISTRA
            int first = v[0];
            for(int j=0; j<N-1; j++) 
                v[j] = v[j+1];
            v[N-1] = first; 
        }
    }
}

/**
 * Funzione che ruota un vettore a destra o sinistra. Questa versione è più ottimizzata (complessità O(N)).
 * L'algoritmo consiste nel ruotare direttamente il vettore di P posizioni, usando come variabile temporanea un vettore lungo P.
 */ 
void ruota_alt(int v[], int N, int P, int dir) {
    if(P>N)
        P=P%N;
    int tmp[P], i;
        
    if(dir > 0) {
        //ROTAZIONE A DESTRA
        for(i=0; i<P; i++)
            tmp[i] = v[i+N-P];
        for(i=N-P-1; i>=0; i--)
            v[i+P] = v[i];
        for(i=0; i<P; i++)
            v[i] = tmp[i];
    } else {
        //ROTAZIONE A SINISTRA
        for(i=0; i<P; i++)
            tmp[i] = v[i];
        for(i=P; i<N;i++)
            v[i-P] = v[i];
        for(i=0; i<P; i++)
            v[N-P+i] = tmp[i];
    }
}

void stampaVettore(int v[]) {
    printf("[ ");
    for(int i=0; i<6; i++)
        printf("%d ", v[i]);
    printf("]\n");
}