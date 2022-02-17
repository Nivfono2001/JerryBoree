#ifndef KEY_PER_VALUE_H
#define KEY_PER_VALUE_H
#include "Defs.h"

//initialization of the pointer to KeyValuePair struct
typedef struct KeyValuePair_t *KeyValuePair;

//function declaration
KeyValuePair createKeyValuePair(Element key, Element value,
		CopyFunction copyKeyFunction, CopyFunction copyValueFunction,
		FreeFunction freeKeyFunction, FreeFunction freeValueFunction,
		PrintFunction printKeyFunction, PrintFunction printValueFunction,
		EqualFunction equalKeyFunction) ;
status destroyKeyValuePair(Element keyValuePair);
status displayValue(KeyValuePair keyValuePair);
status displayKey(KeyValuePair keyValuePair);
Element getValue(KeyValuePair keyValuePair);
Element getKey(KeyValuePair keyValuePair);
bool isEqualKey(Element keyValuePair, Element key);
Element nullElements(Element keyValuePair);
//extra functions
bool isEqualPairs(Element keyValuePair1,Element keyValuePair2);
Element copyPair(Element keyPerValue);
status displayKeyValue(Element keyValuePair);

#endif /* KEY_PER_VALUE_H */
