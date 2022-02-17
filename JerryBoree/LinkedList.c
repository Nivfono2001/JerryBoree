#include "LinkedList.h"

//Node definition
typedef struct Node_t {
	Element value;
	struct Node_t *next;
	struct Node_t *prev;
} Node;

//LinkedList definition
struct LinkedList_t {
	Node *headPtr;
	Node *tailPtr;
	int listLength;
	CopyFunction copyFunction;
	FreeFunction freeFunction;
	PrintFunction printFunction;
	EqualFunction equalFunction;
	EqualFunction isPartEqual;
};

//HELPER - returns status if an element exists in the list, and updates a pointer to the found element
Node* ifElementExists(LinkedList linkedList, Element element) {
	if (linkedList == NULL || element == NULL)
		return NULL;
	Node *currNode = linkedList->headPtr;
	for (int i = 1; i <= linkedList->listLength; i++) {
		if (linkedList->equalFunction(element, currNode->value) == true) { // COMPARE FUNC
			return currNode;
		}
		currNode = currNode->next;
	}
	return NULL;
}

//functions implementation
//creation of a linked list
LinkedList createLinkedList(CopyFunction copyFunction,
		FreeFunction freeFunction, PrintFunction printFunction,
		EqualFunction equalFunction, EqualFunction isPartEqual) {
	if (copyFunction == NULL || freeFunction == NULL || printFunction == NULL
			|| equalFunction == NULL || isPartEqual == NULL)
		return NULL;
	LinkedList newList = (LinkedList)malloc(sizeof(struct LinkedList_t));
	newList->headPtr = NULL;
	newList->tailPtr = NULL;
	newList->listLength = 0;
	newList->copyFunction = copyFunction;
	newList->freeFunction = freeFunction;
	newList->printFunction = printFunction;
	newList->equalFunction = equalFunction;
	newList->isPartEqual = isPartEqual;
	return newList;
}

//destroying a linked list
status destroyList(Element linkedList) {
	if (linkedList == NULL)
		return failure;
	LinkedList list = (LinkedList)linkedList;
	int i,len=list->listLength;
	for (i = 1; i <= len; i++) {
		deleteNode(list, list->headPtr->value);	//we assume that the element is deleted inside this func
	}
	free(list);
	return success;
}

//add new element to end of the list
status appendNode(LinkedList linkedList, Element element) {
	if (linkedList == NULL || element == NULL)
		return failure;
	Node *nodePtr = (Node*) malloc(sizeof(Node)); //will contain the element we want to add
	if (nodePtr == NULL) {
		printf("Memory Problem\n");
		return failure;
	}

	//initialization of the new node fields
	nodePtr->prev = linkedList->tailPtr;
	nodePtr->value = linkedList->copyFunction(element);			//deep copy
	nodePtr->next = NULL;

	//update the lists fields
	if (linkedList->listLength == 0) { //update the head only if its the first element in list
		linkedList->headPtr = nodePtr;
		linkedList->tailPtr = nodePtr;
	} else {
		linkedList->tailPtr->next = nodePtr;
		linkedList->tailPtr = nodePtr;
	}
	linkedList->listLength++;
	return success;
}

//removes an element from list
status deleteNode(LinkedList linkedList, Element element) {
	if (linkedList == NULL || element == NULL)
		return failure;
	Node *toDeleteElement = ifElementExists(linkedList, element);//pointer to the element we want to delete // a hidden malloc!
	if (toDeleteElement == NULL)	//if the element doesn't exist in list
		return failure;

	//if its the only node to be deleted
	if (linkedList->listLength == 1) {
		linkedList->freeFunction(toDeleteElement->value);	//when implement the free func
		linkedList->headPtr = NULL;			//dont free recursivly the pointers in value- just put NULL there
		linkedList->tailPtr = NULL;
		free(toDeleteElement);
		linkedList->listLength-=1;
		return success;
	}
	//if its the first node to be deleted
	if (toDeleteElement->prev == NULL) {
		linkedList->headPtr = toDeleteElement->next;
		toDeleteElement->next->prev = NULL;
		linkedList->freeFunction(toDeleteElement->value);
		free(toDeleteElement);
		linkedList->listLength-=1;
		return success;
	}
	//if its the last node to be deleted
	if (toDeleteElement->next == NULL) {
		linkedList->tailPtr = toDeleteElement->prev;
		toDeleteElement->prev->next = NULL;
		linkedList->freeFunction(toDeleteElement->value);
		free(toDeleteElement);
		linkedList->listLength-=1;
		return success;
	}

	//the node to be deleted is in middle of list.
	toDeleteElement->prev->next = toDeleteElement->next;//update the neighbors to point to each other
	toDeleteElement->next->prev = toDeleteElement->prev;
	linkedList->freeFunction(toDeleteElement->value);
	free(toDeleteElement);
	linkedList->listLength-=1;
	return success;
}

//prints the list from head
status displayList(Element linkedList) {
	if (linkedList == NULL)
		return failure;
	LinkedList list = (LinkedList)linkedList;
	Node *printNode = list->headPtr;
	int i;
	for (i = 1; i <= list->listLength; i++) {
		list->printFunction(printNode->value);
		printNode = printNode->next;
	}
	return success;
}

//returns value of element in given index in list
Element getDataByIndex(LinkedList linkedList, int index) {
	if (linkedList == NULL)
		return NULL;
	if (index > linkedList->listLength) //illegal index
		return NULL;

	Node *foundElement = linkedList->headPtr;
	int i;
	for (i = 1; i < index; i++) {
		foundElement = foundElement->next;
	}
	return foundElement->value;//mabey we need copy function here
}

//returns the length of the list
int getLengthList(LinkedList linkedList) {
	if (linkedList == NULL)
		return failure; // failure == 1
	return linkedList->listLength;
}

//returns the val of node if it contains the given element (element is a part of nodes value)
Element searchByKeyInList(LinkedList linkedList, Element element) {
	if (linkedList == NULL || element == NULL)
		return NULL;
	int i;
	Node *foundElement = linkedList->headPtr;
	for (i = 0; i < linkedList->listLength; i++) {
		if (linkedList->isPartEqual(foundElement->value, element))//this func gets two different types of params
			return foundElement->value;
		foundElement = foundElement->next;
	}
	return NULL;
}

Element copyList(Element linkedList){
	if(linkedList == NULL)
		return NULL;
	LinkedList originaList = (LinkedList)linkedList;
	LinkedList newList = createLinkedList(originaList->copyFunction, originaList->freeFunction,
			originaList->printFunction, originaList->equalFunction, originaList->isPartEqual);
	if(newList == NULL)
		return NULL;
	return newList;

}

