%{
    #include<stdio.h>
    #include<stdlib.h>
    extern FILE *yyin;



    void checkLeapYear(int year){
        if((year%4==0 && year%100!=0|| year%400==0)){
            printf("%d\n Is a leap year",year);
        }
        else{
            printf("%d\n is not a leap year",year);
        }
    }

%}
%%

[0-9]{4}                 {checkLeapYear(atoi(yytext));}
\n                        ;
.                         ;


%%
int yywrap(){
    return 1;
}
int main(int argc,char *argv[]){
    if(argc>1){
        yyin=fopen(argv[1],"r");
        if(!yyin){
            perror("Cannot open file");
        return 1;
        }
    }
    printf("---- Leap Year Checker ----\n");
    yylex();
    fclose(yyin);
    return 0;
}
