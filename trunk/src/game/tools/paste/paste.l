%{

#include <iostream>
#include <string>
#include <vector>
#include "enemy.h"
#include "properties.h"

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
{IDENT}             {
                        yylval.strVal = new string(yytext);
                        return ANIMATION_IDENT;
                    }
{FILE}              {
                        yylval.strVal = new string(yytext);
                        return ANIMATION_FILE;
                    }
}

"animate"           { BEGIN(ANIMATE_CTX); return ANIMATE; }
<ANIMATE_CTX>{
"never"             { return ANIMATE_DEF; }
"once"              { return ANIMATE_DEF; }
"forever"           { return ANIMATE_DEF; }
}

"motion"            { BEGIN(MOTION_CTX); return MOTION; }
<MOTION_CTX>{
"boring"            { return MOTION_BORING; }
"chase"             { return MOTION_CHASE; }
"target"            { return MOTION_TARGET; }
"roll"              { return MOTION_ROLL; }
{INT}               {
                        yylval.intVal = atoi(yytext);
                        return INT;
                    }
}

"health"            { BEGIN(HEALTH_CTX); return HEALTH; }
<HEALTH_CTX>{
{INT}               {
                        yylval.intVal = atoi(yytext);
                        return INT;
                    }
}

"firing"            { BEGIN(FIRING_CTX); return FIRING; }
<FIRING_CTX>{
"lock-on"           { return FIRING_DEF; }
"random"            { return FIRING_DEF; }
}

"weapon"            { BEGIN(WEAPON_CTX); return WEAPON; }
<WEAPON_CTX>{
{IDENT}             {
                        yylval.strVal = new string(yytext);
                        return WEAPON_NAME;
                    }
}

"death"             { BEGIN(DEATH_CTX); return DEATH; }
<DEATH_CTX>{
}

{IDENT}             {
                        yylval.strVal = new string(yytext);
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
