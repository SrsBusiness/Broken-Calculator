#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char operators[4] = {'+', '-', '*', '/'};

int gen(char *, int, int, int);

int main(){
    srand(time(NULL));
    char expr[1000];
    int i;
    for(i = 0; i < 1000; i++){
        expr[gen(expr, 0, 1000, 0)] = 0;
        printf("%s\n", expr);
    }
}

int rand_bool(int);

int gen(char *string, int current_index, int max_len, int level){
    while(max_len > current_index + 12 + 2 * level){
        // operand 
        // 50% parenthesis
        // 50% chance integer
        if(rand_bool(30) && max_len > current_index + 12 + 2 * (level + 1) + 1 ) {
            string[current_index++] = '(';
            current_index = gen(string, current_index, max_len, level + 1);
            string[current_index++] = ')';
            // parens

        }else{
            // operand
            current_index += sprintf(string + current_index, "%d", (rand() % 10) + 1);
        }
        string[current_index++] = ' ';
        if((level && rand_bool(70)) || max_len <= current_index + 11 + 2 * level + 3) 
            return current_index;
        
        //choose operator
        string[current_index++] = operators[(rand_bool(50) << 1) | rand_bool(50)];
        string[current_index++] = ' ';
    }
    return current_index;
}

int rand_bool(int percent){
    // generates 1 or 0
    // percent is the percent chance of returning 1
    return ((rand() % 100) <= percent);
}
