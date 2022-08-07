#include "symbol_conversion.h"

char symbols[32] = { '!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v' };

char * decimalToSymbolsBase(int num){    
    int j, count=0, sb=0, position=1, i=0;
    unsigned int abs = num;
    char temp;
    char bin[10];
    char * SymbolsChars = malloc(3*sizeof(char));    
    while(i<10){
        bin[i]= abs%2+'0';
        abs=abs/2;
        i++;
    }
    for (j=0;j<5;j++){
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }    
    if(!SymbolsChars)
        return "`Dynamic allocatio error.\0";
    for (i=9;i>-1;i--){
        sb+=pow(2,count)*(bin[i]-'0');
        count++;
        if(count==5){
            SymbolsChars[position]=symbols[sb];
            position--;
            count=0;

            sb=0;
        }
    }
    SymbolsChars[2]='\0';
    return SymbolsChars;    
}

int binToDecimal(char *bin){
    int i;
    int num=0;

    for (i=9;i>-1;i--){
        num+=pow(2,9-i)*(bin[i]-'0');
    }
    if(bin[0]=='1')
        return 0-(pow(2,10)-num);
    return num;
}


char * decimalToBin(int num){
    unsigned int i=0,abs = num;
    int j;
    char temp;
    char * bin = (char *)malloc(11* sizeof(char));
    if (!bin)
        return "`Dynamic allocatio error.\0";
    while(i<10){
        bin[i]= abs%2+'0';
        abs=abs/2;
        i++;
    }
    for (j=0;j<5;j++){
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }    
    bin[10] = '\0';
    return bin;
}


char *binToSymbolsBase(char *bin){
    int count=0;
    int num=0;
    int i=0;
    int position=1;
    char * SymbolsChars = malloc(3*sizeof(char));
    if(!SymbolsChars)
        return "`Dynamic allocatio error.\0";
    for (i=9;i>-1;i--){
        num+=pow(2,count)*(bin[i]-'0');
        count++;
        if(count==5){
            SymbolsChars[position]=symbols[num];
            position--;
            count=0;

            num=0;
        }
    }
    SymbolsChars[2]='\0';
    return SymbolsChars;
}