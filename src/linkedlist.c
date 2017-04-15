#include "linkedlist.h"
#include "stringutil.h"
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
bool isLarger(int a, int b)
{
    return (a>b);
}
bool isNoLargerThan(int a, int b)
{
    return (a<=b);
}
bool isSmaller(int a, int b)
{
    return (a<b);
}

int modeData(SortMode mode, Node* node)
{
    switch(mode)
    {
        case ID_M:
            return node->id;
        case TYPE_M:
            return node->type;
        case PRICE_M:
            return node->price;
        case DAY_M:
            return node->day;
        case YIELD_M:
            return node->yield;
    }
}

bool compareNode(SortMode mode, SortOrder order, Node* a, Node* b)
{
    bool (*compare)(int, int);
    if(order==INCREASING)
        compare = &isNoSmallerThan;
    else
        compare = &isNoLargerThan;
    return compare(modeData(mode, a), modeData(mode, b));
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
        list->size++;
    }
}

void appendNode(List* list, Node* node)
{
    if(list->head == NULL)
    {
        list->head = node;
        list->size++;
        return;
    }
    Node* current = list->head;
    while(current->next != NULL)
        current = current->next;
    current->next = node;
    list->size++;
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
        list->size--;
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
        list->size = 0;
        list->head = NULL;
    }
}

void reverseList(List* list)
{
    if(list->head == NULL)
        return;
    Node* prev = NULL;
    Node* current = list->head;
    Node* next = list->head->next;
    while(next != NULL)
    {
        current->next = prev;
        prev = current;
        current = next;
        next = next->next;
    }
    current->next = prev;
    list->head = current;
}

Node* getTail(List* list)
{
    if(list->head == NULL)
        return list->head;
    Node* current = list->head;
    while(current->next != NULL)
        current = current->next;
    return current;
}

Node* partition(Node* head, Node* end, Node **newHead, Node **newEnd, SortMode mode, bool (*compare)(int, int))
{
    Node* pivot = end;
    Node *prev = NULL, *current = head, *tail = pivot;
    while(current != pivot)
    {
        if(compare(modeData(mode, current), modeData(mode, pivot)))
        {
            if(prev != NULL)
                prev->next = current->next;
            //Node* temp = current->next;
            tail->next = current; //將原本的最尾端加上目前的節點
            tail = current; //將最尾端改為目前的節點
            current = current->next; //current改為原目前的下一個節點
            tail->next = NULL;
        }
        else
        {
            //若還不存在newHead，則指定目前指到的為newHead
            if(*newHead == NULL)
                *newHead = current;
            prev = current;
            current = current->next;
        }
    }
    //若是經過一連串比較，newHead還是為空，則代表pivot是目前排序中最前面的那個
    if(*newHead == NULL)
        (*newHead) = pivot;
    (*newEnd) = tail;
    return pivot;
}

Node* quickSortmain(Node* head, Node* end, SortMode mode, bool (*compare)(int, int))
{
    if(head == NULL || head == end)
        return head;
    Node *newHead = NULL, *newEnd = NULL;
    Node* pivot = partition(head, end, &newHead, &newEnd, mode, *compare);
    if(newHead != pivot)
    {
        Node* temp = newHead;
        //切割左邊的表
        while(temp->next != pivot)
            temp = temp->next;
        temp->next = NULL;
        newHead = quickSortmain(newHead, temp, mode, *compare);
        //接回去
        temp = newHead;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = pivot;
    }
    pivot->next = quickSortmain(pivot->next, newEnd, mode, *compare);
    return newHead;
}

void quickSort(List* list)
{
    if(list->order == INCREASING)
        list->head = quickSortmain(list->head, getTail(list), list->mode, &isLarger);
    else
        list->head = quickSortmain(list->head, getTail(list), list->mode, &isSmaller);
}

void reassembleList(List* list, SortMode newmode, SortOrder neworder)
{
    if(list->mode == newmode)
    {
        if(list->order == neworder)
            return;
        else
        {
            list->order = neworder;
            reverseList(list);
        }
    }
    else
    {
        list->mode = newmode;
        list->order = neworder;
        quickSort(list);
    }
}

void printList(List* list)
{
    if(list->head == NULL)
    {
        char header[240];
        printf("%s\n", strcenter(header, "════════════════════════════════════════", 40, 80));
        printf("%s\n", strcenter(header, "本列表為空列表！", 16, 80));
        return;
    }
    else
    {
        char header[240];
        char bar[120];
        char title[5][30];
        printf("%s\n", strcenter(header, "════════════════════════════════════════", 40, 80));
        sprintf(bar, "%s%s%s%s%s", strright(title[0], "ID", 2, 4), strright(title[1], "類別", 4, 8),
            strright(title[2], "售價", 4, 8),strright(title[3], "收成天數", 8, 12),strright(title[4], "產量", 4, 8));
        printf("%s\n", strcenter(header, bar, 40, 80));
        printf("%s\n", strcenter(header, "════════════════════════════════════════", 40, 80));
        Node* current = list->head;
        do
        {
            sprintf(bar, "%4d%8c%8d%12d%8d", current->id, current->type, current->price, current->day, current->yield);
            printf("%s", strcenter(header, bar, 40, 80));
            current = current->next;
        }while(current != NULL);
    }
}
