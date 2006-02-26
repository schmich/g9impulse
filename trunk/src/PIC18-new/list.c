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
}

void appendElement(List* list, void* data)
{
    Node* newNode = new(Node);
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL)
        list->head = newNode;
    else
        list->tail->next = newNode;

    list->tail = newNode;
}

Node* removeNode(List* list, Node* node)
{
    Node* prev;
    Node* next = node->next;

    if (node == list->head)
    {
        if (node == list->tail)
            list->head = list->tail = NULL;
        else
            list->head = node->next;
    }
    else
    {
        for (prev = list->head; prev->next != node; prev = prev->next)
        {
            // do nothing, just scan
        }

        if (node == list->tail)
            list->tail = prev;

        prev->next = node->next;
    }

    free(node);

    return next;
}
