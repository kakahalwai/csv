%{
#include<stdlib.h>
#include<stdio.h>
extern FILE *yyin;
int line=1;
%}

%token WHILE
%token NUM ID
%token AND OR NOT
%token RELOP

%left OR
%left AND
%right NOT
%left RELOP

%%

stmt : WHILE '(' cond ')' block                     {puts("Valid while statement");}
      ;

cond : cond OR cond
     | cond AND cond
     | NOT cond
     | rel
     | '(' cond ')'
     ;

rel : expr RELOP expr;

expr: ID | NUM ;

block: '{' stmt_list '}' ;

stmt_list : /* empty */
           |stmt_list simple_statement
           ;

simple_statement : expr ';'
                  |block
                   ;

%%

void yyerror(const char *s){
    fprintf(stderr, "❌ Syntax error encountered at line %d → %s\n", line, s);
}

int main(int argc,char **argv){
    if(argc>1){
        yyin=fopen(argv[1],"r");
        if(!yyin){
            printf("Cannot open file");
        }

    }
     else {
        printf("Enter a while statement:\n");
    }

    return yyparse();
}
                 

       
