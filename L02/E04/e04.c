#include <stdio.h>

#define MAX_FILE 20
#define MAX_SEQUENZA 100

FILE* getFile();
void leggi_seq(FILE* fp, int seq[], int l_seq);
void selection_sort(int seq[], int l_seq);
void insertion_sort(int seq[], int l_seq);
void shell_sort(int seq[], int l_seq);
void stampa_seq(int v[], int l);
void clona_seq(int seq[], int clone[], int l_seq);

int main()  {

    FILE* fp = getFile();
    if(!fp) {
        printf("Errore in lettura del file.\n");
        return -1;
    }

    int n_seq, l_seq;

    fscanf(fp, "%d", &n_seq);

    for(int i=0; i<n_seq; i++) {
        fscanf(fp, "%d", &l_seq);
        int seq[l_seq], clone[l_seq];

        leggi_seq(fp, seq, l_seq);

        printf("Sequenza %d: ", i+1);
        stampa_seq(seq, l_seq);
            
        printf("Selection sort:\n");
        clona_seq(seq, clone, l_seq);
        selection_sort(clone, l_seq);

        printf("Insertion sort:\n");
        clona_seq(seq, clone, l_seq);
        insertion_sort(clone, l_seq);

        printf("Shell sort:\n");
        clona_seq(seq, clone, l_seq);
        shell_sort(clone, l_seq);
    }

    return 0;
}

FILE* getFile() {
    char fileName[MAX_FILE];
    printf("Inserire nome file: ");
    scanf("%s", fileName);
    return fopen(fileName, "r");
}

void leggi_seq(FILE* fp, int seq[], int l_seq) {
    for(int i=0; i<l_seq; i++)
        fscanf(fp, "%d", seq+i);
}

void clona_seq(int seq[], int clone[], int l_seq) {
    for(int i=0; i<l_seq; i++)
        clone[i] = seq[i]; 
}

void selection_sort(int seq[], int l_seq) {
    int iterazioni = 0;
    for(int i=0; i<l_seq-1; i++) {
        int min_j=i, tmp;
        for(int j=i+1; j<l_seq; j++) {
            if(seq[j] < seq[min_j])
                min_j = j;
            iterazioni++;
        }
        if(min_j != i) {
            tmp = seq[i];
            seq[i] = seq[min_j];
            seq[min_j] = tmp;
        }
        iterazioni++;
    }

    printf("Iterazioni totali: %d\n", iterazioni);
    printf("Sequenza ordinata: ");
    stampa_seq(seq, l_seq);
}

void insertion_sort(int seq[], int l_seq) {
    int iterazioni = 0;

    for(int i=1; i<l_seq; i++) {
        int tmp=seq[i], j;
        for(j=i; j > 0 && seq[j-1]>tmp; j--) {
            seq[j] = seq[j-1];
            iterazioni++;
        }
        seq[j] = tmp;
        iterazioni++;
    }

    printf("Iterazioni totali: %d\n", iterazioni);
    printf("Sequenza ordinata: ");
    stampa_seq(seq, l_seq);
}

void shell_sort(int seq[], int l_seq) {
    int h=1, iterazioni=0;
    
    while(h<l_seq/3)
        h=3*h+1;
    while(h>=1) {
        for(int i=h; i<=l_seq-1; i++) {
            int tmp = seq[i], j;
            for(j=i; j>=h && seq[j-h]>tmp; j-=h) {
                seq[j] = seq[j-h];
                iterazioni++;
            }
            seq[j] = tmp;
            iterazioni++;
        }
        h/=3;
    }
    printf("Iterazioni totali: %d\n", iterazioni);
    printf("Sequenza ordinata: ");
    stampa_seq(seq, l_seq);
}

void stampa_seq(int v[], int l) {
    printf("[ ");
    for(int i=0; i<l; i++)
        printf("%d ", v[i]);
    printf("]\n");
}