%{

#include <iostream>
#include <string>
#include <vector>
#include "enemy.h"

#include "paste.tab.h"

using namespace std;

int LineNumber = 1;

%}

DIGIT		[0-9]
INT         {DIGIT}+
IDENT       [a-zA-Z_][a-zA-Z0-9_\-]*
SPACE       [ \t]
CRLF        \r\n
NL          \n
FILE        [a-zA-Z0-9_][a-zA-Z0-9_\-\.]*

%x ANIMATION_CTX
%x ANIMATE_CTX
%x MOTION_CTX
%x HEALTH_CTX
%x FIRING_CTX
%x WEAPON_CTX
%x DEATH_CTX

%%

"enemy"             { return ENEMY; }
"projectile"        { return PROJ; }
"sprite"            { return SPRITE; }

"animation"         { BEGIN(ANIMATION_CTX); return ANIMATION; }
<ANIMATION_CTX>{
{FILE}"/"{IDENT}    { return ANIMATION_DEF; }
}

"animate"           { BEGIN(ANIMATE_CTX); return ANIMATE; }
<ANIMATE_CTX>{
"never"             { return ANIMATE_DEF; }
"once"              { return ANIMATE_DEF; }
"forever"           { return ANIMATE_DEF; }
}

"motion"            { BEGIN(MOTION_CTX); return MOTION; }
<MOTION_CTX>{
"boring"            { return MOTION_DEF; }
"chase"             { return MOTION_DEF; }
"directed"          { return MOTION_DEF; }
}

"health"            { BEGIN(HEALTH_CTX); return HEALTH; }
<HEALTH_CTX>{
{INT}               { return HEALTH_DEF; }
}

"firing"            { BEGIN(FIRING_CTX); return FIRING; }
<FIRING_CTX>{
"lock-on"           { return FIRING_DEF; }
"random"            { return FIRING_DEF; }
}

"weapon"            { BEGIN(WEAPON_CTX); return WEAPON; }
<WEAPON_CTX>{
{IDENT}             { return WEAPON_DEF; }
}

"death"             { BEGIN(DEATH_CTX); return DEATH; }
<DEATH_CTX>{
}

{IDENT}             { 
                        yylval.identifier = new string(yytext);
                        return IDENT;
                    }
<*>{SPACE}+         { }
<*>{CRLF}+          {
                        BEGIN(INITIAL);
                        LineNumber += (int)(strlen(yytext) / 2);
                        return NL;
                    }
<*>{NL}+            {
                        BEGIN(INITIAL);
                        LineNumber += strlen(yytext);
                        return NL;
                    }
<*>.                { return yytext[0]; }

%%

//{DIGIT}+	{yylval.ival = atoi(yytext); return NUM;}
//"+"|"-"		{yylval.op = yytext[0]; return (ADDOP);}
//"*"		    {yylval.op = yytext[0]; return (MULTOP);}
//";"		    {yylval.op = yytext[0]; return (EOL);}
//"("		    {return ('(');}
//")"		    {yylval.op = yytext[0]; return (CLOSE);}
//{SPACE}+	{}
