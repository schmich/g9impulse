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
%x EFFECT_CTX

%x ENEMY_CTX
%x PROJ_CTX
%x ARTIFACT_CTX
%x SPRITE_CTX

%x ENEMY_DEF_CTX
%x PROJ_DEF_CTX
%x ARTIFACT_DEF_CTX
%x SPRITE_DEF_CTX

%%

"projectile"        { BEGIN(PROJ_CTX); return PROJ; }
"artifact"          { BEGIN(ARTIFACT_CTX); return ARTIFACT; }
"sprite"            { BEGIN(SPRITE_CTX); return SPRITE; }

"enemy"             { BEGIN(ENEMY_CTX); return ENEMY; }
<ENEMY_CTX>{
{IDENT}             {
                        BEGIN(ENEMY_DEF_CTX);
                        yylval.strVal = new string(yytext);
                        return IDENT;
                    }
}

<ENEMY_DEF_CTX>{
"animation"         { BEGIN(ANIMATION_CTX); return ANIMATION; }
"animate"           { BEGIN(ANIMATE_CTX); return ANIMATE; }
"motion"            { BEGIN(MOTION_CTX); return MOTION; }
"health"            { BEGIN(HEALTH_CTX); return HEALTH; }
"firing"            { BEGIN(FIRING_CTX); return FIRING; }
"weapon"            { BEGIN(WEAPON_CTX); return WEAPON; }
"death"             { BEGIN(DEATH_CTX); return DEATH; }
}

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

<ANIMATE_CTX>{
"never"             { return ANIMATE_DEF; }
"once"              { return ANIMATE_DEF; }
"forever"           { return ANIMATE_DEF; }
}

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

<HEALTH_CTX>{
{INT}               {
                        yylval.intVal = atoi(yytext);
                        return INT;
                    }
}

<FIRING_CTX>{
"lock-on"           { return FIRING_DEF; }
"random"            { return FIRING_DEF; }
}

<WEAPON_CTX>{
{IDENT}             {
                        yylval.strVal = new string(yytext);
                        return WEAPON_NAME;
                    }
}

<DEATH_CTX>{
}

"effect"            { BEGIN(EFFECT_CTX); return EFFECT; }
<EFFECT_CTX>{
"increase-health"   { return EFFECT_HEALTH; }
}

<PROJ_CTX>{
{IDENT}             {
                        BEGIN(PROJ_DEF_CTX);
                        yylval.strVal = new string(yytext);
                        return IDENT;
                    }
}

<ARTIFACT_CTX>{
{IDENT}             {
                        BEGIN(ARTIFACT_DEF_CTX);
                        yylval.strVal = new string(yytext);
                        return IDENT;
                    }
}

<SPRITE_CTX>{
{IDENT}             {
                        BEGIN(SPRITE_DEF_CTX);
                        yylval.strVal = new string(yytext);
                        return IDENT;
                    }
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
