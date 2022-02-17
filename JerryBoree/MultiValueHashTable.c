#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "KeyValuePair.h"
#include "LinkedList.h"

struct MultiValueHashTable_t{
	hashTable myHashTable;
	CopyFunction copyKey;
	FreeFunction freeKey;
	PrintFunction printKey;
	CopyFunction copyValue;
	FreeFunction freeValue;
	PrintFunction printValue;
	EqualFunction equalKey;
	TransformIntoNumberFunction transformIntoNumber;
	EqualFunction equalValue;
	EqualFunction isPartOfValEq;
	int size;
};

multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey,
		PrintFunction printKey, CopyFunction copyValue,
		FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
		TransformIntoNumberFunction transformIntoNumber, int hashNumber, EqualFunction equalValue, EqualFunction isPartOfValEq){
	if (copyKey == NULL || freeKey == NULL || freeKey == NULL
				|| copyValue == NULL || freeValue == NULL || printValue == NULL
				|| equalKey == NULL || transformIntoNumber == NULL)
			return NULL;
	multiValueHashTable newMultiHash=(multiValueHashTable)malloc(sizeof(struct MultiValueHashTable_t));
	newMultiHash->myHashTable = createHashTable(copyKey, freeKey, printKey, copyList, destroyList, displayList, equalKey, transformIntoNumber, hashNumber);
	newMultiHash->size=hashNumber;
	newMultiHash->copyKey = copyKey;
	newMultiHash->freeKey = freeKey;
	newMultiHash->printKey = printKey;
	newMultiHash->copyValue = copyValue;
	newMultiHash->freeValue = freeValue;
	newMultiHash->printValue = printValue;
	newMultiHash->equalKey = equalKey;
	newMultiHash->transformIntoNumber = transformIntoNumber;
	newMultiHash->equalValue = equalValue;
	newMultiHash->isPartOfValEq = isPartOfValEq;
	return newMultiHash;
}



status destroyMultiValueHashTable(multiValueHashTable multiHashTable){
	if(multiHashTable == NULL)
		return failure;
	//NOT SO GOOD - WE NEED TO DESTROY THE LINKED LISTS IN EACH PAIR
	return destroyHashTable(multiHashTable->myHashTable);
}

//TO DO:  add test - is the new value already exists in the values list??
status addToMultiValueHashTable(multiValueHashTable multiHashTable, Element key,Element value){
	if (multiHashTable == NULL || key == NULL || value == NULL)
		return failure;
	//KeyValuePair newPair = createKeyValuePair(key, value, multiHashTable->copyKey, copyList, //hidden malloc!!
		//	multiHashTable->freeKey, destroyList, multiHashTable->printKey, displayList,
			//multiHashTable->equalKey);

	//check if key exists in table
	KeyValuePair foundPair = lookupInHashTable(multiHashTable->myHashTable, key);

	//if the key doesnt exists in the table yet.
	if(foundPair == NULL){

		//first create new linked list that will contain values for key
		LinkedList valuesList = createLinkedList(multiHashTable->copyValue, multiHashTable->freeValue,
				multiHashTable->printValue, multiHashTable->equalValue, multiHashTable->isPartOfValEq);

		//add the new value to the values list
		status res = appendNode(valuesList, value);
		if(  res == success){

			//add the new pair ([ key | valuesList ]) to hash table.
			addToHashTable(multiHashTable->myHashTable, key, valuesList);
		}
	}
	//if key already exists - we add to the pair's value list the new value
	return appendNode(getValue(foundPair), value);
}

//finds all the values that connected to the given key - first look for the key in the hash table,
//than return it's value.
LinkedList lookupInMultiValueHashTable(multiValueHashTable multiHashTable, Element key){
	if(multiHashTable == NULL || key == NULL)
		return NULL;
	KeyValuePair foundPair = lookupInHashTable(multiHashTable->myHashTable, key);
	if(foundPair == NULL)
		return NULL;
	return(getValue(foundPair));
	return NULL;
}

//removes an element from the multi hash table
status removeFromMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element value){
	if(multiHashTable == NULL || key == NULL || value == NULL)
		return failure;
	KeyValuePair foundPair = lookupInHashTable(multiHashTable->myHashTable, key);
	if(foundPair == NULL)
		return failure;
	LinkedList valuesList = getValue(foundPair);
	Element foundValue = searchByKeyInList(valuesList, value);
	if(foundValue == NULL)
		return failure;
	if(deleteNode(valuesList, foundValue) == failure)
		return failure;
	if(getLengthList(valuesList) == 0)
		return(removeFromHashTable(multiHashTable->myHashTable, key));
	return success;
}

//prints the values of specific key - first finds the key in hash table, then print the values list
status displayMultiValueHashElementsByKey(multiValueHashTable multiHashTable, Element key){
	if(multiHashTable == NULL || key == NULL)
		return failure;
	KeyValuePair foundPair = lookupInHashTable(multiHashTable->myHashTable, key);
	if(foundPair == NULL)
		return failure;
	displayKey(foundPair);
	displayValue(foundPair);
	return success;
}








