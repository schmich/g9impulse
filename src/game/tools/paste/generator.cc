#include <iostream>
#include "generator.h"
#include "properties.h"

void generateCode(vector<Definition*> defs)
{
    for (int i = 0; i < defs.size(); ++i)
        defs[i]->emitIncludes(cout);

    cout << "\n";

    for (int i = 0; i < defs.size(); ++i)
        defs[i]->emitCode(cout);
}
