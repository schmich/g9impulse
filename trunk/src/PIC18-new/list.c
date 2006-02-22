#include "list.h"

static void destroyList(List* list)
{
    Node* curr = list->head;
    Node* next;

    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
}

List* createList(void)
{
    List* list = new(List);
    list->destroy = destroyList;
    list->head = list->tail = NULL;
    list->size = 0;
}

void appendElement(List* list, void* data)
{
    Node* newNode = new(Node);
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = list->tail;

    if (!list->head)
        list->head = newNode;
    else
        list->tail->next = newNode;

    list->tail = newNode;
    list->size++;
}

void removeNode(List* list, Node* node)
{
    if (node == list->head)
    {
        list->head = node->next;
        if (list->head)
            list->head->prev = NULL;
    }

    if (node == list->tail)
    {
        list->tail = list->tail->prev;
        if (list->tail)
            list->tail->next = NULL;
    }

    if (node->next)
        node->next->prev = node->prev;

    if (node->prev)
        node->prev->next = node->next;

    free(node);
    list->size--;
}
