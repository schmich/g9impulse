%{
#include <stdlib.h>
%}

SPACE		[ \t\n]
DIGIT		[0-9]
IDENT       [a-zA-Z_][a-zA-Z0-9_]+

%%

"enemy"         { return ENEMY; }
{IDENT}         { return IDENT; }

{DIGIT}+	{yylval.ival = atoi(yytext); return NUM;}
"+"|"-"		{yylval.op = yytext[0]; return (ADDOP);}
"*"		    {yylval.op = yytext[0]; return (MULTOP);}
";"		    {yylval.op = yytext[0]; return (EOL);}
"("		    {return ('(');}
")"		    {yylval.op = yytext[0]; return (CLOSE);}
{SPACE}+	{}

%%
