#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define OPERAND 0
#define OPERATOR 1
#define is_white(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\v' || (c) == '\f' || (c) == '\r')
#define is_op(c) ((c) == '+' || (c) == '-' || (c) == '*' || (c) == '/')
#define is_nan(d) ((d) != (d))

// supported operators:
// +, -, *, /, (, )
// all operands are assumed to be floating point

// useful: * and / are 2 (mod 5), + and - are not

int buffer_size = 10000;

void token(char **);
int precedence(char);
double evaluate(double, char **, int, int);
//double parens(char **);
double eval(char, double, double);


int main(int argc, char **argv){
    // initial input is 49 characters
    char *buffer = malloc(buffer_size);
    char *save = buffer;
    while(1){
        buffer = save;
        memset(buffer, 0, buffer_size);
        fgets(buffer, buffer_size, stdin);
        if(!buffer | *buffer == 0)
            break;
        double result = evaluate(NAN, &buffer, 0, 0);
        printf("%f\n", result);        
    }
    //char *old;
    //do{
    //    old = buffer; token(&buffer);
    //    printf("%s\n", old);
    //}while(*buffer);
}

// searches through string for the 1st whitespace character (the delimiter)
// substitutes it with '\0'
// moves the pointer to the beginning of the next token
//
void token(char **string){
    while(**string){ // not end of string
        if(is_white(**string)){ // found end of token
            **string = 0; // replace with string terminator
            // now search for next non-whitespace character
            do{
                (*string)++;
            }
            while(is_white(**string));
            return;
        }else
            (*string)++;
    }
}

// expect indicates what type of token
// is expected next
// 0 - operand
// 1 - operator
// mismatch between expect and the next token
// leads to NAN being returned
//
//
// expression may start with a parens or an operand
//
//
// if the next operator has lower precedent than current
// one, return 
// if next operator has higher precedent than current
// one, recurse
//
// expression may only end with an operand
//
double evaluate(double result, char **string, int expect_parens, int elevated){
    // we need to accumulate the value of the expression
    // as it is evaluated`
    // expect parens - whether or not closing parens is expected
    char *old;
    // set result to the first operand or,
    // the value of the parenthetical expression or,
    // if a value was passed in result, use that
    if(is_nan(result)){
        if(**string == '('){
            (*string)++;
            result = evaluate(NAN, string, 1, 0);
            if(is_nan(result)) // NaN != NaN
                return NAN;
        }else{
            old = *string;
            token(string); 
            result = strtod(old, NULL);
        }
        // now we have a result of the first expression/operand
    }
    char current_op, next_op;
    double next_value;
    current_op = **string;
    token(string);
    // expect operator 
    
    while(**string){
        if((!current_op && !expect_parens) || (current_op == ')' && expect_parens))
            return result;
        else if(!is_op(current_op))
            return NAN;
        // check if next char is a parens
        if(**string == '('){
            (*string)++;
            next_value = evaluate(NAN, string, 1, 0);
            if(is_nan(next_value)) // NaN != NaN
                return NAN;
        }else{
            old = *string;
            token(string);
            next_value = strtod(old, NULL);
        }
        // expect the next_op
        next_op = **string;
        switch(next_op){
            case '+': case '-': case '*': case '/': case ')':
                break;
            case 0:
                if(expect_parens)
                    return NAN;
                break;
            default:
                return NAN;
        }
        int diff_precedence = precedence(next_op) - precedence(current_op);
        if(diff_precedence > 0){
            // pointer right now points to next operand
            // we either need to backtrack the pointer to point to next_value
            // or pass in the next value into evaluate (which is easier)
            result = eval(current_op, result, evaluate(next_value, string, 0, 1));
            current_op = **string;
            //if((!current_op && !expect_parens) || (current_op == ')' && expect_parens))
            //    return result;
            //else if(!is_op(current_op))
            //    return NAN;
            // pointer is pointing to end of expression after next_op
            //
        }else{
            result = eval(current_op, result, next_value);
            current_op = next_op;
            // pointer is pointing to next_op
        }        
        if(diff_precedence < 0 && elevated)
            return result;
        token(string);
        
    }
    return result;
}

double eval(char op, double d1, double d2){
    switch(op){
        case '+':
            return d1 + d2;
        case '-':
            return d1 - d2;
        case '*':
            return d1 * d2;
        case '/':
            return d1 / d2;
        default:
            return NAN;
    }
}

//double parens(char **string){
//    // if we encounter a null terminator
//    // before a closing parens, return NAN
//}

int precedence(char operator){
    switch(operator){
        case 0: case ')':
            return 0;
        case '+': case '-':
            return 1;
        case '*': case '/':
            return 2;
            // case for exponentiation
            // case for functions too
    }
}
