#ifndef LIST_H
#define LIST_H

#include "common.h"
#include "destroyable.h"

typedef struct Node
{
    void* data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct List
{
    DESTROYABLE_BASE;

    Node*  head;
    Node*  tail;
    uint16 size;
} List;

List* createList(void);
void appendElement(List* list, void* data);
void removeNode(List* list, Node* node);

#endif // LIST_H
