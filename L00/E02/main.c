#include <stdio.h>
#include <string.h>

#define MAX_NOMEFILE 50
#define MAX_PAROLA 20
#define N_VOCALI 5

int conta(char[], int);
int vocale(char);

int main() {

    //INPUT NOME FILE
    char file[MAX_NOMEFILE];
    printf("Inserire nome del file: ");
    scanf("%s", file);

    FILE* fp = fopen(file, "r");
    if(!fp) {
        printf("File non trovato\n");
        return -1;
    }

    //INPUT LUNGHEZZA SOTTOSTRINGHE
    int n;
    do {
        printf("Inserire lunghezza sottostringhe: ");
        scanf("%d", &n);
        if(n<=0)
            printf("Lunghezza non valida.\n");
    } while(n<=0);

    int n_parole;
    fscanf(fp, "%d\n", &n_parole);

    for(int i=0; i<n_parole; i++) {
        char s[MAX_PAROLA];
        fscanf(fp, "%s\n", s);
        printf("Sottostringhe contenenti due vocali nella parola \"%s\": %d\n", s, conta(s, n));
    }
    
    fclose(fp);

    return 0;
}

int conta(char s[], int n) {
    int ret = 0;
    int l = strlen(s);
    for(int j=0; j<l-n+1; j++) {
        int n_vocali = 0;
        char ss[n]; //VARIABILE TEMPORANEA SOTTOSTRINGA
        for(int k=0; k<n; k++) {
            ss[k] = s[j+k];
            if(vocale(ss[k]))
                n_vocali++;
        }
        if(n_vocali == 2)
            ret++;
    }

    return ret;
}

int vocale(char c) {
    const char VOCALI[N_VOCALI*2] = {'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
    for(int i=0; i<N_VOCALI*2; i++)
        if(c == VOCALI[i])
            return 1;
    return 0;
}