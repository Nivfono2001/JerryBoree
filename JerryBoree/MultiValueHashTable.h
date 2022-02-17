#ifndef MULTI_VALUE_HASH_TABLE_H
#define MULTI_VALUE_HASH_TABLE_H
#include "Defs.h"
#include "LinkedList.h"

typedef struct MultiValueHashTable_t *multiValueHashTable;

multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
		CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
		TransformIntoNumberFunction transformIntoNumber, int hashNumber, EqualFunction equalValue,
		EqualFunction isPartOfValEq);
status destroyMultiValueHashTable(multiValueHashTable);
status addToMultiValueHashTable(multiValueHashTable, Element key,Element value);
LinkedList lookupInMultiValueHashTable(multiValueHashTable, Element key);
status removeFromMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element value);
status displayMultiValueHashElementsByKey(multiValueHashTable, Element key);

#endif /* HASH_TABLE_H */
