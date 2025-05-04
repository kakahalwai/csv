%{
#include<stdio.h>
#include<stdlib.h>
int lineno = 1;
int yylex();
void yyerror(const char *s);
extern FILE *yyin;
%}

%token ID NUM CONDI_OP OPERATOR FOR KEY
//Note to use single quotes only

%%
start : for_smt ;

for_smt : FOR '(' assignent ';' condition ';' assignent ')' '{' statement_list '}' 
{
    printf("FOR Statment Syntax Valid")
}

assignent: ID OPERATOR ID | ID OPERATOR | ID OPERATOR NUM |KEY ID OPERATOR ID |KEY ID OPERATOR NUM


condition : ID CONDI_OP ID | ID CONDI_OP NUM | NUM CONDI_OP ID 

statement_list : | statement_list statement

statement : ID OPERATOR expression ';' | NUM OPERATOR expression ';' | expression ';' 

expression : ID | NUM | expression OPERATOR expression ;
%%


void yyerror(const char *s){
    fprintf(stderr, "Syntax error at line %d: %s\n", lineno, s);
    exit(1);}

int main (int argc, char **argv){
    if (argc >1 ){
        yyin = fopen(argv[1],"r");
    }
    return yyparse();

}