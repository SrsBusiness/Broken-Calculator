#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int buffer_size = 100000;

int matched(char **, int);

int main(){
    char *buffer = malloc(buffer_size);
    char *save = buffer;
    int is_matched;
    while(1){
        buffer = save;
        memset(buffer, 0, buffer_size);
        fgets(buffer, buffer_size, stdin);
        if(!buffer | *buffer == 0)
            break;
        is_matched = matched(&buffer, 0);
        assert(is_matched);
        //printf("%d\n", is_matched);
    }
}

int matched(char **string, int level){
    while(**string && **string != ')'){
        if(**string == '('){
            (*string)++;
            if(!matched(string, level + 1))
                return 0;
        }
        else
            (*string)++; 
    }
    if(!(!!**string ^ !!level)){
        (*string)++;
        return 1;
    }
    else
        return 0;
}
