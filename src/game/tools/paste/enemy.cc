#include "enemy.h"
#include "properties.h"

Enemy::Enemy()
: name(""), health(NULL), animation(NULL), weapon(NULL)
{
}

bool Enemy::fullyDefined()
{
    return (name != "")
        && (health != NULL)
        && (animation != NULL)
        && (weapon != NULL)
        && (behaviors.size() != 0);
}

void Enemy::emitIncludes(ostream& out)
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
        out << "#include \"" + includes[i] << "\"\n";
    }

    out << "\n";*/

    vector<string> animationFiles;

    if (animation)
    {
        out << "#include \""
             << animation->file
             << ".inl\"\n";
    }
}

void Enemy::emitCode(ostream& out)
{
    out << "static void spawn_" << name
        << "(World* w, int16 x)\n"
        << "{\n"
        << "\tEnemy* e;\n";

    if (behaviors.size() > 1)
        out << "\tBehavior** bs;\n";

    out << "\n";

    if (behaviors.size() > 1)
    {
        out << "\tbs = newArray(Behavior*, "
            << behaviors.size() << ");\n";

        for (int i = 0; i < behaviors.size(); ++i)
        {
            out << "\tbs[" << i << "] = "
                << behaviors[i]->code() << ";\n";
        }

        out << "\n";
    }

    out << "\te = createEnemy(\n"
        << "\t\t" << animation->ident << "Animation(),\n"
        << "\t\t" << "0,\n";

    if (behaviors.size() == 1)
    {
        out << "\t\t" << behaviors[0]->code() << ",\n";
    }
    else
    {
        out << "\t\tcreateChainBehavior(bs, "
            << behaviors.size() << "),\n";
    }

    out << "\t\t" << health->amount << ",\n"
        << "\t\t" << "makePoint(x, -(int16)spriteHeight(e)),\n"
        << "\t\t" << "spawn_" << weapon->name << ",\n"
        << "\t\t" << "DEATH\n"
        << "\t);\n"
        << "\n\taddEnemy(w, e);\n"
        << "}\n";
}
