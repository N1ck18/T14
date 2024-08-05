#include "io.h"

void input(char *str) {
    char ch;
    int i = 0;
    while ((ch = getchar()) && ch != '\n') {
        str[i] = ch;
        i++;
        if (i > 254) {
            str = realloc(str, sizeof(str) * 2);
        }
    }
    str[i + 1] = '\0';
}