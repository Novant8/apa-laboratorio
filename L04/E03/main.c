#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TESTO 100
#define MAX_REGEXP 50

char* cercaRegexp(char *src, char *regexp);
int getMatch(char *src, char *regexp);

int main() {

    /* INPUT TESTO */
    char testo[MAX_TESTO+1];
    printf("Inserire una successione di caratteri: ");
    fgets(testo, MAX_TESTO+1, stdin);

    /* INPUT REGEX */
    char regexp[MAX_REGEXP+1];
    printf("Inserire un'espressione regolare: ");
    scanf("%s", regexp);

    /*if(validaRegex(regexp)<=0) {
        printf("Regexp invalida.\n");
        return -1;
    }*/

    char* match = cercaRegexp(testo, regexp);

    if(match != NULL) {
        printf("È stata trovata l'occorrenza \"%s\" nel testo.\n", match);
    } else {
        printf("Non è stata trovata alcuna occorrenza.\n");
    }

    return 0;
}

char* cercaRegexp(char *src, char *regexp) {
    int length;
    char match[MAX_REGEXP];

    while(*src != '\0') {
        length = getMatch(src, regexp);
        if(length>0)
            return strncpy(match, src, length);
        src++;
    }
    return NULL;
}

int getMatch(char *src, char *regexp) {
    int length = 0;
    while(*regexp != '\0' && *src != '\0') {
        switch(*regexp) {
            case '[':
                regexp++;
                int found = -1, excluded = -1;

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

                if(excluded * found == 1) //Equivalente a un XAND tra excluded e found
                    return -1;
                break;
            case '\\':
                regexp++;
                if(tolower(*regexp) != 'a'                  /* - Se il carattere successivo allo slash non è una A (maiuscola o minuscola) */
                    || (*regexp == 'A' && !isupper(*src))   /* - Se il carattere successivo allo slash è una A maiuscola e il caratterre da analizzare di src è minuscolo */
                    || (*regexp == 'a' && !islower(*src)))  /* - Se il carattere successivo allo slash è una a minuscola e il carattere da analizzare di src è maiuscolo */
                    return -1;                              /* Allora la sottostringa non è una match per la regex. */
                break;
            case '.':
                break;
            default:
                if(*regexp != *src)
                    return -1;
        }
        regexp++;
        src++;
        length++;
    }
    return length;
}