#include <iostream>
#include "generator.h"
#include "properties.h"

void generateCode(vector<Enemy*> defs)
{
    string includes[] =
    {
        "cloud.h",
        "boring.h",
        "hover.h",
        "direct.h",
        "enemy.h",
        "chain.h",
        "chase.h",
        "facing.h",
        "animator.h",
        "spawner.h",
        "roll.h",
        "random-shoot.h",
        "lock-on.h",
        "projectile.h",
        "explosion.h",
        "powerup.h",
        "player.h",
        "world.h"
    };

    int includeSize = sizeof(includes) / sizeof(includes[0]);

    /*for (int i = 0; i < includeSize; ++i)
    {
        cout << "#include \"" + includes[i] << "\"\n";
    }

    cout << "\n";*/

    vector<string> animationFiles;
    for (int i = 0; i < defs.size(); ++i)
    {
        Enemy* e = defs[i];

        if (e->animation)
        {
            cout << "#include \""
                 << e->animation->file
                 << ".inl\"\n";
        }
    }

    cout << "\n";

    for (int i = 0; i < defs.size(); ++i)
    {
        Enemy* e = defs[i];
        
        cout << "static void spawn_" << e->name
             << "(World* w, int16 x)\n"
             << "{\n"
             << "\tEnemy* e;\n";

        if (e->behaviors.size() > 1)
            cout << "\tBehavior** bs;\n";

        cout << "\n";

        if (e->behaviors.size() > 1)
        {
            cout << "\tbs = newArray(Behavior*, "
                 << e->behaviors.size() << ");\n";

            for (int i = 0; i < e->behaviors.size(); ++i)
            {
                cout << "\tbs[" << i << "] = "
                     << e->behaviors[i]->code() << ";\n";
            }

            cout << "\n";
        }

        cout << "\te = createEnemy(\n"
             << "\t\t" << e->animation->ident << "Animation(),\n"
             << "\t\t" << "0,\n";

        if (e->behaviors.size() == 1)
        {
            cout << "\t\t" << e->behaviors[0]->code() << ",\n";
        }
        else
        {
            cout << "\t\tcreateChainBehavior(bs, "
                 << e->behaviors.size() << "),\n";
        }

        cout << "\t\t" << e->health->amount << ",\n"
             << "\t\t" << "makePoint(x, -(int16)spriteHeight(e)),\n"
             << "\t\t" << "spawn_" << e->weapon->name << ",\n"
             << "\t\t" << "DEATH\n"
             << "\t);\n"
             << "\n\taddEnemy(w, e);\n"
             << "}\n\n";
    }
}
