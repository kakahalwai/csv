%{
    #include<stdio.h>
    #include<stdlib.h>
    extern FILE *yyin;
    int line=1;
    void yyerror(const char *s);
    int yylex(void);
%}

%token IF ELSE ID NUM RELOP

%%
stmt: IF '(' cond ')' block opt_else                   {printf("Correct Syntax of If-Else Statement");}
     ;

cond: ID RELOP NUM
    | NUM RELOP ID
    | ID RELOP ID

block : '{' stmt_list '}'
       ;

stmt_list :/* empty */
          | stmt_list simple_statement
          ;

simple_statement : ID '=' NUM ';'
                | block
                ;

opt_else:/* empty */
         | ELSE block
         ;

%%

void yyerror(const char *s){
    fprintf(stderr, " Syntax error at line %d: %s\n", line, s);
}
int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("File open failed");
            return 1;
        }
    } else {
        printf("Enter an if-else statement:\n");
    }
    return yyparse();
}
   
