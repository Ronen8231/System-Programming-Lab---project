/**************************************************************************

    File:           support.c

    Written By:     Ronen Hanukayev
    ID:             322881327

    Description:    This is the implementation of the support.h header. 

**************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include "errors.h"
#include "memory_words.h"



void reverse(char* s){
    int i, j;
    char c;
    
    for(i = 0, j = strlen(s) - 1; i<j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


int countDigits(int num){
    int count = 0;
    while(num!=0){
        num /= 10;
        count++;
    }
    return count;
}


/* int to string */
char* itoa(int num){
    int i, sign;
    char* s = (char*) malloc(countDigits(num) + 2);
    memset(s, 0, countDigits(num)+2);
    if((sign = num) < 0) /* save sign */
        num = -num; /* make positive */
    i = 0;
    do{
        s[i++] = num % 10 + '0';
    } while((num /= 10) > 0);
    if(sign < 0)
        s[i++] = '-';
    s[i] = 0;
    reverse(s);
    return s;
}


/* must not have non-zero bits after the sixth position 
    the transition is according to the table at the wikipedia page */
char base64(int num){
    if(num >= 0 && num <= 25)
        return num + 'A';
    if(num >= 26 && num <= 51)
        return num - 26 + 'a';
    if(num >= 52 && num <= 61)
        return num - 52 + '0';
    if(num == 62)
        return '+';
    if(num == 63)
        return '/';
    return 0; /* unreachable if num is valid */
}


/* returned string should be freed after use */
char* wordToBase64(int word){
    int i;
    int mask = 0;
    int first_half;/* first 6 bits*/
    int second_half;/* second 6 bits */
    char* str = (char*)malloc(BASE64_LINE_SIZE + 1); /* size needed plus null terminator */
    
    for(i = 0; i < WORD_SIZE/2; i++){
        mask <<= 1;
        mask += 1;
    }
    
    first_half = (word & mask);
    word >>= (WORD_SIZE/2);
    second_half = (word & mask);
    str[0] = base64(second_half);
    str[1] = base64(first_half);
    str[2] = 0; /* null terminator */
    return str; 
}


char* read_line(FILE* file){
    char* line = (char*)malloc(MAX_LINE_LENGTH + 1);
    char c;
    int i = 0;

    memset(line, 0, MAX_LINE_LENGTH + 1);

    c = fgetc(file);
    if(c == EOF){
        free(line);
        return EOF_REACHED;
    }

    line[i++] = c;

    while(i < MAX_LINE_LENGTH){
        c = fgetc(file);
        if(c == '\n')
            return line;
        if(c == EOF){
            return line;
        }
        line[i++] = c;
    }
    
    free(line);    
    return LINE_LENGTH_EXCEEDED;
} 

void replace_tabs_with_spaces(char *str){
	int pos = 0;
	while(str[pos] != '\0'){
		if(str[pos] == '\t')
			str[pos] = ' ';
		pos++;
	}
}
