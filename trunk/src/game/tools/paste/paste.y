%{

#include <iostream>
#include <string>
#include <vector>
#include "enemy.h"

using namespace std;

int yylex(void);
int yyerror(const char* error);
void processDefinitions(vector<Enemy*> defs);

extern int LineNumber;

#define D(x) { cout << #x << endl; }

%}

/* Here we define the union, used to store different types of */
/* data in the same variable name.                            */

%union {
    Enemy* enemy;
    Properties* properties;
    std::string* identifier;
    std::pair<std::string, Property*>* property;
    std::vector<Enemy*>* definitions;
}

/* We list the tokens used in bison, but we also indicate     */
/* what type of data they retain (which part of the union).   */
/* Notice that the non-terminals are also typed if they are   */
/* involved with the calculations.                            */

%token ENEMY
%token PROJ SPRITE
%token ANIMATION ANIMATION_DEF
%token ANIMATE ANIMATE_DEF
%token MOTION MOTION_DEF
%token HEALTH HEALTH_DEF
%token FIRING FIRING_DEF
%token WEAPON WEAPON_DEF
%token DEATH DEATH_DEF
%token<identifier> IDENT
%token NL
%type<properties> ENEMY_PROPS
%type<property>   ENEMY_PROP
%type<enemy> DEF
%type<definitions> DEFS
%start TOP

//%token <ival> NUM
//%token <op> ADDOP MULTOP EOL OPEN CLOSE
//%type <ival> E T F
//%start L

%%

TOP
    : DEFS
        { processDefinitions(*$1); }
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
            $$->properties = *$4;
        }
    | PROJ IDENT NL PROJ_PROPS { $$ = new Enemy; }
    | SPRITE IDENT NL SPRITE_PROPS { $$ = new Enemy; }
    ;

ENEMY_PROPS
    : ENEMY_PROP NL ENEMY_PROPS
        {
            (*$3)[$1->first] = $1->second;
            $$ = $3;
        }
    | /* empty */
        { $$ = new Properties; }
    ;

ENEMY_PROP
    : ANIMATION ':' ANIMATION_DEF
        { $$ = new pair<string, Property*>("animation", new Property); }
    | ANIMATE ':' ANIMATE_DEF
        { $$ = new pair<string, Property*>("animate", new Property); }
    | MOTION ':' MOTION_DEF
        { $$ = new pair<string, Property*>("motion", new Property); }
    | HEALTH ':' HEALTH_DEF
        { $$ = new pair<string, Property*>("health", new Property); }
    | FIRING ':' FIRING_DEF
        { $$ = new pair<string, Property*>("firing", new Property); }
    | WEAPON ':' WEAPON_DEF
        { $$ = new pair<string, Property*>("weapon", new Property); }
    | DEATH ':' DEATH_DEF
        { $$ = new pair<string, Property*>("death", new Property); }
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

//L :	E {printf(" result = %d\n", $1);} EOL L
	//| /* empty */
//;

/* Notice that we can write normal C code in the actions and  */
/* so we have used the character type associated with ADDOP   */
/* to determine if we add or subtract.                        */

//E :	E ADDOP T	{if ($2 == '+') $$ = $1 + $3; else $$ = $1 - $3; }
	//| T		{$$ = $1; }
//;

//T :	T MULTOP F	{$$ = $1 * $3; }
	//| F		{$$ = $1; }
//;

//F :	NUM		{$$ = $1; }
	//| '(' E CLOSE	{$$ = $2; }	/* Notice that E is item 2 */
//;

%%

extern "C" int yywrap(void)
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

void processDefinitions(vector<Enemy*> defs)
{
    for (int i = 0; i < defs.size(); ++i)
        cout << defs[i]->name << endl;
}

int main()
{
    return yyparse();
}
