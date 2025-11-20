#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_EXPR_LEN 256
#define STACK_CAPACITY 50
#define DIV_BY_ZERO_SENTINEL INT_MIN 

typedef enum { //a set of named integer constants to categorize our tokens.
    TOKEN_NUMBER,
    TOKEN_OPERATOR
} TokenType;

typedef struct {
//Starts a struct (a custom data type) to hold the actual token data.
    TokenType type;
    int value;
    char op;
} Token;

typedef struct {
    Token items[STACK_CAPACITY];
    int top;
} Stack;

void initStack(Stack *s) { s->top = -1; }
int isStackEmpty(Stack *s) { return s->top == -1; }
void push(Stack *s, Token item) {
    if (s->top >= STACK_CAPACITY - 1) return;
    s->items[++s->top] = item;
}
Token pop(Stack *s) {
    if (isStackEmpty(s)) return (Token){TOKEN_OPERATOR, 0, '\0'};
    return s->items[s->top--];
}
Token peek(Stack *s) {
    if (isStackEmpty(s)) return (Token){TOKEN_OPERATOR, 0, '\0'};
    return s->items[s->top];
}

int getPrecedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int applyOp(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) return DIV_BY_ZERO_SENTINEL;
        return a / b;
    }
    return 0;
}

int infixToRPN(const char *expr, Token rpn_output[], int *rpn_count) {
    Stack opStack;
    initStack(&opStack);
    *rpn_count = 0;
    
    const char *p = expr;

    while (*p != '\0') {
        if (*p == ' ') { p++; continue; }

        if (isdigit(*p)) {
            int num = 0;
            while (isdigit(*p)) {
                num = num * 10 + (*p - '0');
                p++;
            }
            rpn_output[(*rpn_count)++] = (Token){TOKEN_NUMBER, num, 0};
            continue;
        }

        if (isOperator(*p)) {
            char current_op = *p;
            
            while (!isStackEmpty(&opStack) && 
                   getPrecedence(current_op) <= getPrecedence(peek(&opStack).op)) 
            {
                rpn_output[(*rpn_count)++] = pop(&opStack);
            }
            
            push(&opStack, (Token){TOKEN_OPERATOR, 0, current_op});
            p++;
            continue;
        }

        return -1;
    }

    while (!isStackEmpty(&opStack)) {
        rpn_output[(*rpn_count)++] = pop(&opStack);
    }

    return 0;
}

int evaluateRPN(Token rpn_output[], int rpn_count, int *error_flag) {
    Stack valueStack;
    initStack(&valueStack);
    *error_flag = 0;
    
    for (int i = 0; i < rpn_count; i++) {
        Token current = rpn_output[i];

        if (current.type == TOKEN_NUMBER) {
            push(&valueStack, current);
        } else if (current.type == TOKEN_OPERATOR) {
            if (isStackEmpty(&valueStack)) { *error_flag = 1; return 0; }
            int b = pop(&valueStack).value;
            
            if (isStackEmpty(&valueStack)) { *error_flag = 1; return 0; }
            int a = pop(&valueStack).value;

            int result_val = applyOp(a, b, current.op);

            if (result_val == DIV_BY_ZERO_SENTINEL) {
                *error_flag = 2;
                return 0;
            }

            push(&valueStack, (Token){TOKEN_NUMBER, result_val, 0});
        }
    }

    if (valueStack.top != 0) {
        *error_flag = 1;
        return 0;
    }

    return pop(&valueStack).value;
}

int main() {
    char input_expr[MAX_EXPR_LEN];
    Token rpn_output[STACK_CAPACITY];
    int rpn_count = 0;
    int error_flag = 0;
    int result = 0;

    printf("Enter expression: ");
    if (fgets(input_expr, MAX_EXPR_LEN, stdin) == NULL) return 0;
    input_expr[strcspn(input_expr, "\n")] = 0; 

    if (infixToRPN(input_expr, rpn_output, &rpn_count) == -1) {
        fprintf(stderr, "Error: Invalid expression.\n");
        return 1;
    }

    result = evaluateRPN(rpn_output, rpn_count, &error_flag);

    if (error_flag == 2) {
        fprintf(stderr, "Error: Division by zero.\n");
        return 1;
    } else if (error_flag == 1) {
        fprintf(stderr, "Error: Invalid expression.\n");
        return 1;
    } else {
        printf("Output: %d\n", result);
    }

    return 0;
}