%{
    #include<stdio.h>
    #include<stdlib.h>
    extern FILE *yyin;
    int yylex(void);
    void yyerror(const char *s);
%}
%token NUM
%left '+' '-'
%left '*' '/'
%%

S : E           {printf("Answer is:\n%d",$1);}

E : E '+' E       { $$= $1 + $3 ; }
  | E '*' E       { $$= $1 * $3 ; }
  | E '-' E        { $$ = $1 - $3 ; }
  | E '/' E        { $$ = $1 / $3 ; }
  | '(' E ')'    { $$ = $2 ; }
  | NUM          { $$ = $1 ; }


%%
void yyerror(const char *s){
    printf("Syntax Error\n");
}
int main(int argc,char **argv){
    
    if(argc>1){
        FILE *fp=fopen(argv[1],"r");
        if(!fp){
            perror("Error opening file");
            return 1;
        }
        yyin=fp;
    }
    else{
        printf("\nEnter an input:");
    }
    yyparse();
    return 0;
}

