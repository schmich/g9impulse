#include "text.h"
#include "score.anim.inl"
#include "input.h"
#include "storage.h"
#include "list.h"
#include "gpu.h"
#include "delay.h"
#include "high-score.h"

typedef struct ScoreEntry
{
    char   initials[4];
    uint16 score;
} ScoreEntry;

static void destroyScores(List* scores)
{
    Node* curr;

    foreach (curr, scores)
        free(curr->data);

    destroy(scores);
}

static void saveHighScores(List* highScores)
{
    uint8 pos = 0;

    Node* curr;
    ScoreEntry* entry;

    foreach (curr, highScores)
    {
        entry = (ScoreEntry*)curr->data;

        writeStorage(entry->initials, 3, pos);
        writeStorage((char*)&entry->score, 2, pos + 3);
        
        pos += 5;
    }
}

static List* loadHighScores(void)
{
    uint8 i;
    uint8 pos;
    List* scores = createList();
    ScoreEntry* entry;

    for (i = 0, pos = 0; i < 10; ++i, pos += 5)
    {
        entry = new(ScoreEntry); 

        readStorage(entry->initials, 3, pos);
        entry->initials[3] = '\0';

        readStorage((char*)&entry->score, 2, pos + 3);

        appendElement(scores, entry);
    }

    return scores;
}

static bool displayHighScores(List* highScores, Node* scorePos)
{
    uint8 y;

    uint8 buffer;
    Animation* background = highScoreBackgroundAnimation();

    char _[] = "_";
    uint8 pos = 0;
    char* initial;

    Node* curr;
    ScoreEntry* entry;
    Input* event = getInputEvent();

    uint16 delay = 0;

    setDoubleBuffer(true);
    while (!event->startPressed)
    {
        event = getInputEvent();
        if (scorePos != NULL)
        {
            initial = ((ScoreEntry*)scorePos->data)->initials + pos;

            if (event->leftPressed)
            {
                if (pos == 0)
                    pos = 2;
                else
                    --pos;
            }
            else if (event->rightPressed)
            {
                if (++pos > 2)
                    pos = 0;
            }
            else if (event->upPressed)
            {
                if (*initial == 'A')
                    *initial = ' ';
                else if (*initial == ' ')
                    *initial = 'Z';
                else
                    --*initial;
            }
            else if (event->downPressed)
            {
                if (*initial == 'Z')
                    *initial = ' ';
                else if (*initial == ' ')
                    *initial = 'A';
                else
                    ++*initial;
            }
            else if (event->buttonBPressed)
            {
                *initial = 'I';
            }
            else if (event->buttonAPressed)
            {
                *initial = 'R';
            }
        }
        else
        {
            delay_ms(10);
            delay += 10;
            if (delay >= 15000)
                return true;
        }

        drawImage(&background->images[0], makePoint(0, 0), true);

        y = 70;
        foreach (curr, highScores)
        {
            entry = (ScoreEntry*)curr->data;
            drawText(entry->initials, makePoint(85, y));
            drawNumber(entry->score, makePoint(115, y));

            if (scorePos == curr)
                drawText(_, makePoint(85 + pos * textWidth(_), y - 1));

            y += 16;
        }

        flipBuffer(&buffer);
    }

    return false;
}

static void setDefaultHighScores(void)
{
    char inits[] = "LH _CPS_ZC _JPC_EMS_SYB_ECE_ABC_XYZ_DEF";

    uint16 scores[] = {100, 99, 98, 97, 96,
                       95,  94, 93, 92, 91};

    uint8 i;
    char* currInit;
    List* highScores = createList();
    ScoreEntry* entry;

    for (i = 0; i < 10; ++i)
    {
        entry = new(ScoreEntry);

        entry->initials[0] = inits[i * 4];
        entry->initials[1] = inits[i * 4 + 1];
        entry->initials[2] = inits[i * 4 + 2];
        entry->initials[3] = '\0';
        entry->score = scores[i];

        appendElement(highScores, entry);
    }

    saveHighScores(highScores);
    destroyScores(highScores);
}

static Node* highScorePosition(uint16 score, List* highScores)
{
    Node* curr;
    ScoreEntry* entry;

    foreach (curr, highScores)
    {
        entry = (ScoreEntry*)curr->data;

        if (score > entry->score)
            return curr;
    }

    return NULL;
}

bool showHighScores()
{
    List* highScores = loadHighScores();
    bool startPressed = displayHighScores(highScores, NULL);
    destroyScores(highScores);

    return startPressed;
}

void checkHighScore(uint16 score)
{
    char initials[3];
    List* highScores;
    Node* scorePos;
    Node* newScore;
    ScoreEntry* entry;

    highScores = loadHighScores();

    scorePos = highScorePosition(score, highScores);
    if (scorePos != NULL)
    {
        entry = new(ScoreEntry);
        entry->initials[0] = entry->initials[1] = entry->initials[2] = 'A';
        entry->initials[3] = '\0';
        entry->score = score;

        newScore = insertElement(highScores, scorePos, entry);
        removeNode(highScores, highScores->tail);

        displayHighScores(highScores, newScore);
        saveHighScores(highScores);
    }

    destroyScores(highScores);
}
