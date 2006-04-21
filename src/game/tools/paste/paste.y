%{

#include <iostream>
#include <string>
#include <vector>
#include "enemy.h"
#include "generator.h"
#include "properties.h"

using namespace std;

int yylex(void);
int yyerror(const char* error);

extern int LineNumber;

#define D(x) { cout << #x << endl; }

%}

/* Here we define the union, used to store different types of */
/* data in the same variable name.                            */

%union {
    int     intVal;
    string* strVal;
    pair<int, int>* fracVal;

    Enemy* enemy;
    Property* property;
    vector<Property*>* properties;
    vector<Enemy*>* definitions;
}

/* We list the tokens used in bison, but we also indicate     */
/* what type of data they retain (which part of the union).   */
/* Notice that the non-terminals are also typed if they are   */
/* involved with the calculations.                            */

%token ENEMY
%token PROJ SPRITE
%token ANIMATION
%token<strVal> ANIMATION_FILE ANIMATION_IDENT
%token ANIMATE ANIMATE_DEF
%token MOTION MOTION_ROLL MOTION_CHASE MOTION_BORING MOTION_TARGET
%token HEALTH
%token FIRING FIRING_DEF
%token WEAPON
%token<strVal> WEAPON_NAME
%token DEATH DEATH_DEF
%token<strVal> IDENT
%token NL
%token<intVal> INT
%type<properties> ENEMY_PROPS
%type<property>   ENEMY_PROP
%type<enemy> DEF
%type<definitions> DEFS
%type<fracVal> SPEED
%start TOP

//%token <ival> NUM
//%token <op> ADDOP MULTOP EOL OPEN CLOSE
//%type <ival> E T F
//%start L

%%

TOP
    : DEFS
        { generateCode(*$1); }
    ;

DEFS
    : DEF OPT_NL DEFS
        {
            $3->push_back($1);
            $$ = $3;
        }
    | /* empty */
        { $$ = new vector<Enemy*>; }
    ;

DEF 
    : ENEMY IDENT NL ENEMY_PROPS
        {
            $$ = new Enemy;
            $$->name = *$2;

            for (int i = 0; i < $4->size(); ++i)
            {
                (*$4)[i]->attach($$);
            }
        }
    | PROJ IDENT NL PROJ_PROPS { $$ = new Enemy; }
    | SPRITE IDENT NL SPRITE_PROPS { $$ = new Enemy; }
    ;

ENEMY_PROPS
    : ENEMY_PROP NL ENEMY_PROPS
        {
            $3->push_back($1);
            $$ = $3;
        }
    | /* empty */
        { $$ = new std::vector<Property*>; }
    ;

ENEMY_PROP
    : ANIMATION ':' ANIMATION_FILE '/' ANIMATION_IDENT
        { $$ = new Animation(*$3, *$5); }
    | ANIMATE ':' ANIMATE_DEF
        { $$ = new Health(55); }
    | MOTION ':' MOTION_BORING SPEED
        { $$ = new BoringBehavior($4->first, $4->second); }
    | MOTION ':' MOTION_CHASE SPEED
        { $$ = new ChaseBehavior($4->first, $4->second); }
    | MOTION ':' MOTION_ROLL
        { $$ = new RollBehavior; }
    | HEALTH ':' INT
        { $$ = new Health($3); }
    | FIRING ':' FIRING_DEF
        { $$ = new Health(55); }
    | WEAPON ':' WEAPON_NAME
        { $$ = new Weapon(*$3); }
    | DEATH ':' DEATH_DEF
        { $$ = new Health(55); }
    ;

SPEED
    : INT
        { $$ = new pair<int, int>($1, 1); }
    | INT '/' INT
        { $$ = new pair<int, int>($1, $3); }
    ;

PROJ_PROPS
    : PROJ_PROP NL PROJ_PROPS
    | /* empty */
    ;

PROJ_PROP
    : '/'
    ;

SPRITE_PROPS
    : SPRITE_PROP NL SPRITE_PROPS
    | /* empty */
    ;

SPRITE_PROP
    : '/'
    ;

OPT_NL
    : NL OPT_NL
    | /* empty */
    ;

%%

extern "C"
int yywrap(void)
{
    return 1;
}

int yyerror(const char* error)
{
    cout << "Parse error on "
         << LineNumber << ": "
         << error << endl;

    return 0;
}

int main()
{
    return yyparse();
}
