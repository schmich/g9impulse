%{
#include <stdlib.h>
%}

DELIM		[ \t\n]
WS		    {DELIM}+
DIGIT		[0-9]

%%

{DIGIT}+	{yylval.ival = atoi(yytext); return NUM;}
"+"|"-"		{yylval.op = yytext[0]; return (ADDOP);}
"*"		    {yylval.op = yytext[0]; return (MULTOP);}
";"		    {yylval.op = yytext[0]; return (EOL);}
"("		    {return ('(');}
")"		    {yylval.op = yytext[0]; return (CLOSE);}
{WS}		{}

%%
