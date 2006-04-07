%{
%}

/* Here we define the union, used to store different types of */
/* data in the same variable name.                            */

%union {
  int ival;
  char op;
}

/* We list the tokens used in bison, but we also indicate     */
/* what type of data they retain (which part of the union).   */
/* Notice that the non-terminals are also typed if they are   */
/* involved with the calculations.                            */

%token <ival> NUM
%token <op> ADDOP MULTOP EOL OPEN CLOSE
%type <ival> E T F
%start L

%%

/* grammar will be very simple expressions:             */
/*    L -> E ; L | empty                                */
/*    E -> E addop T | T                                */
/*    T -> T multop F | F                               */
/*    F -> NUM | ( E )                                  */
/*                                                      */
/*    where addop is either + or -                      */
/*      and multop is *                                 */

/* bison refers to grammar elements in a production by number */
/* so $1 refers to the first item, $2 to the second, etc.    */
/* $$ refers to the left hand side item.                      */
/* It is important to keep the various types coordinated.     */

L :	E {printf(" result = %d\n", $1);} EOL L
	| /* empty */
;

/* Notice that we can write normal C code in the actions and  */
/* so we have used the character type associated with ADDOP   */
/* to determine if we add or subtract.                        */

E :	E ADDOP T	{if ($2 == '+') $$ = $1 + $3; else $$ = $1 - $3; }
	| T		{$$ = $1; }
;

T :	T MULTOP F	{$$ = $1 * $3; }
	| F		{$$ = $1; }
;

F :	NUM		{$$ = $1; }
	| '(' E CLOSE	{$$ = $2; }	/* Notice that E is item 2 */
;

%%

int yywrap(void)
{
    return 1;
}

int yyerror(const char* error)
{
    printf("Parse error: %s\n", error);
    return 0;
}

int main()
{
    return yyparse();
}

#include "paste.yy.c"

