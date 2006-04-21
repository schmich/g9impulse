#ifndef ENEMY_H
#define ENEMY_H

#include <map>
#include <string>

using namespace std;

class Health;
class Animation;
class Weapon;
class Behavior;

class Enemy
{
public:
    Enemy()
        : name(""), health(NULL), animation(NULL), weapon(NULL)
        { }

    bool fullyDefined()
        {
            return (name != "")
                && (health != NULL)
                && (animation != NULL)
                && (weapon != NULL)
                && (behaviors.size() != 0);
        }

    string      name;
    Health*     health;
    Animation*  animation;
    Weapon*     weapon;

    vector<Behavior*> behaviors;
};

#endif // ENEMY_H
