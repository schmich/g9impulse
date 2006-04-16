#ifndef ENEMY_H
#define ENEMY_H

#include <map>
#include <string>
#include "properties.h"

using namespace std;

typedef map<string, Property*> Properties;

class Enemy
{
public:
    string     name;
    Properties properties;
};

#endif // ENEMY_H
