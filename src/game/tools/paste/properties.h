#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include "enemy.h"
#include "util.h"

using namespace std;

class Property
{
public:
    virtual void attach(Enemy* e) = 0;
};

class Health : public Property
{
public:
    Health(int _amount)
        : amount(_amount)
        { }

    virtual void attach(Enemy* e)
        { e->health = this; }

    int amount;
};

class Animation : public Property
{
public:
    Animation(string _file, string _ident)
        : file(_file), ident(_ident)
        { }

    virtual void attach(Enemy* e)
        { e->animation = this; }

    string file;
    string ident;
};

class Weapon : public Property
{
public:
    Weapon(string _name)
        : name(_name)
        { }

    virtual void attach(Enemy* e)
        { e->weapon = this; }

    string name;
};

class Behavior : public Property
{
public:
    virtual void attach(Enemy* e)
        { e->behaviors.push_back(this); }

    virtual string code() = 0;
};

class BoringBehavior : public Behavior
{
public:
    BoringBehavior(int _speed, int _frameDelay)
        : speed(_speed), frameDelay(_frameDelay)
        { }

    virtual string code()
        { return "createBoring("
               + toString(speed)
               + ", "
               + toString(frameDelay)
               + ")"; }

    int speed;
    int frameDelay;
};

class ChaseBehavior : public Behavior
{
public:
    ChaseBehavior(int _speed, int _frameDelay)
        : speed(_speed), frameDelay(_frameDelay)
        { }

    virtual string code()
        { return "createChase(" 
               + toString(speed) 
               + ", "
               + toString(frameDelay)
               + ")"; }

    int speed;
    int frameDelay;
};

class RollBehavior : public Behavior
{
public:
    virtual string code()
        { return "createRoll(x)"; }
};

#endif // PROPERTIES_H
