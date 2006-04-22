#ifndef DEFINITION_H
#define DEFINITION_H

#include <iostream>

using namespace std;

class Definition
{
public:
    virtual void emitCode(ostream& out) = 0;
    virtual void emitIncludes(ostream& out) = 0;
    virtual bool fullyDefined() = 0; 
};

#endif // DEFINITION_H
