#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "Defs.h"

//initialization of the pointer to LinkedList struct
typedef struct LinkedList_t *LinkedList;

//function declaration
LinkedList createLinkedList(CopyFunction copyFunction,
		FreeFunction freeFunction, PrintFunction printFunction,
		EqualFunction equalFunction, EqualFunction isPartEqual);
status destroyList(Element linkedList);
status appendNode(LinkedList linkedList, Element element);
status deleteNode(LinkedList linkedList, Element element);
status displayList(Element linkedList);
Element getDataByIndex(LinkedList linkedList, int index);
int getLengthList(LinkedList linkedList);
Element searchByKeyInList(LinkedList linkedList, Element element);
Element copyList(Element linkedList);


#endif /* LINKED_LIST_H */
