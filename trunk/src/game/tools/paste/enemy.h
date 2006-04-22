#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <string>
#include <vector>
#include "definition.h"

using namespace std;

class Health;
class Animation;
class Weapon;
class Behavior;

class Enemy : public Definition
{
public:
    Enemy();

    virtual bool fullyDefined();
    virtual void emitCode(ostream& out);
    virtual void emitIncludes(ostream& out);

    string      name;
    Health*     health;
    Animation*  animation;
    Weapon*     weapon;

    vector<Behavior*> behaviors;
};

#endif // ENEMY_H
