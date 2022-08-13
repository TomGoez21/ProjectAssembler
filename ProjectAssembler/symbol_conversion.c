#include "symbol_conversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


char symbols[32] = { '!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v' };

char* decimalToSymbolsBase(int num) {
    char bin[10];
    static char SymbolsChars[3];
    int  pos = 1 ,count = 0, sb = 0, i = 0 , j;
    unsigned int abs = num;
    char temp;
    for (; i < 10; i++)
    {
        bin[i] = abs % 2 + '0';
        abs = abs / 2;
    }
    for (j = 0;j < 5;j++) {
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }
    for (i = 9;i > -1;i--) {
        sb += pow(2, count) * (bin[i] - '0');
        count++;
        if (count == 5) {
            SymbolsChars[pos] = symbols[sb];
            pos--;
            count = 0;

            sb = 0;
        }
    }
    SymbolsChars[2] = '\0';
    return SymbolsChars;
}

char* binToSymbolsBase(char* bin) {
    int pos = 1;
    int num = 0;
    int count = 0;
    int i = 0;
    static char SymbolsChars[3];
    for (i = 9;i > -1;i--) {
        num += pow(2, count) * (bin[i] - '0');
        count++;
        if (count == 5) {
            SymbolsChars[pos] = symbols[num];
            num = 0;
            count = 0;
            pos--;
        }
    }
    SymbolsChars[2] = '\0';
    return SymbolsChars;
}

int binToDecimal(char* bin) {
    int i;
    int num = 0;

    for (i = 9;i > -1;i--) {
        num += pow(2, 9 - i) * (bin[i] - '0');
    }
    if (bin[0] == '1')
        return 0 - (pow(2, 10) - num);
    return num;
}

char* decimalToBin(int num) {
    unsigned int i = 0, abs = num;
    int j;
    char temp;
    char* bin = (char*)calloc(11, sizeof(char));
    if (!bin)
    {
        printf("`Dynamic allocation error.\0");
        return NULL;
    }
    while (i < 10) {
        bin[i] = abs % 2 + '0';
        abs = abs / 2;
        i++;
    }
    for (j = 0;j < 5;j++) {
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }
    bin[10] = '\0';
    return bin;
}
