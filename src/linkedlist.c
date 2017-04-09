#include "linkedlist.h"
#include <stdio.h>

bool searchID(List* list, int id)
{
    Node* nextnode = list->head;
    while(nextnode != NULL)
    {
        if(nextnode->id == id)
            return true;
        nextnode = nextnode->next;
    }
    return false;
}

bool isNoSmallerThan(int a, int b)
{
    return (a>=b);
}
bool isNoLargerThan(int a, int b)
{
    return (a<=b);
}

bool compareNode(SortMode mode, SortOrder order, Node* a, Node* b)
{
    bool (*compare)(int, int);
    if(order==INCREASING)
        compare = &isNoSmallerThan;
    else
        compare = &isNoLargerThan;
    switch(mode)
    {
        case ID_M:
            return compare(a->id, b->id);
        case TYPE_M:
            return compare(a->type, b->type);
        case PRICE_M:
            return compare(a->price, b->price);
        case DAY_M:
            return compare(a->day, b->day);
        case YIELD_M:
            return compare(a->yield, b->yield);
        default:
            return false;
    }
}

void insertNode(List* list, Node* node)
{
    if(list->head == NULL)
    {
        list->head = node;
        list->size++;
    }
    else
    {
        Node* current = list->head;
        Node* prev = NULL;
        while(compareNode(list->mode, list->order, node, current) && (current->next != NULL))
        {
            prev = current;
            current = current->next;
        }
        if(prev == NULL)
        {
            if(compareNode(list->mode, list->order, node, current))
                current->next = node;
            else
            {
                node->next = list->head;
                list->head = node;
            }
        }
        else
        {
            if(compareNode(list->mode, list->order, node, current))
                current->next = node;
            else
            {
                prev->next = node;
                node->next = current;
            }
        }
    }
}

void appendNode(List* list, Node* node)
{
    if(list->head == NULL)
    {
        list->head = node;
        return;
    }
    Node* current = list->head;
    while(current->next != NULL)
        current = current->next;
    current->next = node;
}

void deleteByID(List* list, int id)
{
    if(list->head == NULL)
        printf("本列表為空！");
    else
    {
        Node* prev = NULL;
        Node* current = list->head;
        while((current->id != id) && (current->next != NULL))
        {
            prev = current;
            current = current->next;
        }
        if(current->id == id)
        {
            if(prev == NULL)
                list->head = current->next;
            else
                prev->next = current->next;
            free(current);
        }
        else
            printf("ID %d 並不存在於此列表中！", id);
    }
}

void cleanList(List* list)
{
    if(list->head == NULL)
        return;
    else
    {
        Node* prev = NULL;
        Node* current = list->head;
        while(current != NULL)
        {
            prev = current;
            current = current->next;
            free(prev);
        }
    }
}
