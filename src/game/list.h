#ifndef LIST_H
#define LIST_H

#include "common.h"
#include "destroyable.h"

#define foreach(elem, list) for ((elem) = (list)->head; (elem) != NULL; (elem) = (elem)->next)

typedef struct Node
{
    void* data;
    struct Node* next;
} Node;

typedef struct List
{
    DESTROYABLE_BASE;

    Node*  head;
    Node*  tail;
} List;

List* createList(void);
void appendElement(List* list, void* data);
Node* removeNode(List* list, Node* node);
Node* insertElement(List* list, Node* before, void* data);

#endif // LIST_H
