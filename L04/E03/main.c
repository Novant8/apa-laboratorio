#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TESTO 100
#define MAX_REGEXP 50

char* cercaRegexp(char *src, char *regexp);
int getMatch(char *src, char *regexp);
int validaRegex(char* regexp);

int main() {

    /* INPUT TESTO */
    char testo[MAX_TESTO+1];
    printf("Inserire una successione di caratteri: ");
    fgets(testo, MAX_TESTO+1, stdin);

    /* INPUT REGEX */
    char regexp[MAX_REGEXP+1];
    printf("Inserire un'espressione regolare: ");
    scanf("%s", regexp);

    int regex_len = validaRegex(regexp);
    if(regex_len<0) {
        printf("Regexp inserita invalida.\n");
        return -1;
    }

    char* match = cercaRegexp(testo, regexp);    

    if(match != NULL) {
        char match_str[regex_len+1];
        strncpy(match_str, match, regex_len);
        match_str[regex_len] = '\0';
        printf("È stata trovata l'occorrenza \"%s\" nel testo.\n", match_str);
    } else {
        printf("Non è stata trovata alcuna occorrenza nel testo.\n");
    }

    return 0;
}

char* cercaRegexp(char *src, char *regexp) {
    int length;

    /* Scorri la stringa src carattere per carattere, e controlla a ogni iterazione se i primi caratteri rispettano la regex */
    while(*src != '\0') {
        if(getMatch(src, regexp)) //Se la lunghezza trovata è maggiore di zero (diverso da -1), allora è stata trovata un'occorrenza
            return src;
        src++;
    }
    return NULL;
}

/**
 * La funzione controlla se i primi n caratteri della stringa src rispettano la regex regexp.
 * n dipende dalla lunghezza della regex
 * Return:
 * - 1 se è stata trovata la match
 * - 0 se non è stata trovata la match
 */ 
int getMatch(char *src, char *regexp) {
    while(*regexp != '\0' && *src != '\0') {
        switch(*regexp) {
            case '[':
                regexp++;
                int found = 0, excluded = 0;

                if(*regexp == '^') {
                    excluded = 1;
                    regexp++;
                }

                /* Il ciclo non si interrompe alla prima occorrenza, si va avanti finchè viene raggiunta una parentesi chiusa in ogni caso */
                while(*regexp != '\0' && *regexp != ']') {
                    if(*src == *regexp)
                        found = 1;
                    regexp++;
                }

                if(excluded == found) //Se le booleane sono entrambe vere o entrambe false, allora la sottostringa non è una match
                    return 0;
                break;
            case '\\':
                regexp++;
                    if((*regexp == 'A' && !isupper(*src))   /* - Se il carattere successivo allo slash è una A maiuscola e il caratterre da analizzare di src è minuscolo */
                    || (*regexp == 'a' && !islower(*src)))  /* - Se il carattere successivo allo slash è una a minuscola e il carattere da analizzare di src è maiuscolo */
                    return 0;                              /* Allora la sottostringa non è una match per la regex. */
                break;
            case '.':
                break;
            default:
                if(*regexp != *src)
                    return 0;
        }
        regexp++;
        src++;
    }
    return 1;
}

/**
 * La funzione verifica che la regex inserita sia valida.
 * Return:
 * - La lunghezza dell'ipotetica match se la regex è valida
 * - -1 se la regex non è valida
 */ 
int validaRegex(char* regexp) {
    int len = 0;
    while(*regexp != '\0') {
        switch(*regexp) {
            case '[':
                regexp++;
                if(*regexp == '^')
                    regexp++;
                while(*regexp != ']') {
                    if(!isalpha(*regexp))
                        return 0;
                    regexp++;
                }
                break;
            case '\\':
                regexp++;
                if(tolower(*regexp) != 'a')
                    return 0;
        }
        regexp++;
        len++;
    }
    return len;
}