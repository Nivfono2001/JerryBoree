#include "KeyValuePair.h"

struct KeyValuePair_t {
	Element key;
	Element value;
	CopyFunction copyKeyFunction, copyValueFunction;
	FreeFunction freeKeyFunction, freeValueFunction;
	PrintFunction printKeyFunction, printValueFunction;
	EqualFunction equalKeyFunction;
};

//creates a new KeyValuePair
KeyValuePair createKeyValuePair(Element key, Element value,
		CopyFunction copyKeyFunction, CopyFunction copyValueFunction,
		FreeFunction freeKeyFunction, FreeFunction freeValueFunction,
		PrintFunction printKeyFunction, PrintFunction printValueFunction,
		EqualFunction equalKeyFunction) {
	if (key == NULL || value == NULL || copyKeyFunction == NULL
			|| equalKeyFunction == NULL || copyValueFunction == NULL
			|| freeKeyFunction == NULL || freeValueFunction == NULL
			|| printValueFunction == NULL || printKeyFunction == NULL)
		return NULL;
	KeyValuePair keyValuePair = (KeyValuePair) malloc(sizeof(struct KeyValuePair_t));
	if (keyValuePair == NULL) {
		printf("Memory Problem\n");
		return NULL;
	}
	keyValuePair->key = key;
	keyValuePair->value = value;
	keyValuePair->copyKeyFunction = copyKeyFunction;
	keyValuePair->freeKeyFunction = freeKeyFunction;
	keyValuePair->printKeyFunction = printKeyFunction;
	keyValuePair->copyValueFunction = copyValueFunction;
	keyValuePair->freeValueFunction = freeValueFunction;
	keyValuePair->printValueFunction = printValueFunction;
	keyValuePair->equalKeyFunction = equalKeyFunction;
	return keyValuePair;
}

//destroys a keyValuePair
status destroyKeyValuePair(Element keyValuePair) {
	if(keyValuePair == NULL)
		return failure;
	KeyValuePair pair = (KeyValuePair)keyValuePair;
	pair->freeKeyFunction(pair->key); //free the  key inside the free function
	pair->freeValueFunction(pair->value); //free the val  inside the free function
	free(pair);
	return success;
}

//prints value of the pair
status displayValue(KeyValuePair keyValuePair) {
	if (keyValuePair == NULL)
		return failure;
	return(keyValuePair->printValueFunction(keyValuePair->value));
}

//prints key of the pair
status displayKey(KeyValuePair keyValuePair) {
	if (keyValuePair == NULL)
		return failure;
	return(keyValuePair->printKeyFunction(keyValuePair->key));
}

//returns the value of the pair
Element getValue(KeyValuePair keyValuePair) {
	if (keyValuePair == NULL )
		return NULL;
	return keyValuePair->value;
	//return keyValuePair->copyValueFunction(keyValuePair->value); //not sure that copy func is needed here
}
//returns the key of the pair
Element getKey(KeyValuePair keyValuePair) {
	if (keyValuePair == NULL )
		return NULL;
	return keyValuePair->key;
	//return keyValuePair->copyKeyFunction(keyValuePair->key); //not sure that copy func is needed here
}

//checks if given key is equal to pair's key
bool isEqualKey(Element keyValuePair, Element key) {
	if (keyValuePair == NULL || key == NULL)
		return false;
	KeyValuePair pair = (KeyValuePair)keyValuePair;
	return pair->equalKeyFunction(pair->key, key);
}

//displays the whole pair
status displayKeyValue(Element keyValuePair){
	if (keyValuePair == NULL)
		return failure;
	KeyValuePair pair = (KeyValuePair)keyValuePair;
	(pair->printKeyFunction(pair->key));
	(pair->printValueFunction(pair->value));
	return success;
}

//checks if two pairs are equal by comparing their keys
bool isEqualPairs(Element keyValuePair1, Element keyValuePair2){
	if (keyValuePair1== NULL|| keyValuePair2 == NULL )
			return false;
	KeyValuePair pair1 = (KeyValuePair)keyValuePair1;
	KeyValuePair pair2 = (KeyValuePair)keyValuePair2;
	return isEqualKey(pair1, pair2->key);
}

//copy function for KVP - use in hashTable
Element copyPair(Element keyPerValue){
	if(keyPerValue == NULL)
		return NULL;
	KeyValuePair pair = (KeyValuePair)keyPerValue;
	Element key = getKey(pair);
	Element value = getValue(pair);
	KeyValuePair newPair = createKeyValuePair(key, value, pair->copyKeyFunction, pair->copyValueFunction, pair->freeKeyFunction, pair->freeValueFunction, pair->printKeyFunction, pair->printValueFunction, pair->equalKeyFunction);
	if(newPair == NULL)
		return NULL;
	return newPair;
}

//gets a pair and changes its kay and value pointers to NULL
//help us to destroy a pair without destroying the pointed by it elements.
Element nullElements(Element keyValuePair) {
	if (keyValuePair == NULL)
		return NULL;
	KeyValuePair pair = (KeyValuePair) keyValuePair;
	pair->key = NULL;
	pair->value = NULL;
	return pair;

}
