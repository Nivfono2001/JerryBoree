#include "HashTable.h"
#include "KeyValuePair.h"
#include "LinkedList.h"
struct hashTable_s {

	LinkedList *table;
	CopyFunction copyKey;
	FreeFunction freeKey;
	PrintFunction printKey;
	CopyFunction copyValue;
	FreeFunction freeValue;
	PrintFunction printValue;
	EqualFunction equalKey;
	TransformIntoNumberFunction transformIntoNumber;
	int hashNumber;
};


//creates a hash table
hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey,
		PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue,
		PrintFunction printValue, EqualFunction equalKey,
		TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
	if (copyKey == NULL || freeKey == NULL || freeKey == NULL
			|| copyValue == NULL || freeValue == NULL || printValue == NULL
			|| equalKey == NULL || transformIntoNumber == NULL)
		return NULL;
	hashTable hashtable = (hashTable) malloc(sizeof(struct hashTable_s));//place for the struct itself
	hashtable->hashNumber = hashNumber;
	hashtable->table = (LinkedList*) malloc(
			sizeof(LinkedList) * hashtable->hashNumber);//place for the array
	int i;
	for (i = 0; i < hashtable->hashNumber; i++) {
		hashtable->table[i] = createLinkedList(copyPair, destroyKeyValuePair, displayKeyValue,
				isEqualPairs, isEqualKey);	//hidden mallocs
	}
	hashtable->copyKey = copyKey;
	hashtable->freeKey = freeKey;
	hashtable->printKey = printKey;
	hashtable->copyValue = copyValue;
	hashtable->freeValue = freeValue;
	hashtable->printValue = printValue;
	hashtable->equalKey = equalKey;
	hashtable->transformIntoNumber = transformIntoNumber;
	return hashtable;
}

//destroys a hash table - go for each cell in the table , go to its linked list and destroys its values,
//then destroy the linked list, and then free the table array and finally destroy the struct itself.
status destroyHashTable(hashTable hashtable) {
	if (hashtable == NULL) {
		return success;//its already deleted
	}
	int i, j;
	for (i = 0; i < hashtable->hashNumber; i++) {
		int listLength = getLengthList(hashtable->table[i]);
		for(j=1; j<= listLength; j++){						//STARTS FROM INDEX 1
			KeyValuePair pairToDestroy= (KeyValuePair)getDataByIndex(hashtable->table[i], j);
			//destroyKeyValuePair(pairToDestroy);
		}
		destroyList(hashtable->table[i]);
	}
	free(hashtable->table);
	free(hashtable);
	return success;
}

//adds an element to the hash table - the element will go to the cell by its key,
//and then will be appended to the end of the linked list that in the cell.
status addToHashTable(hashTable hashtable, Element key, Element value) {
	if (hashtable == NULL || key == NULL || value == NULL)
		return failure;
	int index = (hashtable->transformIntoNumber(key) % hashtable->hashNumber);//CHANGED TO "X%n"
	if(searchByKeyInList(hashtable->table[index], key) != NULL)	//check if the key already exists in list
		return failure;
	KeyValuePair newKeyVP = createKeyValuePair(key, value, hashtable->copyKey,	//hidden malloc
			hashtable->copyValue, hashtable->freeKey, hashtable->freeValue,
			hashtable->printKey, hashtable->printValue, hashtable->equalKey);

	if(newKeyVP == NULL)
		return failure;
	if(appendNode(hashtable->table[index], newKeyVP) == success){	//check the append went well
		newKeyVP= nullElements(newKeyVP);
		destroyKeyValuePair(newKeyVP);
		return success;
	}
	destroyKeyValuePair(newKeyVP);
	return failure;
}

//finds an element by its key in the hash table - first finds the cell by its key,
//then search in the linked list for this specific element
Element lookupInHashTable(hashTable hashtable, Element key) {
	if (hashtable == NULL || key == NULL)
		return NULL;
	int index = (hashtable->transformIntoNumber(key) % hashtable->hashNumber); //calculate the keys cell
	LinkedList indexList = hashtable->table[index];	//go to the list in index.
	int listLength = getLengthList(indexList);	//get the length of the list
	if(listLength == 0)
		return NULL;
	return  searchByKeyInList(indexList, key);

}

//removes an element from a hash table by it's key - first find the correct cell according to key,
//then delete element from list.
status removeFromHashTable(hashTable hashtable, Element key) {
	if (hashtable == NULL || key == NULL)
		return failure;
	Element found= lookupInHashTable(hashtable, key);
	if(found == NULL)
		return failure;
	int index = (hashtable->transformIntoNumber(key) % hashtable->hashNumber); //calculate the keys cell
	LinkedList indexList = hashtable->table[index];	//go to the list in index.
	return deleteNode(indexList, found); //returns status


}

//prints all the hash table - first the key, then the value its attached to.
status displayHashElements(hashTable hashtable) {
	if (hashtable == NULL )
		return failure;
	int i, j;
	for(i=0; i<hashtable->hashNumber; i++){	//for each cell in table => for each linked list
		//int listLength = getLengthList(hashtable->table[i]);	//get the length of the list
		//for (j=0; j<listLength; j++){	//print each element in list
	//		if((hashtable->printKey(hashtable->table[i])) == failure||(hashtable->printValue(hashtable->table[i])))
			//	return failure;
			displayList(hashtable->table[i]);
		//}
	}
	return success;
}
