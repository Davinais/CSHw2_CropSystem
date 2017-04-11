#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdlib.h>
#include <stdbool.h>

typedef enum SortMode
{
    ID_M, TYPE_M, PRICE_M, DAY_M, YIELD_M
}SortMode;

typedef enum SortOrder
{
    INCREASING, DECREASING
}SortOrder;

typedef struct Node
{
    int id;
    char type;
    int price, day, yield;
    struct Node* next;
}Node;
typedef struct List
{
    size_t size;
    SortMode mode;
    SortOrder order;
    struct Node* head;
}List;

bool searchID(List*, int);
bool compareNode(SortMode, SortOrder, Node*, Node*);
void insertNode(List*, Node*);
void appendNode(List*, Node*);
void deleteByID(List*, int);
void cleanList(List*);
Node* getTail(List*);
void reverseList(List*);
void quickSort(List*);
void reassembleList(List*, SortMode, SortOrder);
void printList(List*);
#endif
